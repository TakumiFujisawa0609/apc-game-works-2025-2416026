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
	float delta = SceneManager::GetInstance().GetDeltaTime();

	for (auto [type, effects] : effects_)
	{
		Effect effect = *effects;

		if (!GetIsEffectAlive(type)) continue;

		effect.playId = PlayEffekseer3DEffect(effect.handle);

		//エフェクトの大きさ
		SetScalePlayingEffekseer3DEffect(effect.playId,
										 effect.scale.x,
										 effect.scale.y,
										 effect.scale.z);

		//エフェクトの回転
		SetRotationPlayingEffekseer3DEffect(effect.playId,
											effect.angle.x,
											effect.angle.y,
											effect.angle.z);

		//エフェクトの位置
		SetPosPlayingEffekseer3DEffect(effect.playId,
									   effect.pos.x,
									   effect.pos.y,
									   effect.pos.z);

		effect.aliveTime -= delta;

		// エフェクト停止
		if (effect.aliveTime < 0.0f)
		{
			StopEffect(type);
		}
	}
}

void EffectController::Draw(void)
{
	
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
	Effect effect;

	effect.pos = _pos;
	effect.type = _type;
	effect.scale = _scale;
	effect.angle = _angle;
	

	switch (_type)
	{
		case EFFECT_TYPE::SWORD_SLASH:
		{
			effect.handle = res.LoadHandleId(ResourceManager::SRC::EFFECT_SWORD_SLASH);
		}
		break;

		case EffectController::EFFECT_TYPE::SPAWN_SKELETON:
		{
			effect.handle = res.LoadHandleId(ResourceManager::SRC::EFFECT_SWORD_SLASH);
		}
		break;

		case EffectController::EFFECT_TYPE::NONE:
		default:
		{
			// エフェクトなしの場合は何もしない
			effect.handle = -1;
		}
		break;
	}
}

bool EffectController::GetIsEffectAlive(EFFECT_TYPE type)
{
	return (effects_[type]->aliveTime > 0.0f &&
			effects_[type]->handle != -1);
}