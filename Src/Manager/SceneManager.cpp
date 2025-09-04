/* -------------------
　シーン管理マネージャ
 --------------------- */
#pragma region インクルード
//#include "../Common/Fader.h"

#include "../Scene/SceneBase.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"

#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

#include <chrono>
#include <DxLib.h>

#pragma endregion

SceneManager* SceneManager::manager_ = nullptr;

/*　マネージャ生成処理　*/
void SceneManager::CreateInstance(void)
{
	if (manager_ == nullptr) manager_ = new SceneManager();

	manager_->Init(); // 初期化処理
}

/*　シーンマネージャ取得処理　*/
SceneManager& SceneManager::GetInstance(void)
{
	return *manager_;
}

/*　デフォルトコンストラクタ　*/
SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	isChangeScene_ = false; // 遷移フラグ

	Init(); // 初期化処理
}

/*　コピーコンストラクタ　*/
SceneManager::SceneManager(const SceneManager &other) {}


/*　初期化処理　*/
void SceneManager::Init(void)
{
	sceneId_	 = START_SCENE;	   // 現在シーン
	waitSceneId_ = SCENE_ID::NONE; // 待機シーンID
	DoChangeState(sceneId_); // シーン初期化

	isChangeScene_ = false; // 遷移フラグ無効化

	// フェーダ初期化
	//fader_ = new Fader();
	//fader_->Init();

	// 現在時間取得
	preTime_ = std::chrono::system_clock::now();

	Init3D(); // 3D初期化処理
}

/*　3D初期化処理　*/
void SceneManager::Init3D(void)
{
	// 背景色割り当て
	SetBackgroundColor(0, 0, 0);

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


/*　更新処理　*/
void SceneManager::Update(void)
{
	// シーン無効時は処理終了
	if (curScene_ == nullptr) return;

	DeltaCount(); // 経過時間カウント

	//fader_->Update(); // フェード更新
	if (isChangeScene_) Fade(); // フェード処理


	if (sceneId_ == SCENE_ID::TITLE &&
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_RETURN) &&
		InputManager::GetInstance().KeyIsNew(KEY_INPUT_LSHIFT))
	{
		sceneId_ = SCENE_ID::GAME;
		DoChangeState(sceneId_); // シーン初期化
	}

	curScene_->Update(); // 現在のシーン更新処理
}

/*　描画処理　*/
void SceneManager::Draw(void)
{
	curScene_->Draw(); // 現在シーン描画

	//fader_->Draw(); // フェーダ描画
}

/*　解放処理　*/
void SceneManager::Release(void)
{
	if (curScene_ != nullptr)
	{
		// シーン解放
		curScene_->Release();
		delete curScene_;
	}

	//delete fader_;	 // フェーダ削除
	delete manager_; // マネージャ削除
}

/*　シーン遷移処理　*/
void SceneManager::ChangeScene(SceneManager::SCENE_ID nextScene)
{
	// 遷移先シーンをメンバ変数に取得
	waitSceneId_ = nextScene;

	// 暗転して遷移開始フラグ 有効化
	//fader_->SetFade(Fader::FADE_STATE::FADE_OUT); 
	isChangeScene_ = true; 
}

/*　シーンID取得　*/
SceneManager::SCENE_ID SceneManager::GetSceneId(void) const
{
	return sceneId_;
}


/*　遷移処理　*/
void SceneManager::DoChangeState(SCENE_ID nextScene)
{
	if (curScene_ != nullptr)
	{
		// 遷移前のシーンを解放
		curScene_->Release();
		delete curScene_;
	}

	sceneId_ = nextScene; // シーン状態遷移

	/* シーン状態遷移 */
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

/*　経過時間取得　*/
float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

/*　各フェード処理　*/
void SceneManager::Fade(void)
{
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

/*　経過時間の処理　*/
void SceneManager::DeltaCount(void)
{
	auto nowTime = std::chrono::system_clock::now();
	// 今時間を前フレームの時間をナノ秒単位で計算
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;
}