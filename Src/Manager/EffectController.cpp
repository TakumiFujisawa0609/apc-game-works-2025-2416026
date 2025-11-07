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
		SetScalePlayingEffekseer3DEffect(
			effect.playId,
			effect.size.x,
			effect.size.y,
			effect.size.z
		);

		//エフェクトの回転
		SetRotationPlayingEffekseer3DEffect(
			effect.playId,
			effect.angle.x,
			effect.angle.y,
			effect.angle.z
		);

		//エフェクトの位置
		SetPosPlayingEffekseer3DEffect(
			effect.playId,
			effect.pos.x,
			effect.pos.y,
			effect.pos.z
		);

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

void EffectController::ChangeEffect(EFFECT_TYPE type, VECTOR pos)
{
	// 既存のエフェクトを削除
	if (!GetIsEffectAlive(type)) return;
	/*
	pos_ = pos;

	ResourceManager& res = ResourceManager::GetInstance();

	switch (type_)
	{
	case EffectController::EFFECT_TYPE::COTTON_DAMAGE:
	{
		effect_ = new EffectCotton();
		effectId_ = res.LoadHandleId(ResourceManager::EFFECT_DAMAGE);
		effect_->Init(effectId_, pos_);
	}
	break;


	case EffectController::EFFECT_TYPE::COTTON_KNOCK:
	{
		effect_ = new EffectCotton();
		effectId_ = res.LoadHandleId(ResourceManager::EFFECT_KNOCK);
		effect_->Init(effectId_, pos_);
	}
	break;

	case EffectController::EFFECT_TYPE::STAR:
	{
		effect_ = new EffectStar();
		effectId_ = res.LoadHandleId(ResourceManager::EFFECT_STAR);
		effect_->Init(effectId_, pos_);
	}
	break;

	case EffectController::EFFECT_TYPE::NONE:
	default:
	{
		// エフェクトなしの場合は何もしない
		effectId_ = -1;
	}
	break;
	}
	effects_.*/
}

bool EffectController::GetIsEffectAlive(EFFECT_TYPE type)
{
	return (effects_[type]->aliveTime > 0.0f &&
			effects_[type]->handle != -1);
}