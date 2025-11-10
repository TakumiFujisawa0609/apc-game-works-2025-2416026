#include "EnemyController.h"
#include <DxLib.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include "../Status/StatusData.h"
#include "../Object.h"
#include "./Enemy.h"
#include "./EnemyWarrior.h"
#include "../Player/Player.h"
#include "../,./../../Utility/AsoUtility.h"
#include "../,./../../Application.h"
#include "../,./../../Manager/SceneManager.h"
#include "../,./../../Manager/EffectController.h"


EnemyController::EnemyController(Player& player) :
	enemyCnt_(0),listCnt_(0),
	player_(&player)
{
	enemys_.clear();
}

void EnemyController::Init(void)
{
	enemys_.clear();

	EnemysSpawn(ENEMY_TYPE::SKELETON_WARRIOR, { 0.0f, 0.0f, 250.0f });
}

void EnemyController::Update(void)
{
	if (enemys_.empty()) return;

	int cnt = enemyCnt_;
	
	for (auto& [num, enemyList] : enemys_)
	{
		// 敵リストが空の時、スキップ
		if (enemyList.empty()) { continue; }

		for (auto& enemy : enemyList)
		{
			if (enemy->GetCurHp() <= 0 && enemy->GetIsAnimEnd())
			{
				if (enemy->GetAnimState() == Enemy::ANIM_STATE::DEATH ||
					enemy->GetAnimState() == Enemy::ANIM_STATE::HIT_2)
				{
					cnt--;
				}
			}

			// 敵無効時、スキップ
			if (!enemy->GetIsActive()) { continue; }

			enemy->Update();
		}
	}

	if (cnt <= 0)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}
}

void EnemyController::Draw(void)
{
	if (enemys_.empty()) return;

	// 敵の数
	int cnt = 0;


	for (auto& [num, enemylist] : enemys_)
	{
		// 敵リストが空の時、スキップ
		if (enemylist.empty()) { continue; }

		for (auto& enemy : enemylist)
		{
			// 敵無効時、処理終了
			if (!enemy->GetIsActive()) { continue; }

			// 敵の数増加
			cnt++;

			enemy->Draw();
		}
	}

	DrawFormatString(Application::SCREEN_HALF_X, 0, 0xffffff, "敵の数：残り%d体", cnt);
}

void EnemyController::DrawDebug(void)
{
#ifdef _DEBUG
	if (!SceneManager::GetInstance().GetIsDebugMode()) { return; }

	int y = 136;
	for (auto& [num, enemylist] : enemys_)
	{
		for (auto& enemy : enemylist)
		{
			//if (!enemy->GetIsActive()) continue;
			if (enemy->GetAnimState() == Enemy::ANIM_STATE::DEATH) { continue; }

			enemy->DrawDebug();

			DrawFormatString(0, y, 0xff0000, "enemy: pos(%.2f,%.2f,%.2f), state(%d), anim(%d), hp(%d), inv(%.2f)\n",
				enemy->GetPos().x, enemy->GetPos().y, enemy->GetPos().z,
				enemy->GetActionState(), enemy->GetAnimState(),
				enemy->GetCurHp(), enemy->GetTimeInv());

			y += 16;
		}
	}
#endif
}

void EnemyController::Release(void)
{
	if (enemys_.empty()) { return; }

	// 敵解放処理
	for (auto& [num, enemylist] : enemys_)
	{
		for (auto& enemy : enemylist)
		{
			enemy->Release();
			delete enemy;
		}
	}
	enemys_.clear();
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
	EnemyList list;
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

		// リスト数増加
		listCnt++;

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
	// リスト数取得
	listCnt_ = listCnt;

	// リスト格納
	enemys_.emplace(listCnt_, list);
}

Enemy& EnemyController::EnemySpawn(ENEMY_TYPE type, const VECTOR& _pos)
{
	Enemy* enemy = nullptr;
	
	if (type == ENEMY_TYPE::SKELETON_WARRIOR)
	{
		enemy = new EnemyWarrior(*player_);
	}
	if (type == ENEMY_TYPE::SKELETON_MAGE)
	{
		
	}

	// 敵の数
	enemyCnt_++;

	// 敵を初期化して返す
	enemy->Load();
	enemy->Init(_pos, SPAWN_ROT_Y);
	return *enemy;
}
