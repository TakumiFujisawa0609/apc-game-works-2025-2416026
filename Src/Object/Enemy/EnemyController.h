#pragma once
#include "../Status/StatusEnemy.h"
#include <vector>
#include <DxLib.h>
class Enemy;

using ENEMY_TYPE = StatusEnemy::TYPE;

class EnemyController
{
public:

	EnemyController(void);

	~EnemyController(void) = default;

	void Init(void);

	void Update(void);

	void Draw(void);

	void Release(void);


private:

	std::vector<Enemy*> enemys_;

	void EnemySpawn(ENEMY_TYPE type, const VECTOR& posField);
};