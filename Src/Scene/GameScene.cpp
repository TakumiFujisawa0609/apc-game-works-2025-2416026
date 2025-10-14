#pragma region インクルード
#include "GameScene.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <string>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Vector2.h"
#include "../Object/Player.h"
#include "../Object/Enemy/EnemyController.h"
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

	Load();
}


void GameScene::Load(void)
{
	// プレイヤー処理
	player_ = new Player();
	player_->Load();

	// 敵マネージャー
	EnemyController::CreateInstance(*player_);

	EnemyController& enemys = EnemyController::GetInstance();

	// 当たり判定マネージャ
	CollisionManager::CreateInstance(*player_);
}

void GameScene::Init(void)
{
	/*　初期化処理　*/


	// 初期化処理
	ReInit();

	// BGM再生
	SoundManager::GetInstance().Play(SoundManager::SRC::BGM_GAME,
		true, true);
}
void GameScene::ReInit(void)
{
	// ゲーム有効化
	gameState_ = GAME_STATE::IDLE;

	// 敵初期化処理
	EnemyController::GetInstance().Init();

	// プレイヤー処理
	player_->Init(POS_START_PLAYER, 90.0f);
	/*
	colManager_->Init(gameStage_->GetStageModelHandle(), stage->GetStagePos(), stage->GetStageScale(),
		gameStage_->GetDamageModelHandle(), stage->GetDamagePos(), stage->GetDamageScale());
		*/


	// カメラ移動領域割り当て
	Camera& camera = Camera::GetInstance();
	camera.Init(Camera::MODE::FLLOW, player_->GetPos(), player_->GetRotationLocal().y, player_);

	// カメラ追従対象初期化
	camera.SetTrackingTarget(&player_->GetPos());
}



void GameScene::Update(void)
{
	/*　更新処理　*/

	// ステージ更新処理
	//gameStage_->Update();

	// 当たり判定更新
	CollisionManager::GetInstance().Update();


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

	Camera& camera = Camera::GetInstance();
	// カメラ位置更新
	camera.UpdatePlayerTransform(&player_->GetPos(), &player_->GetRotation());
	camera.SetTrackingTarget(&player_->GetPos());
}

void GameScene::Draw(void)
{
	/*　描画処理　*/
	Font& font = Font::GetInstance();
	Vector2 textPos = {};
	Vector2 midPos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };

	// グリッド線描画
	SceneManager::GetInstance().DrawGrid();

	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();

	// 敵描画
	EnemyController::GetInstance().Draw();

	// プレイヤー描画
	player_->Draw();

	//ステージ描画処理
	//gameStage_->Draw();

	// Effekseerにより再生中のエフェクトを描画する
	DrawEffekseer3D();

	// ゲーム描画
	DrawUI();

#ifdef _DEBUG

	CollisionManager::GetInstance().DrawDebug();

	EnemyController::GetInstance().DrawDebug();

	player_->DrawDebug();

	Camera::GetInstance().DrawDebug();
#endif
}

void GameScene::Release(void)
{
	/*　解放処理　*/

	// 当たり判定マネージャ解放
	CollisionManager::GetInstance().Destroy();

	// 敵マネージャ解放
	EnemyController::GetInstance().Destroy();

	// 当たり判定解放
	//CollisionManager::Destroy();

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

	if (input.PadIsBtnTrgDown(InputManager::PAD_NO::PAD1, PAD_BTN::START) ||
		input.PadIsBtnTrgDown(InputManager::PAD_NO::PAD1, PAD_BTN::RIGHT) ||
		input.PadIsBtnTrgDown(InputManager::PAD_NO::PAD1, PAD_BTN::DOWN) ||

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
	// プレイヤー更新
	player_->Update();

	// 敵マネージャ更新
	EnemyController::GetInstance().Update();
}

void GameScene::GameOverProc(void)
{
	
}
