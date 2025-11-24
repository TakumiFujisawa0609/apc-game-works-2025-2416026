#include "EffectController.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <map>
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/SceneManager.h"


EffectController::EffectController(void)
{
	effects_.clear();
}

void EffectController::Init(void)
{

}

void EffectController::Update(void)
{
	if (effects_.empty()) { return; }

	for (auto& [type, effects] : effects_)
	{
		UpdateEffect(type, *effects);
	}
}

void EffectController::StopEffect(EFFECT_TYPE type)
{
	if (!GetIsEffectAlive(type)) return;
	
	// エフェクト停止
	StopEffekseer3DEffect(effects_[type]->playId);
}

void EffectController::SetEffect(EFFECT_TYPE _type, const VECTOR& _pos, float _aliveTime, const VECTOR& _angle, const VECTOR& _scale)
{
	// 既存のエフェクトを削除
	//if (!GetIsEffectAlive(_type)) { return; }
	
	Effect* effect = new Effect();

	effect->pos = _pos;
	effect->type = _type;
	effect->scale = _scale;
	effect->angle = _angle;
	effect->aliveTime = _aliveTime;
	

	switch (_type)
	{
		case EFFECT_TYPE::SWORD_SLASH:
		{
			effect->handle = ResourceManager::GetInstance().LoadHandleId(ResourceManager::SRC::EFFECT_SWORD_SLASH);
		}
		break;

		case EFFECT_TYPE::SPAWN_SKELETON:
		{
			effect->handle = ResourceManager::GetInstance().LoadHandleId(ResourceManager::SRC::EFFECT_SPAWN_SKELETON);
		}
		break;

		case EffectController::EFFECT_TYPE::NONE:
		default:
		{
			// エフェクトなしの場合は何もしない
			return;
		}
		break;
	}

	// エフェクト再生
	effect->playId = PlayEffekseer3DEffect(effect->handle);

	// 再生したエフェクトのパラメータ割り当て
	SetScalePlayingEffekseer3DEffect(effect->playId,
		effect->scale.x,
		effect->scale.y,
		effect->scale.z);

	SetRotationPlayingEffekseer3DEffect(effect->playId,
		effect->angle.x,
		effect->angle.y,
		effect->angle.z);

	SetPosPlayingEffekseer3DEffect(effect->playId,
		effect->pos.x,
		effect->pos.y,
		effect->pos.z);

	// エフェクト格納
	effects_.emplace(_type, effect);
}

bool EffectController::GetIsEffectAlive(EFFECT_TYPE type)
{
	if (effects_[type] == nullptr) { return false; }

	return (effects_[type]->aliveTime > 0.0f &&
			effects_[type]->handle != -1);
}

void EffectController::UpdateEffect(EFFECT_TYPE _type, Effect& _effect)
{
	// エフェクト終了時、処理終了
	if (!GetIsEffectAlive(_type)) { return; }

	// 生存時間更新
	_effect.aliveTime -= SceneManager::GetInstance().GetDeltaTime();

	// エフェクト停止
	if (_effect.aliveTime < 0.0f)
	{
		//StopEffect(_type);
	}
}
