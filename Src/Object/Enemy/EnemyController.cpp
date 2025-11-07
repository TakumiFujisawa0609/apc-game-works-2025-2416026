#include "EnemyController.h"
#include <DxLib.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include "../Object.h"
#include "./Enemy.h"
#include "./EnemyWarrior.h"
#include "../Player/Player.h"
#include "../,./../../Utility/AsoUtility.h"
#include "../,./../../Application.h"
#include "../,./../../Manager/SceneManager.h"


EnemyController::EnemyController(Player& player)
{
	player_ = &player;
	enemys_.clear();
}

void EnemyController::Init(void)
{
	enemys_.clear();

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
	}
}

void EnemyController::Update(void)
{
	if (enemys_.empty()) return;

	int cnt = enemys_.size();
	for (auto& enemy : enemys_)
	{
		if (enemy->GetCurHp() <= 0 && enemy->GetIsAnimEnd())
		{
			if (enemy->GetAnimState() == Enemy::ANIM_STATE::DEATH ||
				enemy->GetAnimState() == Enemy::ANIM_STATE::HIT_2)
			{
				cnt--;
			}
		}

		if (!enemy->GetIsActive())continue;

		enemy->Update();
	}

	if (cnt <= 0)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}
}

void EnemyController::Draw(void)
{
	if (enemys_.empty()) return;

	int cnt = enemys_.size();
	for (auto& enemy : enemys_)
	{
		if (enemy->GetCurHp() <= 0 && enemy->GetIsAnimEnd())
		{
			if (enemy->GetAnimState() == Enemy::ANIM_STATE::DEATH ||
				enemy->GetAnimState() == Enemy::ANIM_STATE::HIT_2)
			{
				cnt--;
			}
		}

		if (!enemy->GetIsActive()) continue;
		enemy->Draw();
	}

	DrawFormatString(Application::SCREEN_HALF_X, 0, 0xffffff, "“G‚Ì”FŽc‚è%d‘Ì", cnt);
}

void EnemyController::DrawDebug(void)
{
	if (!SceneManager::GetInstance().GetIsDebugMode())return;
	int y = 136;
	for (auto& enemy : enemys_)
	{
		//if (!enemy->GetIsActive()) continue;
		if (enemy->GetAnimState() == Enemy::ANIM_STATE::DEATH) continue;

		enemy->DrawDebug();
		DrawFormatString(0, y, 0xff0000, "enemy: pos(%.2f,%.2f,%.2f), state(%d), hp(%d), inv(%.2f)\n",
			enemy->GetPos().x,enemy->GetPos().y,enemy->GetPos().z,
			enemy->GetActionState(),
			enemy->GetCurHp(), enemy->GetTimeInv());
		y += 16;
	}
}

void EnemyController::Release(void)
{
	if (!enemys_.empty())
	{
		for (auto& enemy : enemys_) { delete enemy; }

		enemys_.clear();
	}
}

void EnemyController::EnemySpawn(ENEMY_TYPE type, const VECTOR& posField)
{
	Enemy* enemy = nullptr;

	
	if (type == ENEMY_TYPE::SKELETON_WARRIOR)
	{
		enemy = new EnemyWarrior(*player_);
	}

	// “G‚ð‰Šú‰»‚µ‚ÄƒŠƒXƒg‚ÉŠi”[
	enemy->Load();
	enemy->Init(posField);
	enemys_.push_back(enemy);
}
