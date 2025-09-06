#include "AnimationController.h"
#include <DxLib.h>
#include <string>
#include <cassert>
#include "../Manager/SceneManager.h"


AnimationController::AnimationController(void)
{
	animations_.clear();
}

AnimationController::AnimationController(int modelId)
{
	modelId_ = modelId;
	playType_ = -1;
	isLoop_	  = true;
}


void AnimationController::AddInFbx(int type, float speed, int animIndex)
{
	Animation animation;
	animation.animIndex = animIndex;

	animation.speed = speed;

	// アニメーション割り当て処理
	Add(type, speed, animation);
}
void AnimationController::Add(int type, float speed, const std::string path)
{
	Animation animation;
	animation.model = MV1LoadModel(path.c_str());
	animation.animIndex = -1;

	animation.speed = speed;

	// アニメーション割り当て処理
	Add(type, speed, animation);
}

void AnimationController::Play(int type, bool isLoop)
{
	// 同じアニメーションだったら再生を継続する
	if (playType_ == type) return;
	
	if (playType_ != -1)
	{
		// モデルからアニメーションを外す
		MV1DetachAnim(modelId_, playAnim_.attachNo);
	}

	// 停止を解除
	isStop_ = false;

	// アニメーション種別を変更
	playType_ = type;
	playAnim_ = animations_[type];

	// 初期化
	playAnim_.step = 0.0f;	

	// モデルにアニメーションを付ける
	if (playAnim_.model == -1)
	{
		// モデルと同じファイルからアニメーションをアタッチする
		playAnim_.attachNo = MV1AttachAnim(modelId_, playAnim_.animIndex);
	}
	else
	{
		// 別のモデルファイルからアニメーションをアタッチする
		// DxModelViewerを確認すること(大体0か1)
		int animIdx = 0;
		playAnim_.attachNo = MV1AttachAnim(modelId_, animIdx, playAnim_.model);
	}

	// アニメーション総時間の取得
	playAnim_.totalTime = MV1GetAttachAnimTotalTime(modelId_, playAnim_.attachNo);

	// アニメーションループ
	isLoop_ = isLoop;
}


void AnimationController::Update(void)
{
	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	if (playAnim_.attachNo == -1) return;


	// 再生
	if (!isStop_)
	{
		playAnim_.step += (deltaTime * playAnim_.speed);
	}

	if (playAnim_.step > playAnim_.totalTime)
	{
		if (isLoop_)
		{
			// 再生位置リセット
			playAnim_.step = 0.0f;
		}
		else
		{
			playAnim_.step = playAnim_.totalTime;
		}
	}
	
	// アニメーション設定
	MV1SetAttachAnimTime(modelId_, playAnim_.attachNo, playAnim_.step);
}

void AnimationController::DrawDebug(void)
{
#ifdef _DEBUG
	// アニメーションの描画
	DrawFormatString(0,64,0xFF0000,"animTime:%.2f",playAnim_.step);
#endif // _DEBUG
}

void AnimationController::Release(void)
{
	// ロードしたアニメーションを解放
	for (auto& pair : animations_)
	{
		if (pair.second.model != -1)
		{
			MV1DeleteModel(pair.second.model);
		}
	}

#pragma region beta
	/*
	// ロードしたアニメーションを解放
	for (const std::pair<int, Animation>& pair : animations_)
	{
		if (pair.second.model != -1)
		{
			MV1DeleteModel(pair.second.model);
		}
	}*/
#pragma endregion

	// リスト解放
	animations_.clear();

	MV1DeleteModel(modelId_);
}

bool AnimationController::IsEnd(void) const
{
	// アニメーションが終了しているか
	if (playAnim_.step >= playAnim_.totalTime)
	{
		// アニメーションが終了している
		return true;
	}

	if (isLoop_)
	{
		// ループ時は終了していない判定
		return false;
	}

	return false;
}

int AnimationController::GetPlayType(void)
{
	return playType_;
}

void AnimationController::Stop(bool isStop)
{
	isStop_ = isStop;
}

void AnimationController::SetAnimStep(float step)
{
	// 再生位置がマイナス時、０にする
	step = ((step < 0.0f) ? 0.0f : step);

	// 再生位置が最大時間を超えたとき、最大時間にする
	step = ((step > playAnim_.totalTime) ? playAnim_.totalTime : step);

	if (playType_ != -1)
	{
		// 再生位置割り当て
		playAnim_.step = step;
	}

#ifdef _DEBUG
	else
	{
		OutputDebugString("\nアニメーションが割り当てられていないため、再生位置の割り当てが出来ませんでした；；\n");
		assert(false); // 例外スロー
	}
#endif
}

float AnimationController::GetPlayTime(void)
{
	float time = -1;
	if (playType_ != -1)
	{
		time = playAnim_.step;
	}

#ifdef _DEBUG
	else
	{
		OutputDebugString("\nアニメーションが割り当てられていないため、再生時間が取得出来ませんでした；；\n");
		assert(false); // 例外スロー
	}
#endif

	return time;
}

float AnimationController::GetPlayTimeTotal(void)
{
	float time = -1;
	if (playType_ != -1)
	{
		time = playAnim_.totalTime;
	}

#ifdef _DEBUG
	else
	{
		OutputDebugString("\nアニメーションが割り当てられていないため、総再生時間が取得出来ませんでした；；\n");
		assert(false); // 例外スロー
	}
#endif

	return time;
}

void AnimationController::Add(int type, float speed, Animation& animation)
{
	animation.speed = speed;

	
	if (animations_.count(type) == 0)
	{
		// 動的配列に追加
		animations_.emplace(type, animation);
	}
}