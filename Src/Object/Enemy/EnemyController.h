#pragma once
#include "../Status/StatusEnemy.h"
#include "../../Utility/AsoUtility.h"
#include <map>
#include <vector>
#include <DxLib.h>
class Enemy;
class EnemyBoss;
class Player;
class Stage;
class SceneManager;

using ENEMY_TYPE = StatusEnemy::TYPE;

class EnemyController
{
public:

	using Enemys = std::vector<Enemy*>;
	using EnemyList = std::vector<Enemys>;
	using EnemyBossList = std::vector<EnemyBoss*>;


	/// @brief デフォルトコンストラクタ
	/// @param player
	EnemyController(Player& player, Stage& _stage);

	/// @brief デフォルトデストラクタ
	~EnemyController(void) = default;


	void Init(void);

	void Update(void);

	void Draw(void);

	void DrawDebug(void);

	void Release(void);


	const EnemyList& GetEnemyLists(void)const { return enemys_; };

	/// @brief 敵リスト取得
	/// @param _listNum リスト番号
	const Enemys& GetEnemys(int _listNum)const { return enemys_.at(_listNum); };

	Enemy& GetEnemy(int _listNum, int num) { return *GetEnemys(_listNum).at(num); };

	EnemyBossList& GetEnemyBossList(void) { return enemyBossList_; };
	
	bool GetIsActiveBoss(void);
	

private:

	// 生成方向
	static constexpr float SPAWN_ROT_Y = (0.0f * AsoUtility::DEG2RAD);

	// 円状生成の分割数
	static constexpr int SPAWN_CIRCLE_SPLIT = 8;

	// 生成位置中央の敵を除く、一度に生成される数
	static constexpr int SPAWN_MAX = (3 * SPAWN_CIRCLE_SPLIT) - 1;

	SceneManager& sceneMng_;

	Player& player_;
	Stage& stage_;
	

	EnemyList enemys_;
	EnemyBossList enemyBossList_;

	enum class SPAWN_TYPE
	{
		MOB,
		BOSS,
		MAX
	};
	std::map<SPAWN_TYPE ,VECTOR> spawnPos_;
	

	void EnemyBossSpawn(const VECTOR& _pos);

	void EnemysSpawn(ENEMY_TYPE _type, const VECTOR& _posField);

	/// @brief 敵生成処理
	/// @param type 敵の種類
	/// @param _pos 生成位置
	Enemy& EnemySpawn(ENEMY_TYPE type, const VECTOR& _pos);
};