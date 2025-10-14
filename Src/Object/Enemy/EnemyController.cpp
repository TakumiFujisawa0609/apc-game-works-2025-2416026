#include "EnemyController.h"
#include <DxLib.h>
#include <vector>
#include <cassert>
#include "../Object.h"
#include "./Enemy.h"
#include "./EnemyWarrior.h"
#include "../Player.h"
#include "../,./../../Utility/AsoUtility.h"
#include "../,./../../Application.h"
#include "../,./../../Manager/SceneManager.h"

EnemyController* EnemyController::instance_ = nullptr;

void EnemyController::CreateInstance(Player& _player)
{
	if (instance_ == nullptr)
	{
		instance_ = new EnemyController(_player);
	}

	instance_->player_ = &_player;

	instance_->Init();
}

EnemyController& EnemyController::GetInstance(void)
{
	if (instance_ != nullptr)
	{
		return *instance_;
	}
	assert("\n敵管理クラスが取得できませんでした。\n");
}


EnemyController::EnemyController(Player& player)
{
	player_ = &player;
	enemys_.clear();
}

void EnemyController::Init(void)
{
	enemys_.clear();

	const int rangeXZ = 250;
	
	for (int i = 0; i < 30; i++)
	{
		float x = static_cast<float>(GetRand(rangeXZ + rangeXZ) - rangeXZ);
		float z = static_cast<float>(GetRand(rangeXZ));
		
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

	DrawFormatString(Application::SCREEN_HALF_X, 0, 0xffffff, "敵の数：残り%d体", cnt);
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
		DrawFormatString(0, y, 0xff0000, "enemy: pos(%.2f,%.2f,%.2f), velo(%.2f,%.2f,%.2f), knock(%.1f, %.1f, %.1f), hp(%d), inv(%.2f)\n",
			enemy->GetPos().x,enemy->GetPos().y,enemy->GetPos().z,
			AsoUtility::Rad2Deg(enemy->GetVelocity().x),
			AsoUtility::Rad2Deg(enemy->GetVelocity().y),
			AsoUtility::Rad2Deg(enemy->GetVelocity().z),
			AsoUtility::Rad2Deg(enemy->GetKnockVelo().x),
			AsoUtility::Rad2Deg(enemy->GetKnockVelo().y),
			AsoUtility::Rad2Deg(enemy->GetKnockVelo().z),
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
void EnemyController::Destroy(void)
{
	if (instance_ != nullptr)
	{
		// インスタンス解放
		instance_->Release();

		delete instance_;
		
		instance_ = nullptr;
	}
}

void EnemyController::EnemySpawn(ENEMY_TYPE type, const VECTOR& posField)
{
	Enemy* enemy = nullptr;

	enemy = new EnemyWarrior(*player_);

	if (type == ENEMY_TYPE::SKELETON_WARRIOR)
	{
		//enemy = new EnemyWarrior(*player_);
	}

	// 敵を初期化してリストに格納
	enemy->Load();
	enemy->Init(posField);
	enemys_.push_back(enemy);
}
