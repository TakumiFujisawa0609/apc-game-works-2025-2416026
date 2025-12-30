#pragma region インクルード
#include "GameScene.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <string>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Vector2.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/EnemyController.h"
#include "../Object/Stage/Stage.h"
#include "../Manager/EffectController.h"
#include "../Manager/SceneManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Resource.h"
#include "../Common/Camera.h"
#include "../Common/HpBer.h"
#include "../Common/Font.h"
#include "../Common/Perform.h"
#pragma endregion


GameScene::GameScene(void):
	gameState_(GAME_STATE::NONE),
	SceneBase()
{
	Load();
}


void GameScene::Load(void)
{
	// プレイヤー処理
	player_ = new Player();

	// 敵マネージャー
	enemys_ = new EnemyController(*player_);

	stage_ = new Stage();

	// 当たり判定マネージャ
	CollisionManager::CreateInstance(*player_, *enemys_);
}

void GameScene::Init(void)
{
	/*　初期化処理　*/

	// 初期化処理
	ReInit();


	// フォグ有効化
	SetFogEnable(true);

	// フォグの色割り当て
	SetFogColor(FOG_COLOR.r, FOG_COLOR.g, FOG_COLOR.b);

	// フォグの開始距離
	SetFogStartEnd(FOG_START, FOG_END);

	// BGM再生
	//SoundManager::GetInstance().Play(SoundManager::SRC::BGM_GAME,true, true);
}
void GameScene::ReInit(void)
{
	// ゲーム有効化
	gameState_ = GAME_STATE::IDLE;

	// 敵初期化処理
	enemys_->Init();

	// プレイヤー処理
	player_->Init(POS_START_PLAYER, 90.0f);
	/*
	colManager_->Init(gameStage_->GetStageModelHandle(), stage->GetStagePos(), stage->GetStageScale(),
		gameStage_->GetDamageModelHandle(), stage->GetDamagePos(), stage->GetDamageScale());
		*/


	// カメラ移動領域割り当て
	Camera& camera = sceneMng_.GetCamera();

	float angleY = static_cast<float>(player_->GetRotationLocal().y);
	camera.Init(Camera::MODE::FOLLOW, player_->GetPos(), angleY, player_);

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

	Camera& camera = sceneMng_.GetCamera();
	// カメラ位置更新
	camera.UpdatePlayerTransform(&player_->GetPos(), &player_->GetRotation());
	camera.SetTrackingTarget(&player_->GetPos());

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
		if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_DECISION))
		{
			// 決定処理後、シーン遷移
			sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
}

void GameScene::Draw(void)
{
	/*　描画処理　*/
	Font& font = Font::GetInstance();

	Vector2 textPos = AsoUtility::VECTOR2_ZERO;
	Vector2 midPos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };
	/*
	// 地面
	DrawCube3D({ -5000.0f, 0.0f, -2500.0f },
			   { 5000.0f, -100.0f, 30000.0f },
				0x00aa00, 0x0, true);*/

	stage_->Draw();
	
	// グリッド線描画
#ifdef _DEBUG
	sceneMng_.DrawGrid();
#endif

	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();

	// 敵描画
	enemys_->Draw();

	// プレイヤー描画
	player_->Draw();


	// Effekseerにより再生中のエフェクトを描画する
	DrawEffekseer3D();

	// ゲーム描画
	DrawUI();

	enemys_->DrawDebug();
	 
	player_->DrawDebug();

#ifdef _DEBUG
	/*
	CollisionManager::GetInstance().DrawDebug();

	sceneMng_.DrawDebug();*/
#endif
}

void GameScene::Release(void)
{
	/*　解放処理　*/

	// 当たり判定マネージャ解放
	CollisionManager::GetInstance().Destroy();

	// 敵マネージャ解放
	enemys_->Release();

	// 当たり判定解放
	//CollisionManager::Destroy();

	// プレイヤー解放・削除
	player_->Release();
	delete player_;

	stage_->Release();
	delete stage_;

	//ステージ解放処理
	//gameStage_->Release();
	//delete gameStage_;

	// 全音声停止
	SoundManager::GetInstance().StopAll();
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
	enemys_->Update();

	if (enemys_->GetIsActiveBoss() && enemys_->GetIsDefeatBoss())
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}
	else if (player_->GetCurHp() <= 0)
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		//sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

	
}
