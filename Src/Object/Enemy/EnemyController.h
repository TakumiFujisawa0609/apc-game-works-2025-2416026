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

	static void CreateInstance(Player& player_);

	static EnemyController& GetInstance(void);


	void Init(void);

	void Update(void);

	void Draw(void);

	void DrawDebug(void);

	void Release(void);

	static void Destroy(void);


	/// <summary>
	/// 敵リスト取得
	/// </summary>
	const std::vector<Enemy*>& GetEnemys(void) { return enemys_; };

private:

	static EnemyController* instance_;

	Player* player_;

	std::vector<Enemy*> enemys_;


	/// @brief デフォルトコンストラクタ
	/// @param player
	EnemyController(Player& player);

	/// @brief デフォルトデストラクタ
	~EnemyController(void) = default;

	/// @brief コピーコンストラクタ対策
	EnemyController(EnemyController& other) = default;

	void EnemySpawn(ENEMY_TYPE type, const VECTOR& posField);
};