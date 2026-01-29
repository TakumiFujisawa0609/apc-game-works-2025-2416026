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


GameScene::GameScene(void) :
	gameState_(GAME_STATE::NONE),
	SceneBase(),
	curGameTime_(GAME_TIME),
	collisionMng_(nullptr)
{
	Load();
}


void GameScene::Load(void)
{
	// プレイヤー処理
	player_ = new Player();

	stage_ = new Stage();
	stage_->Init();

	// 敵マネージャー
	enemys_ = new EnemyController(*player_, *stage_);

	// 当たり判定マネージャ
	collisionMng_ = new CollisionManager(*player_, *enemys_, *stage_);
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
	collisionMng_->Update();

	Camera& camera = sceneMng_.GetCamera();
	// カメラ位置更新
	camera.UpdatePlayerTransform(&player_->GetPos(), &player_->GetRotation());
	camera.SetTrackingTarget(&player_->GetPos());

	// ゲーム時間減少
	curGameTime_ -= sceneMng_.GetDeltaTime();
	if (curGameTime_ <= 0.0f)
	{
		// 決定処理後、シーン遷移
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}


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

	collisionMng_->DrawDebug();

	stage_->Draw();
	
	// グリッド線描画
#ifdef _DEBUG
	if (sceneMng_.GetIsDebugMode())
	{
		sceneMng_.DrawGrid();
	}
#endif

	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();

	// 敵描画
	enemys_->Draw();

	// プレイヤー描画
	player_->Draw();


	// Effekseerにより再生中のエフェクトを描画する
	DrawEffekseer3D();

	DrawTimeFeed();

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
	collisionMng_->Release();

	// 敵マネージャ解放
	enemys_->Release();

	// 当たり判定解放
	//CollisionManager::Destroy();

	// プレイヤー解放・削除
	player_->Release();
	delete player_;

	//ステージ解放処理
	stage_->Release();
	delete stage_;
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

	if (!enemys_->GetIsActiveBoss())
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}
	else if (player_->GetCurHp() <= 0)
	{
		sceneMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		//sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

	
}

void GameScene::TimeFeed(void)
{

}

void GameScene::DrawTimeFeed(void)
{
	float range = (1.0f - (curGameTime_ / GAME_TIME));

	float size = 1.0f;

	// 線分割数
	const int DRAW_CNT = 30;

	// 各線の太さ
	const int LINE_SIZE = 5;

	// 最大透過値
	const int ALPHA_MAX = (255 - 0);

	// 各線の透過値
	const int ALPHA = (ALPHA_MAX / DRAW_CNT);

	const unsigned int COLOR = 0xffffff;

	const Vector2 POS_SIZE_MAX = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y};
	const int OFFSET = 2;

	for (int i = 1; i <= DRAW_CNT; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA_MAX - (ALPHA * i));

		// 線の太さ
		size = (LINE_SIZE * range);
		
		// 上
		DrawBox((i * size) - OFFSET, ((i - 1) * size),
				POS_SIZE_MAX.x - (i * size),
				(i * size),
				COLOR, true);
		// 下        
		DrawBox((i * size) - OFFSET, (POS_SIZE_MAX.y - (i * size)),
				POS_SIZE_MAX.x - (i * size),
				POS_SIZE_MAX.y - ((i + 1) * size),
				COLOR, true);
		// 左
		DrawBox(((i - 1) * size), (i * size) - OFFSET,
				(i * size),
				POS_SIZE_MAX.y - ((i - 1) * size) + OFFSET,
				COLOR, true);
		// 右
		DrawBox((POS_SIZE_MAX.x - (i * size)), (i * size) - OFFSET,
				POS_SIZE_MAX.x - ((i + 1) * size),
				POS_SIZE_MAX.y - ((i - 1) * size) - OFFSET,
				COLOR, true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


	unsigned int textCol = 0xffffff;

	// 白背景が一定割合を経過後、色を黒にする
	const float CHANGE_COLOR_TERM = 0.25f;
	const unsigned int CHANGE_COLOR = 0;
	textCol = ((range > CHANGE_COLOR_TERM)
				? CHANGE_COLOR : textCol);
	
	// 一定の時間経過語、色を赤にする
	const float TIME_ALERT = 10.0f;
	const unsigned int ALERT_COLOR = 0xff0000;
	textCol = ((curGameTime_ <= TIME_ALERT)
				? ALERT_COLOR : textCol);

	// カウンタ
	DrawFormatString(Application::SCREEN_HALF_X - 100,
					 0,
					 textCol, "世界の終末まで あと%d分%d秒",
					 static_cast<int>(curGameTime_ / 60.0f),
					 static_cast<int>(curGameTime_) % 60 + 1);
}
