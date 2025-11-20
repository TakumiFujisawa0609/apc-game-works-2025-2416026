#pragma once
#include "./SceneBase.h"
#include "../Utility/UtilityCommon.h"
#include <DxLib.h>

class Player;
class GameStageController;
class EnemyController;
class CollisionManager;
class EffectController;
class HpBer;


class GameScene : public SceneBase
{

	using PAD_BTN = InputManager::PAD_BTN;

public:

	enum class GAME_STATE
	{
		NONE = -1,
		IDLE,
		GAME_OVER,
		RESULT,
	};


	// プレイヤー開始位置
	static constexpr VECTOR POS_START_PLAYER = { 0.0f, 0.0f, -250.0f };

	// プレイヤー開始時の向き
	static constexpr float ANGLE_START_PLAYER = 90.0f;


	
	/// @brief デフォルトコンストラクタ
	GameScene(void);

	/// @brief デフォルトデストラクタ
	~GameScene(void) = default;

	/// @brief 初回読み込み処理
	void Load(void)override;

	/// @brief 初期化処理
	void Init(void)override;

	/// @brief 更新処理
	void Update(void)override;

	/// @brief 描画処理
	void Draw(void)override;

	/// @brief 解放処理
	void Release(void)override;

	const EnemyController& GetEnemyController(void) { return *enemys_; };



private:

	static constexpr float FOG_START = 2000.0f;
	static constexpr float FOG_END = (2500.0f + FOG_START);
	static constexpr UtilityCommon::Color FOG_COLOR = {85, 85, 85, 1};

	// ゲーム状態
	GAME_STATE gameState_;

	// プレイヤー
	Player* player_;

	//ステージオブジェクト
	GameStageController* gameStage_;

	EnemyController* enemys_;


	/// @brief 再初期化処理
	void ReInit(void);


	/// @brief UI描画
	void DrawUI(void);

	/// @brief ゲーム有効処理
	void GameIdleProc(void);

	/// @brief ゲームオーバー処理
	void GameOverProc(void) {};
};