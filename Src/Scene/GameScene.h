#pragma once
#include "SceneBase.h"
#include "../Utility/Vector2.h"
#include <DxLib.h>

class Player;
class GameStageController;
class CollisionManager;
class EffectManager;
class HpBer;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public SceneBase
{
public:

	enum class GAME_STATE
	{
		NONE = -1,
		IDLE,
		GAME_OVER,
		RESULT,
	};


	// プレイヤー開始位置
	static constexpr VECTOR POS_START_PLAYER = { 0.0f, 0.0f, 0.0f };

	// プレイヤー開始時の向き
	static constexpr float ANGLE_START_PLAYER = 90.0f;


	
	// デフォルトコンストラクタ
	GameScene(void);

	// デフォルトデストラクタ
	~GameScene(void) = default;

	/// <summary>
	/// 初回読み込み処理
	/// </summary>
	void Load(void)override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void)override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void)override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void)override;


private:

	// ゲーム状態
	GAME_STATE gameState_;

	// プレイヤー
	Player* player_;


	//ステージオブジェクト
	GameStageController* gameStage_;

	// 当たり判定マネージャ
	CollisionManager* colManager_;


	/// <summary>
	/// 再初期化処理
	/// </summary>
	void ReInit(void);

	/// <sammary>
	/// 決定判定
	/// </sammary>
	bool IsCheck(void);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI(void);

	void GameIdleProc(void);

	void GameOverProc(void);
};