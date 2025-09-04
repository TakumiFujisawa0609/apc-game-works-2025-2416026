#include "FrameRate.h"
#include <DxLib.h>

FrameRate* FrameRate::manager_ = nullptr; // シングルトンインスタンス

FrameRate::FrameRate(void)
{
	curTime_ = lateTime_ = 0;

	counter_ = updateTime_ = 0;

	viewFramelate_ = 0;
}

void FrameRate::CreateManager(void)
{
	// インスタンス未生成時に生成する
	if (manager_ == nullptr) manager_ = new FrameRate();
}


void FrameRate::Update(void)
{
	// システムに処理を返す。「世界」ッ！時よ止まれ！
	Sleep(1);

	//現在時刻を取得する
	curTime_ = GetNowCount();
}


void FrameRate::Draw(void)
{
	// 平均フレームレート 描画
	DrawFormatString(0, 0, 0xFFFFFF, "%.1ffps", viewFramelate_);
}


void FrameRate::Release(void)
{
	if (manager_ == nullptr)
	{
		OutputDebugString("\nマネージャが生成されていません。\n");
		return;
	}
	delete manager_;
}


void FrameRate::SetFrameRate(void)
{
	lateTime_ = curTime_; // 前フレームの時間 割り当て

	counter_++; // フレームカウント増加

	// 現在時間との差分
	int nDifTime = (curTime_ - updateTime_);

	if (nDifTime > 1000)
	{
		// フレームレート単位変更(ミリ秒 → 秒)
		float fFrameCount = static_cast<float>(counter_ * 1000);

		// 描画フレーム単位 取得
		viewFramelate_ = (fFrameCount / nDifTime);

		counter_ = 0; // フレームカウント 初期化

		// フレームレート 更新
		updateTime_ = curTime_;
	}
}

bool FrameRate::GetLimitFrameRate(void)
{
	/*　フレームレート制限するか否か　*/
	if ((curTime_ - lateTime_) >= FRAME_RATE)
	{
		return false;
	}

	return true;
}