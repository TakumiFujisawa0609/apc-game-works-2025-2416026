#include "ActorBase.h"
#include <DxLib.h>
#include "../Common/Transform.h"

ActorBase::ActorBase(void) :
	sceneMng_(SceneManager::GetInstance()),
	resMng_(ResourceManager::GetInstance()),
	transform_(nullptr)
{
}

void ActorBase::Load(void)
{
	transform_ = new Transform();

	// 各オブジェクト独自の読み込み
	LoadPost();
}

void ActorBase::Init(const VECTOR& _pos, float _angleY)
{

}

void ActorBase::Update(void)
{
	transform_->Update();
}

void ActorBase::Draw(void)
{
	// モデル描画処理
	MV1DrawModel(transform_->modelId);
}

void ActorBase::Release(void)
{
	if (transform_)
	{
		delete transform_;
	}

	// その他解放
	ReleasePost();
}