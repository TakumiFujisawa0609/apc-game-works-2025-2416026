#pragma region インクルード
#include "GameScene.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <string>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Utility/Vector2.h"
#include "../Object/Player.h"
#include "../Manager/EffectManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Resource.h"
#include "../Common/Camera.h"
#include "../Common/HpBer.h"
#include "../Common/Font.h"
#pragma endregion

using PAD_BTN = InputManager::PAD_BTN;

GameScene::GameScene(void)
{
	/*　デフォルトコンストラクタ　*/
	gameState_ = GAME_STATE::NONE;
}

void GameScene::Load(void)
{

}

void GameScene::Init(void)
{
	/*　初期化処理　*/

	// プレイヤー処理
	player_ = new Player();

	// 当たり判定マネージャ
	colManager_ = new CollisionManager();


	// 初期化処理
	ReInit();

	// BGM再生
	SoundManager::GetInstance().Play(SoundManager::SRC::BGM_GAME,
		Sound::TIMES::LOOP, true);
}
void GameScene::ReInit(void)
{
	// ゲーム有効化
	gameState_ = GAME_STATE::IDLE;


	// プレイヤー処理
	player_->Init({}, 90.0f);
	/*
	colManager_->Init(gameStage_->GetStageModelHandle(), stage->GetStagePos(), stage->GetStageScale(),
		gameStage_->GetDamageModelHandle(), stage->GetDamagePos(), stage->GetDamageScale());
		*/


	// カメラ移動領域割り当て
	Camera& camera = Camera::GetInstance();
	camera.Init(Camera::MODE::FLLOW, player_->GetPos(), player_->GetRotationLocal().y);
	//camera.SetPosLimit(stage->GetCameraMoveMin(), stage->GetCameraMoveMax());

	// カメラ追従対象初期化
	camera.SetTrackingTarget(&player_->GetPos());
}



void GameScene::Update(void)
{
	/*　更新処理　*/

	// ステージ更新処理
	//gameStage_->Update();

	// 当たり判定更新
	colManager_->Update();

	if (gameState_ == GAME_STATE::IDLE)
	{
		GameIdleProc();
	}
	else if (gameState_ == GAME_STATE::GAME_OVER)
	{
		GameOverProc();
	}
	else if (gameState_ == GAME_STATE::RESULT)
	{
		if (IsCheck())
		{
			// 決定処理後、シーン遷移
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
}

void GameScene::Draw(void)
{
	/*　描画処理　*/
	Font& font = Font::GetInstance();
	Vector2 textPos = {};
	Vector2 midPos = { (Application::SCREEN_SIZE_X / 2), (Application::SCREEN_SIZE_Y / 2) };
	int x;

	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();

	// プレイヤー描画
	player_->Draw();

	//ステージ描画処理
	//gameStage_->Draw();

	// Effekseerにより再生中のエフェクトを描画する
	DrawEffekseer3D();

	// ゲーム描画
	DrawUI();

#ifdef _DEBUG

	//colManager_->DrawDebug();

	player_->DrawDebug();
#endif
}

void GameScene::Release(void)
{
	/*　解放処理　*/

	// 当たり判定マネージャ解放
	delete colManager_;

	// プレイヤー解放・削除
	player_->Release();
	delete player_;

	//ステージ解放処理
	//gameStage_->Release();
	//delete gameStage_;

	// 全音声停止
	SoundManager::GetInstance().StopAll();
}


bool GameScene::IsCheck(void)
{
	bool ret = false;
	InputManager& input = InputManager::GetInstance();
	int pad1 = static_cast<int>(InputManager::JOYPAD_NO::PAD1);

	if (input.PadIsBtnTrgDown(pad1, PAD_BTN::START) ||
		input.PadIsBtnTrgDown(pad1, PAD_BTN::RIGHT) ||
		input.PadIsBtnTrgDown(pad1, PAD_BTN::DOWN) ||

		input.KeyIsTrgDown(KEY_INPUT_SPACE) ||
		input.KeyIsTrgDown(KEY_INPUT_RETURN))
	{
		ret = true;
	}
	return ret;

}

void GameScene::DrawUI(void)
{
	// 勝利した対象名をテキストに反映
	Font& font = Font::GetInstance();
	
}

void GameScene::GameIdleProc(void)
{
	// プレイヤー更新処理
	player_->Update();
}

void GameScene::GameOverProc(void)
{
	
}
