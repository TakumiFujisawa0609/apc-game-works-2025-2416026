#pragma once
#include "../Common/Vector2.h"
#include <DxLib.h>
#include <map>
#include "../Object/Object.h"
class Player;
class Npc;


class CollisionManager
{
public:

	/// <summary>
	/// 当たり判定種類
	/// </summary>
	enum class COL_TYPE
	{
		NONE,
		BOX,
		SPHERE, // 球
		CAPCEL, // カプセル
		LAY,	// 線
	};


	/// <summary>
	/// 当たり判定判定対象
	/// </summary>
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
	static constexpr float THEESHOLD_WALL = 1.0f;

	static constexpr float THEESHOLD_DAMAGE = 1.0f;
	
	// 壁判定の反発力
	static constexpr float BOUNCE_WALL = 7.5f;

	// プレイヤー同士の横反発力
	static constexpr float BOUNCE_PLAYER_XZ = 0.1f;

	// プレイヤー同士の縦反発力
	static constexpr float BOUNCE_PLAYER_Y = 20.0f;



	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	CollisionManager(void);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~CollisionManager(void) = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="stageHandle">ステージモデルハンドル</param>
	/// <param name="stagePos">ステージの位置</param>
	/// <param name="stageScale">ステージのサイズ</param>
	/// <param name="damageHandle">ダメージ領域ハンドル</param>
	/// <param name="damagePos">ダメージ領域の位置</param>
	/// <param name="damageScale">ダメージ領域のサイズ</param>
	void Init(int& stageHandle, const VECTOR& stagePos, const VECTOR& stageScale,
			  int& damageHandle, const VECTOR& damagePos = {}, const VECTOR& damageScale = {});

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// デバッグ描画処理
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// メモリ解放処理
	/// </summary>
	void Release(void);

	/// <summary>
	/// 当たり判定を行うキャラクター割り当て処理
	/// </summary>
	/// <param name="target">当たり判定対象</param>
	/// <param name="chara">キャラクター</param>
	//void SetCharaCollision(COL_TARGET target, CharaBase& chara);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="stageHandle">ステージモデルハンドル</param>
	/// <param name="stagePos">ステージの位置</param>
	/// <param name="stageScale">ステージのサイズ</param>
	/// <param name="damageHandle">ダメージ領域ハンドル</param>
	/// <param name="damagePos">ダメージ領域の位置</param>
	/// <param name="damageScale">ダメージ領域のサイズ</param>
	void SetStageCollision(int& stageHandle, const VECTOR& stagePos, const VECTOR& stageScale,
						   int& damageHandle, const VECTOR& damagePos = {}, const VECTOR& damageScale = {});



	
	/// <summary>
	/// 球形同士の当たり判定
	/// </summary>
	/// <param name="pos1">対象１の座標</param>
	/// <param name="pos2">対象２の座標</param>
	/// <param name="radius1">対象１の半径</param>
	/// <param name="radius2">対象２の半径</param>
	bool IsHitLine(const VECTOR& pos1, const VECTOR& pos2, MV1_COLL_RESULT_POLY* result);

	

private:

	// キャラクター当たり判定対象リスト
	//std::map < int, CharaBase* > colChara_;


	// ステージの当たり判定用モデルハンドル
	int stageColHandle_;

	// ステージのダメージ領域の当たり判定用モデルハンドル
	int stageDamageHandle_;


	// ステージ当たり判定対象リスト
	//std::map < COL_TARGET, VECTOR* > collisionStage_;


	/// <summary>
	/// キャラクター同士の当たり判定
	/// </summary>
	void CollisionChara(void);


	/// <summary>
	/// 各キャラクターのステージ床の当たり判定
	/// </summary>
	void CollisionsGround(void);

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


	/// <summary>
	/// 各キャラクターのステージ壁の当たり判定
	/// </summary>
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


	/// <summary>
	/// ダメージ領域当たり判定処理
	/// </summary>
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

};