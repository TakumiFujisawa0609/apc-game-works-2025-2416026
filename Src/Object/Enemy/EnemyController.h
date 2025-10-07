#pragma once
#include "../Status/StatusEnemy.h"
#include <vector>
#include <DxLib.h>
class Enemy;
class Player;

using ENEMY_TYPE = StatusEnemy::TYPE;

class EnemyController
{
public:

	EnemyController(Player& player);

	~EnemyController(void) = default;

	void Init(void);

	void Update(void);

	void Draw(void);

	void DrawDebug(void);

	void Release(void);

	/// <summary>
	/// “GƒŠƒXƒgŽæ“¾
	/// </summary>
	const std::vector<Enemy*>& GetEnemys(void) { return enemys_; };

private:

	Player& player_;

	std::vector<Enemy*> enemys_;

	void EnemySpawn(ENEMY_TYPE type, const VECTOR& posField);
};