#pragma once
#include "../Status/StatusEnemy.h"
#include "../../Utility/AsoUtility.h"
#include <map>
#include <vector>
#include <DxLib.h>
class Enemy;
class Player;

using ENEMY_TYPE = StatusEnemy::TYPE;

class EnemyController
{
	using EnemyList = std::vector<Enemy*>;
	using EnemysMap = std::map<int, EnemyList>;

public:

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


	const EnemysMap& GetEnemyLists(void)const { return enemys_; };

	/// @brief 敵リスト取得
	/// @param _listNum リスト番号
	const EnemyList& GetEnemys(int _listNum)const { return enemys_.at(_listNum); };

	Enemy& GetEnemy(int _listNum, int num) { return *GetEnemys(_listNum).at(num); }


	int GetListCnt(void) { return listCnt_; };


private:

	// 生成方向
	static constexpr float SPAWN_ROT_Y = (180.0f * AsoUtility::DEG2RAD);

	// 円状生成の分割数
	static constexpr int SPAWN_CIRCLE_SPLIT = 8;
	// 生成位置中央の敵を除く、一度に生成される数
	static constexpr int SPAWN_MAX = (3 * SPAWN_CIRCLE_SPLIT) - 1;

	// 敵の数
	int enemyCnt_;
	int listCnt_;

	Player* player_;

	EnemysMap enemys_;
	
	void EnemysSpawn(ENEMY_TYPE _type, const VECTOR& _posField);

	/// @brief 敵生成処理
	/// @param type 敵の種類
	/// @param _pos 生成位置
	Enemy& EnemySpawn(ENEMY_TYPE type, const VECTOR& _pos);
};