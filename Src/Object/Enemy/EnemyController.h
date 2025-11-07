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

	using EnemyList = std::vector<Enemy*>;

	/// @brief デフォルトコンストラクタ
	/// @param player
	EnemyController(Player& player);

	/// @brief デフォルトデストラクタ
	~EnemyController(void) = default;


	void Init(void);

	void Update(void);

	void Draw(void);

	void DrawDebug(void);

	void Release(void);


	/// @brief 敵リスト取得
	const EnemyList& GetEnemys(void)const { return enemys_; };

private:

	Player* player_;

	EnemyList enemys_;
	
	void EnemySpawn(ENEMY_TYPE type, const VECTOR& posField);
};