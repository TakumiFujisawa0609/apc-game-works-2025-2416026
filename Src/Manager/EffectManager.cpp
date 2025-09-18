#include "EffectManager.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/SceneManager.h"


EffectManager* EffectManager::instance_;

void EffectManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new EffectManager();
	}
	instance_->Init(); // マネージャ初期化処理
}

EffectManager& EffectManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
#ifdef _DEBUG
		OutputDebugString("\nエフェクトマネージャが生成されていません。\nエフェクトマネージャを生成しました。\n");
#endif
	}
	return *instance_;
}

EffectManager::EffectManager(void)
{
	effects_.clear();
}

void EffectManager::Init(void)
{

}

void EffectManager::Update(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();
	EFFECT effect;

	for (auto effects : effects_)
	{
		effect = effects.second;

		if (!GetIsEffectAlive(effects.first)) continue;

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
			StopEffect(effects.first);
		}

	}
}

void EffectManager::Draw(void)
{
	
}

void EffectManager::Destroy(void)
{
	
}

void EffectManager::StopEffect(EFFECT_TYPE type)
{
	if (!GetIsEffectAlive(type)) return;

	// エフェクト停止
	StopEffekseer3DEffect(effects_[type].playId);
}

void EffectManager::ChangeEffect(EFFECT_TYPE type, VECTOR pos)
{
	// 既存のエフェクトを削除
	if (!GetIsEffectAlive(type)) return;
	/*
	pos_ = pos;

	ResourceManager& res = ResourceManager::GetInstance();

	switch (type_)
	{
	case EffectManager::EFFECT_TYPE::COTTON_DAMAGE:
	{
		effect_ = new EffectCotton();
		effectId_ = res.LoadHandleId(ResourceManager::EFFECT_DAMAGE);
		effect_->Init(effectId_, pos_);
	}
	break;


	case EffectManager::EFFECT_TYPE::COTTON_KNOCK:
	{
		effect_ = new EffectCotton();
		effectId_ = res.LoadHandleId(ResourceManager::EFFECT_KNOCK);
		effect_->Init(effectId_, pos_);
	}
	break;

	case EffectManager::EFFECT_TYPE::STAR:
	{
		effect_ = new EffectStar();
		effectId_ = res.LoadHandleId(ResourceManager::EFFECT_STAR);
		effect_->Init(effectId_, pos_);
	}
	break;

	case EffectManager::EFFECT_TYPE::NONE:
	default:
	{
		// エフェクトなしの場合は何もしない
		effectId_ = -1;
	}
	break;
	}
	effects_.*/
}

bool EffectManager::GetIsEffectAlive(EFFECT_TYPE type)
{
	return (effects_[type].aliveTime > 0.0f &&
		effects_[type].handle != -1);
}