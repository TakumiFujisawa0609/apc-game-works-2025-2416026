#pragma region インクルード
#include "SceneManager.h"
#include <DxLib.h>
#include <chrono>
#include "../Application.h"
#include "../Scene/SceneBase.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "./InputManager.h"
#include "./ResourceManager.h"
//#include "../Common/Fader.h"
#pragma endregion


SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance(void)
{
	/*　マネージャ生成処理　*/

	if (instance_ == nullptr) instance_ = new SceneManager();

	// 初期化処理
	instance_->Load();
}

SceneManager::SceneManager(void)
{
	/*　デフォルトコンストラクタ　*/

	sceneId_ = START_SCENE;
	waitSceneId_ = SCENE_ID::NONE;

	isChangeScene_ = false; // 遷移フラグ

	Load(); // 初期化処理
}


void SceneManager::Load(void)
{
	/*　初期化処理　*/

	curScene_ = nullptr;
	sceneId_	 = START_SCENE;	   // 現在シーン
	waitSceneId_ = SCENE_ID::NONE; // 待機シーンID
	DoChangeState(sceneId_); // シーン初期化

	isChangeScene_ = false; // 遷移フラグ無効化

	// フェーダ初期化
	//fader_ = new Fader();
	//fader_->Init();

	// 現在時間取得
	preTime_ = std::chrono::system_clock::now();


	// ヒットストップ初期化
	InitPerform();

	// 3D初期化処理
	Init3D();
}

void SceneManager::Init3D(void)
{
	/*　3D初期化処理　*/

	// 背景色割り当て
	SetBackgroundColor(BACK_COLOR.r, BACK_COLOR.g, BACK_COLOR.g);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込み有効化
	SetWriteZBuffer3D(true);

	// バックカリングを有効化
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);

	// ライトの向きを割り当て
	ChangeLightTypeDir(LIGHT_DIR);
}

void SceneManager::InitPerform(void)
{
	perform_.type = PERFORM_TYPE::NONE;
	perform_.time = 0.0f;
	perform_.shakePos = {};
	perform_.slowTerm = 0;
}


void SceneManager::Update(void)
{
	/*　更新処理　*/

	// シーン無効時は処理終了
	if (curScene_ == nullptr) return;

	// 演出処理
	bool isStop = Performance();
	if (isStop) return;

	// 経過時間カウント
	DeltaCount();

	//fader_->Update(); // フェード更新
	//if (isChangeScene_) Fade(); // フェード処理

	curScene_->Update(); // 現在のシーン更新処理
}

void SceneManager::Draw(void)
{
	/*　描画処理　*/

	// 現在シーン描画
	curScene_->Draw();

	//fader_->Draw(); // フェーダ描画

#ifdef _DEBUG

	Vector2 midPos = { (Application::SCREEN_SIZE_X / 2), (Application::SCREEN_SIZE_Y / 2) };

	// 三分割法グリッド線
	Vector2 lineGlid = { (Application::SCREEN_SIZE_X / 3), (Application::SCREEN_SIZE_Y / 3) };

	DrawBox(0, (lineGlid.y - 1), (midPos.x * 2), (lineGlid.y + 1),
			0x0, true);
	DrawBox(0, ((lineGlid.y * 2) - 1), (midPos.x * 2), ((lineGlid.y * 2) + 1),
			0x0, true);

	DrawBox((lineGlid.x - 1), 0, (lineGlid.x + 1), (midPos.y * 2),
			0x0, true);
	DrawBox(((lineGlid.x * 2) - 1), 0, ((lineGlid.x * 2) + 1), (midPos.y * 2),
			0x0, true);
#endif
}

void SceneManager::Destroy(void)
{
	/* メモリ解放処理 */

	if (curScene_ != nullptr)
	{
		// シーン解放
		curScene_->Release();
		delete curScene_;
	}

	//delete fader_;	 // フェーダ削除
	delete instance_; // マネージャ削除
}

void SceneManager::ChangeScene(SceneManager::SCENE_ID nextScene)
{
	/*　シーン遷移処理　*/

	// 遷移先シーンをメンバ変数に取得
	waitSceneId_ = nextScene;

	// 暗転して遷移開始フラグ 有効化
	//fader_->SetFade(Fader::FADE_STATE::FADE_OUT); 
	isChangeScene_ = true; 

	DoChangeState(nextScene); // シーン初期化
}

void SceneManager::DrawGrid(void)
{
	// XZ基本軸(グリッド)
	float num;
	VECTOR sPos;
	VECTOR ePos;
	for (int i = -GRID_HNUM; i < GRID_HNUM; i++)
	{

		num = static_cast<float>(i);

		// X軸(赤)
		sPos = { -GRID_HLEN, 0.0f, num * GRID_TERM };
		ePos = { GRID_HLEN, 0.0f, num * GRID_TERM };
		DrawLine3D(sPos, ePos, 0xff0000);
		DrawSphere3D(ePos, 20.0f, 10, 0xff0000, 0xff0000, true);

		// Z軸(青)
		sPos = { num * GRID_TERM, 0.0f, -GRID_HLEN };
		ePos = { num * GRID_TERM, 0.0f,  GRID_HLEN };
		DrawLine3D(sPos, ePos, 0x0000ff);
		DrawSphere3D(ePos, 20.0f, 10, 0x0000ff, 0x0000ff, true);

	}

	// Y軸(緑)
	sPos = { 0.0f, -GRID_HLEN, 0.0f };
	ePos = { 0.0f,  GRID_HLEN, 0.0f };
	DrawLine3D(sPos, ePos, 0x00ff00);
	DrawSphere3D(ePos, 20.0f, 10, 0x00ff00, 0x00ff00, true);

}

void SceneManager::DoChangeState(SCENE_ID nextScene)
{
	/*　遷移処理　*/

	sceneId_ = nextScene; // シーン状態遷移

	if (curScene_ != nullptr)
	{
		// 遷移前のシーンを解放
		curScene_->Release();
		delete curScene_;
	}

	// シーン状態遷移
	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		curScene_ = new TitleScene();
		break;

	case SCENE_ID::GAME:
		curScene_ = new GameScene();
		break;
	}

	curScene_->Init(); // 現在シーン初期化

	waitSceneId_ = SCENE_ID::NONE; // 待機シーン状態 無効化
}

void SceneManager::Fade(void)
{
	/*　各フェード処理　*/
	/*
	Fader::FADE_STATE fState = fader_->GetState();

	switch (fState)
	{
		// フェードアウト時
		case Fader::FADE_STATE::FADE_OUT:
		{
			if (fader_->GetFadeEnd())
			{
				// 完全暗転したらシーン遷移
				DoChangeState(waitSceneId_);
	
				// ロード状態化
				fader_->SetFade(Fader::FADE_STATE::LOADING);
			}
		}
		break;

		// ロード時
		case Fader::FADE_STATE::LOADING:
		{
			// 読み込み完了したか否か
			if (curScene_->IsLoad())
			{
				curScene_->Init(); // 現在シーン初期化
					
				// フェードアウト→フェードイン
				fader_->SetFade(Fader::FADE_STATE::FADE_IN);
			}
		}
		break;

		// フェードイン時
		case Fader::FADE_STATE::FADE_IN:
		{
			if (fader_->GetFadeEnd())
			{
				// フェード終了処理
				fader_->SetFade(Fader::FADE_STATE::NONE);
				isChangeScene_ = false;
			}
		}
		break;
	}*/

}

void SceneManager::DeltaCount(void)
{
	/*　経過時間の処理　*/

	auto nowTime = std::chrono::system_clock::now();

	// 今時間を前フレームの時間をナノ秒単位で計算
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);

	preTime_ = nowTime;
}

bool SceneManager::Performance(void)
{
	bool isStop = false;
	float delta = GetDeltaTime();

	if (perform_.time > 0.0f)
	{
		perform_.time -= delta;

		// ヒットストップ処理
		if (perform_.type == PERFORM_TYPE::HIT_STOP)
		{
			// 更新処理の停止
			isStop = true;
		}

		// 遅延処理
		if (perform_.type == PERFORM_TYPE::SLOW)
		{
			int slowCount = static_cast<int>(perform_.time * 10);

			if (slowCount % perform_.slowTerm != 0)
			{
				// 更新処理の停止
				isStop = true;
			}
		}
	}

	return isStop;
}