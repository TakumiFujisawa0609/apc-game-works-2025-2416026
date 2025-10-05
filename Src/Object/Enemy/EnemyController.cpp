#include "EnemyController.h"
#include <DxLib.h>
#include "../Object.h"
#include "./Enemy.h"
#include "./EnemyWarrior.h"

EnemyController::EnemyController(void)
{
	
}

void EnemyController::Init(void)
{
	enemys_.clear();

	EnemySpawn(ENEMY_TYPE::SKELETON_WARRIOR, {0.0f, 0.0f, 50.0f});
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
		//if (!enemy->GetIsActive()) continue;

		enemy->Draw();

		DrawFormatString(0, 148, 0xff0000, "ePos:(%.1f,%.1f,%.1f)",
			enemy->GetPos().x, enemy->GetPos().y, enemy->GetPos().z);
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

	enemy = new EnemyWarrior();

	if (type == ENEMY_TYPE::SKELETON_WARRIOR)
	{
		//enemy = new EnemyWarrior();
	}

	// “G‚ð‰Šú‰»‚µ‚ÄƒŠƒXƒg‚ÉŠi”[
	enemy->Load();
	enemy->Init(posField);
	enemys_.push_back(enemy);
}
