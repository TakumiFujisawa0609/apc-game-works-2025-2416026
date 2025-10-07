#include "EnemyController.h"
#include <DxLib.h>
#include <vector>
#include "../Object.h"
#include "./Enemy.h"
#include "./EnemyWarrior.h"
#include "../Player.h"


EnemyController::EnemyController(Player& player)
	:player_(player)
{
	
}

void EnemyController::Init(void)
{
	enemys_.clear();

	const float rangeXZ = 1000.0f;
	
	for (int i = 0; i < 100; i++)
	{
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
	//if (enemys_.empty()) return;

	for (auto& enemy : enemys_)
	{
		if (!enemy->GetIsActive()) continue;

		enemy->Draw();

		DrawFormatString(0, 148, 0xff0000, "ePos:(%.1f,%.1f,%.1f)",
			enemy->GetPos().x, enemy->GetPos().y, enemy->GetPos().z);
	}
}

void EnemyController::DrawDebug(void)
{
	for (auto& enemy : enemys_)
	{
		if (!enemy->GetIsActive()) continue;
		enemy->DrawDebug();
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

	enemy = new EnemyWarrior(player_);

	if (type == ENEMY_TYPE::SKELETON_WARRIOR)
	{
		//enemy = new EnemyWarrior();
	}

	// “G‚ð‰Šú‰»‚µ‚ÄƒŠƒXƒg‚ÉŠi”[
	enemy->Load();
	enemy->Init(posField);
	enemys_.push_back(enemy);
}
