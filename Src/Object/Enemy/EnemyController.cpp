#include "EnemyController.h"
#include <DxLib.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include "../Status/StatusData.h"
#include "../Object.h"
#include "./Enemy.h"
#include "./EnemySkeleton.h"
#include "./EnemyBoss.h"
#include "../Player/Player.h"
#include "../../Common/Camera.h"
#include "../,./../../Utility/AsoUtility.h"
#include "../,./../../Application.h"
#include "../,./../../Manager/SceneManager.h"
#include "../,./../../Manager/EffectController.h"


EnemyController::EnemyController(Player& player) :
	player_(&player),
	enemyBoss_(nullptr)
{
	enemys_.clear();
}

void EnemyController::Init(void)
{
	enemys_.clear();

	EnemysSpawn(ENEMY_TYPE::SKELETON, { 0.0f, 0.0f, 250.0f });

	EnemyBossSpawn();
}

void EnemyController::Update(void)
{
	if (enemys_.empty()) return;

	bool isView = false;
	for (auto& enemyList : enemys_)
	{
		// 敵リストが空の時、スキップ
		if (enemyList.empty()) { continue; }

		for (auto& enemy : enemyList)
		{
			isView = (SceneManager::GetInstance().GetCamera().GetIsCameraClip(enemy->GetFramePos(Object::COLLISION_TYPE::BOTTOM)) &&
					  SceneManager::GetInstance().GetCamera().GetIsCameraClip(enemy->GetFramePos(Object::COLLISION_TYPE::HEAD)));
			enemy->SetIsView(isView);

			if (!isView) { continue; }

			enemy->Update();
		}
	}


	if (enemyBoss_ != nullptr)
	{
		enemyBoss_->Update();
	}
}

void EnemyController::Draw(void)
{
	if (enemyBoss_ != nullptr)
	{
		enemyBoss_->DrawMagicCircle();
		enemyBoss_->Draw();
		DrawFormatString(Application::SCREEN_HALF_X, 0, 0xffffff, "ボスのHP：%d", enemyBoss_->GetCurHp());
	}
	int temp = 0;

	if (!enemys_.empty())
	{
		for (auto& enemyList : enemys_)
		{
			// 敵リストが空の時、スキップ
			if (enemyList.empty()) { continue; }

			for (auto& enemy : enemyList)
			{
				// 敵がカメラ外の時、スキップ
				if (!enemy->GetIsView()) { continue; }

				enemy->Draw();
			}
		}
	}
}

void EnemyController::DrawDebug(void)
{
#ifdef _DEBUG
	
	if (!SceneManager::GetInstance().GetIsDebugMode()) { return; }

	int y = 136;

	for (auto& enemyList : enemys_)
	{
		for (auto& enemy : enemyList)
		{
			//if (!enemy->GetIsActive()) continue;
			if (enemy->GetAnimState() == Enemy::ANIM_STATE::DEATH) { continue; }

			enemy->DrawDebug();

			DrawFormatString(0, y, 0xff0000, "enemy: pos(%.2f,%.2f,%.2f), atkParam(%d), state(%d), anim(%d), hp(%d), inv(%.2f)\n",
				enemy->GetPos().x, enemy->GetPos().y, enemy->GetPos().z,
				enemy->GetActionMotion().GetAttackState(),
				enemy->GetActionState(), enemy->GetAnimState(),
				enemy->GetCurHp(), enemy->GetTimeInv());

			y += 16;
		}
	}
#endif
}

void EnemyController::Release(void)
{
	if (enemyBoss_ != nullptr)
	{
		enemyBoss_->Release();
		delete enemyBoss_;
	}

	if (!enemys_.empty())
	{

		// 敵解放処理
		for (auto& enemyList : enemys_)
		{
			for (auto& enemy : enemyList)
			{
				enemy->Release();
				delete enemy;
			}
		}
		enemys_.clear();
	}
}

bool EnemyController::GetIsDefeatBoss(void)
{
	// ボスが存在しないとき、false
	if (!GetIsActiveBoss()) { return false; }

	return (enemyBoss_->GetCurHp() <= 0);
}

void EnemyController::EnemyBossSpawn(void)
{
	if (enemyBoss_) { return; }

	enemyBoss_ = new EnemyBoss(*player_);

	// 敵を初期化して返す
	enemyBoss_->Load();
	enemyBoss_->Init(EnemyBoss::SPAWN_POS, SPAWN_ROT_Y);
}

void EnemyController::EnemysSpawn(ENEMY_TYPE _type, const VECTOR& _posField)
{
	// 円の分割数
	const float ANGLE = (360.0f / SPAWN_CIRCLE_SPLIT);

	// 半径
	const int TYPE = static_cast<int>(_type);
	const float RANGE = StatusData::GetInstance().GetEnemyStatus(TYPE).GetRadius();

	float angle = 0;
	int cnt = 0, listCnt = -1;
	Quaternion rot = Quaternion::Identity();

	VECTOR spawnPos = _posField;


	// 中央の敵を生成
	Enemys list;
	list.push_back(&EnemySpawn(_type, spawnPos));

	// エフェクト生成
	/*EffectController& effect = SceneManager::GetInstance().GetEffects();
	effect.SetEffect(EffectController::EFFECT_TYPE::SPAWN_SKELETON,
					 spawnPos, { 10.0f, 10.0f, 10.0f });*/
	/*
	const int rangeXZ = 250.0f;
	float x, z = 0.0f;
	for (int i = 0; i < 2; i++)
	{
		int randX = GetRand(rangeXZ + rangeXZ) - rangeXZ;
		x = static_cast<float>(randX);
		z = static_cast<float>(GetRand(rangeXZ));

		x = std::clamp(x, static_cast<float>(-rangeXZ), static_cast<float>(rangeXZ));
		z = std::clamp(z, 0.0f, static_cast<float>(rangeXZ));
		EnemySpawn(ENEMY_TYPE::SKELETON_WARRIOR, { x, 0.0f, z });
	}*/	

	const int MAX = (SPAWN_MAX / SPAWN_CIRCLE_SPLIT) + 1;
	int spawnMax;

	for (int i = 1; i < MAX; i++)
	{
		rot = Quaternion::Identity();

		// 生成最大数
		spawnMax = (SPAWN_CIRCLE_SPLIT * i);
		for (int circle = 0; circle < spawnMax; circle++)
		{
			// 円状に一定の範囲間隔で生成位置を設定
			angle = ((ANGLE / i) * (circle + 1));
			rot = rot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2Rad(angle), AsoUtility::AXIS_Y));
			spawnPos = VAdd(_posField, VScale(rot.GetForward(), RANGE * i));

			// 円状に敵生成処理
			Enemy& enemy = EnemySpawn(_type, spawnPos);
			list.push_back(&enemy);

			// 生成数が一定を超えたら終了
			cnt++;
			if (cnt > SPAWN_MAX) { break; }
		}
	}

	// リスト格納
	enemys_.push_back(list);
}

Enemy& EnemyController::EnemySpawn(ENEMY_TYPE type, const VECTOR& _pos)
{
	Enemy* enemy = nullptr;
	
	if (type == ENEMY_TYPE::SKELETON)
	{
		enemy = new EnemySkeleton(*player_);
	}
	else if (type == ENEMY_TYPE::SKELETON_WARRIOR)
	{

	}

	else { return *enemy; }

	// 敵を初期化して返す
	enemy->Load();
	enemy->Init(_pos, SPAWN_ROT_Y);
	return *enemy;
}
