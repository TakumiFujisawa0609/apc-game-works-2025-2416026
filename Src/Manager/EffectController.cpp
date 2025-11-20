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

void EffectController::SetEffect(EFFECT_TYPE _type, const VECTOR& _pos, const VECTOR& _angle, const VECTOR& _scale)
{
	// 既存のエフェクトを削除
	if (!GetIsEffectAlive(_type)) { return; }
	
	ResourceManager& res = ResourceManager::GetInstance();
	Effect* effect = new Effect();

	effect->pos = _pos;
	effect->type = _type;
	effect->scale = _scale;
	effect->angle = _angle;
	

	switch (_type)
	{
		case EFFECT_TYPE::SWORD_SLASH:
		{
			effect->handle = res.LoadHandleId(ResourceManager::SRC::EFFECT_SWORD_SLASH);
		}
		break;

		case EFFECT_TYPE::SPAWN_SKELETON:
		{
			effect->handle = res.LoadHandleId(ResourceManager::SRC::EFFECT_SPAWN_SKELETON);
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

	// エフェクト格納
	effects_.emplace(_type, effect);
}

bool EffectController::GetIsEffectAlive(EFFECT_TYPE type)
{
	return (effects_[type]->aliveTime > 0.0f &&
			effects_[type]->handle != -1);
}

void EffectController::UpdateEffect(EFFECT_TYPE _type, Effect& _effect)
{
	// エフェクト終了時、処理終了
	if (!GetIsEffectAlive(_type)) { return; }

	// エフェクト再生
	_effect.playId = PlayEffekseer3DEffect(_effect.handle);

	// 再生したエフェクトのパラメータ割り当て
	SetScalePlayingEffekseer3DEffect(_effect.playId,
		_effect.scale.x,
		_effect.scale.y,
		_effect.scale.z);

	SetRotationPlayingEffekseer3DEffect(_effect.playId,
		_effect.angle.x,
		_effect.angle.y,
		_effect.angle.z);

	SetPosPlayingEffekseer3DEffect(_effect.playId,
		_effect.pos.x,
		_effect.pos.y,
		_effect.pos.z);

	// 生存時間更新
	_effect.aliveTime -= SceneManager::GetInstance().GetDeltaTime();

	// エフェクト停止
	if (_effect.aliveTime < 0.0f)
	{
		StopEffect(_type);
	}
}
