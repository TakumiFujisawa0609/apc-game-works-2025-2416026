#pragma once
#include "../Common/Vector2.h"
#include <DxLib.h>
#include <map>
#include "../Object/Actor/Object.h"
class Player;
class Enemy;
class EnemyController;
class Stage;

class CollisionManager
{

	using COLLISION_TYPE = Object::COLLISION_TYPE;

public:

	/// @brief 当たり判定種類
	enum class COL_TYPE
	{
		NONE,
		BOX,
		SPHERE, // 球
		CAPCEL, // カプセル
		LAY,	// 線
	};


	/// @brief 当たり判定判定対象
	enum class COL_TARGET
	{
		PLAYER_1 = 0,
		PLAYER_2,
		NPC_1,
		NPC_2,

		MAX,
	};


	// 地面判定位置のしきい値(低いと貫通し、高すぎると常に着地する)
	static constexpr float THEESHOLD_GROUND = 7.5f;
	
	// 壁判定位置のしきい値
	static constexpr float THEESHOLD_WALL = 100.0f;

	static constexpr float THEESHOLD_DAMAGE = 1.0f;
	
	// 壁判定の反発力
	static constexpr float BOUNCE_WALL = 7.5f;


	/// @brief コンストラクタ
	/// @param プレイヤー
	/// @param 敵
	/// @param ステージ
	CollisionManager(Player& _player, EnemyController& _enemy, Stage& _stage);

	/// @briefデフォルトデストラクタ
	~CollisionManager(void) = default;


	/// @brief 初期化処理
	/// @param stageHandle ステージモデルハンドル
	/// @param stagePos ステージの位置
	/// @param stageScale ステージのサイズ
	/// @param damageHandle ダメージ領域ハンドル
	/// @param damagePos ダメージ領域の位置
	/// @param damageScale ダメージ領域のサイズ
	void Init(int& stageHandle, const VECTOR& stagePos, const VECTOR& stageScale,
			  int& damageHandle, const VECTOR& damagePos = {}, const VECTOR& damageScale = {});

	/// @brief 更新処理
	void Update(void);

	/// @brief デバッグ描画処理
	void DrawDebug(void);

	void Release(void);


	/// <summary>
	/// 当たり判定を行うキャラクター割り当て処理
	/// </summary>
	/// <param name="target">当たり判定対象</param>
	/// <param name="chara">キャラクター</param>
	//void SetCharaCollision(COL_TARGET target, CharaBase& chara);

	
	/// <summary>
	/// 球形同士の当たり判定
	/// </summary>
	/// <param name="pos1">対象１の座標</param>
	/// <param name="pos2">対象２の座標</param>
	/// <param name="radius1">対象１の半径</param>
	/// <param name="radius2">対象２の半径</param>
	bool IsHitLine(const VECTOR& pos1, const VECTOR& pos2, MV1_COLL_RESULT_POLY* result);

	

private:

	Player* player_;

	Stage& stage_;


	// 敵のマネージャ
	EnemyController* enemys_;


	// ステージの当たり判定用モデルハンドル
	int stageColHandle_;


	// ステージ当たり判定対象リスト
	//std::map < COL_TARGET, VECTOR* > collisionStage_;



	/// @brief キャラクター同士の当たり判定
	void CollisionChara(void);

	/// @brief 敵とプレイヤーの当たり判定 
	void CollisionPlayerToEnemy(void);

	/// @brief ボス敵とプレイヤーの当たり判定 
	void CollisionPlayerToBoss(void);

	/// @brief 敵と敵の当たり判定
	void CollisionEnemyToEnemy(void);

	/// @brief キャラクターとステージの当たり判定
	void CollisionGround(void);

	/// @brief 各キャラクターのステージ床の当たり判定
	//void CollisionsGround(void);

	/// <summary>
	/// 床の当たり判定(カプセル)
	/// </summary>
	/// <param name="top">上の位置</param>
	/// <param name="bottom">下の位置</param>
	/// <param name="result">ポリゴン</param>
	/// <param name="radius">当たり判定の半径</param>
	bool CheckGroundCollision(VECTOR& top, VECTOR& bottom, MV1_COLL_RESULT_POLY_DIM* result, float radius = 1.0f);
	/// <summary>
	/// 床の当たり判定(ポリゴン)
	/// </summary>
	/// <param name="top">上の位置</param>
	/// <param name="bottom">下の位置</param>
	/// <param name="result">ポリゴン</param>
	/// <param name="offset">当たり判定のオフセット</param>
	bool CheckGroundCollision(const VECTOR& top, const VECTOR& bottom, MV1_COLL_RESULT_POLY* result, float offset);


	/// @brief 各キャラクターのステージ壁の当たり判定
	void CollisionsWall(void);

	/// <summary>
	/// 壁の当たり判定(ポリゴン)
	/// </summary>
	/// <param name="type">判定対象の当たり判定種類</param>
	/// <param name="startPos">判定位置１</param>
	/// <param name="endPos">判定位置２</param>
	/// <param name="offset">調整値</param>
	bool CheckWallCollision(const VECTOR& startPos, const VECTOR& endPos, MV1_COLL_RESULT_POLY* result, float offset);
	/// <summary>
	/// 壁の当たり判定(POLY_DIM)
	/// </summary>
	/// <param name="type">判定対象の当たり判定種類</param>
	/// <param name="startPos">判定位置１</param>
	/// <param name="endPos">判定位置２</param>
	/// <param name="offset">調整値</param>
	bool CheckWallCollision(COL_TYPE type, const VECTOR& startPos, const VECTOR& endPos, 
						MV1_COLL_RESULT_POLY_DIM* result, float radius = 1.0f);


	/// @brief ダメージ領域当たり判定処理
	void CollisionsStageDamage(void);

	/// <summary>
	/// ステージの壁当たり判定処理(ポリゴン)
	/// </summary>
	/// <param name="chara">キャラクター</param>
	/// <param name="topPos">上の位置</param>
	/// <param name="bottomPos">下の位置</param>
	/// <param name="result">衝突位置</param>
	/// <param name="offset">当たり判定のオフセット</param>
	//bool CheckStageDamageCollision(CharaBase* chara, const VECTOR& topPos, const VECTOR& bottomPos,
	//							   MV1_COLL_RESULT_POLY_DIM* result, float offset);

	/// @brief 
	/// @param _enemy 
	/// @param _damage 
	void EnemyDamageProc(Enemy& _enemy, int _damage = 1);

};