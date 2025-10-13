#include "EnemyController.h"
#include <DxLib.h>
#include <vector>
#include <cassert>
#include "../Object.h"
#include "./Enemy.h"
#include "./EnemyWarrior.h"
#include "../Player.h"
#include "../,./../../Utility/AsoUtility.h"

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

	const float rangeXZ = 750.0f;
	
	for (int i = 0; i < 30; i++)
	{
		//break;
		float x = static_cast<int>(GetRand(rangeXZ + rangeXZ) - rangeXZ);
		float z = static_cast<int>(GetRand(rangeXZ + rangeXZ) - rangeXZ);
		
		//EnemySpawn(ENEMY_TYPE::SKELETON_WARRIOR, { x, 0.0f, z });
	}

	EnemySpawn(ENEMY_TYPE::SKELETON_WARRIOR, { static_cast<float>(GetRand(rangeXZ)),
											   0.0f,
										       static_cast<float>(GetRand(rangeXZ)) });
}

void EnemyController::Update(void)
{
	if (enemys_.empty()) return;

	for (auto& enemy : enemys_)
	{
		if (!enemy->GetIsActive()) continue;

		enemy->Update();
	}
}

void EnemyController::Draw(void)
{
	if (enemys_.empty()) return;

	for (auto& enemy : enemys_)
	{
		if (!enemy->GetIsActive()) continue;

		enemy->Draw();
	}
}

void EnemyController::DrawDebug(void)
{
	for (auto& enemy : enemys_)
	{
		if (!enemy->GetIsActive()) continue;
		enemy->DrawDebug();
		DrawFormatString(0, 132, 0xff0000, "enemy: pos(%.2f,%.2f,%.2f), dir(%.2f,%.2f,%.2f), knock(%.1f, %.1f, %.1f), hp(%d), inv(%.2f)",
			enemy->GetPos().x,enemy->GetPos().y,enemy->GetPos().z,
			AsoUtility::Rad2Deg(enemy->GetDir().x),
			AsoUtility::Rad2Deg(enemy->GetDir().y), 
			AsoUtility::Rad2Deg(enemy->GetDir().z),
			AsoUtility::Rad2Deg(enemy->GetKnockVelo().x),
			AsoUtility::Rad2Deg(enemy->GetKnockVelo().y),
			AsoUtility::Rad2Deg(enemy->GetKnockVelo().z),
			enemy->GetCurHp(), enemy->GetTimeInv());
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
