#pragma once
#include "../Object.h"
#include "../Status/StatusEnemy.h"
#include <string>
class StatusEnemy;
class Player;

class Enemy : public Object
{
public:

	using STATUS_ANIM_TYPE = StatusEnemy::ANIM_TYPE;

	/// @brief アニメーションの種類
	enum class ANIM_STATE
	{
		NONE = 0,
		ATTACK, // 攻撃
		DEATH,  // 死亡
		DEATH_STOP,
		HIT_1, // 被ダメージ(弱)
		HIT_2, // 被ダメージ(強)
		IDLE, // 待機
		WALK, // 歩行
		SPAWN, // 生成

		MAX,
	};

	enum class ACTION_STATE
	{
		NONE = -1,
		SPAWN,    // 生成状態
		IDLE,	  // 待機状態
		SEARCH,   // プレイヤー追従
		MOVE,     // 移動状態
		KNOCK,    // 吹っ飛ばし状態
		DEAD,	  // 死亡状態
		ATTACK_START, 
		ATTACK_ACTIVE,
		ATTACK_END,

		MAX,
	};

	static constexpr float LOCAL_ANGLE_Y = (180.0f * (DX_PI_F / 180.0f));
	
	// 召喚時の無敵時間
	static constexpr float SPAWN_TIME = 1.0f;

	// 召喚時間の乱数
	static constexpr float SPAWN_TIME_RANGE = 0.5f;

	
	Enemy(StatusEnemy::TYPE type, Player& players, bool _isElite = false);

	/// @brief デフォルトデストラクタ
	virtual ~Enemy(void) = default;


	void Update(void)override;

	void DamageProc(void)override;

	/// @brief 行動状態取得
	ACTION_STATE GetActionState(void)const { return paramEnemy_.actionState; };
	
	ANIM_STATE GetAnimState(void)const { return paramEnemy_.animState; };

protected:

	// スケールの差分
	static constexpr float SCALE_DIFF = 0.25f;

	struct ParamEnemy
	{
		std::string name;

		ACTION_STATE actionState;

		ANIM_STATE animState;

		StatusEnemy::TYPE type;

		float timeAtkInterval;

		float atkRange;

		float searchRange;

		// 索敵有効フラグ
		bool isHearing;

		// 攻撃有効フラグ
		bool isAttack;

		// エリートか否か(ボスは除く)
		bool isElite;
	};

	ParamEnemy paramEnemy_;

	// アニメーション速度
	std::map<int, float> animSpeed_;

	StatusEnemy& status_;

	Player& player_;


	void SetParam(void) override;

	/// @brief 初回読み込み処理
	void LoadPost(void)override;

	/// @brief プレイヤーの方向を追尾
	void LookPlayerPos(void);

	virtual void InitPost(void)override;
	virtual void InitModelFrame(void)override = 0;

	virtual void InitAnim(void) = 0;

	void UpdateAnim(void)override;

	void UpdatePost(void)override;

	void DrawPost(void)override;


	void SearchField(void);

	/// @brief 索敵位置描画処理
	void DrawSearchRange(void);

	void SearchAttackField(void);

	/// @brief 索敵位置描画処理
	void DrawAttackRange(void);


	void UpdateState(void);
	virtual void UpdateStateSpawn(void);
	virtual void UpdateStateIdle(void);
	virtual void UpdateStateMove(void);
	virtual void UpdateStateKnock(void);
	virtual void UpdateStateAtk(void);

	void ChangeActionState(ACTION_STATE _state);

	/// @brief アニメーション遷移処理
	/// @param _state 状態
	/// @param _isLoop ループ判定
	virtual void ChangeAnimState(ANIM_STATE _state, bool _isLoop = true) = 0;

	// アニメーション状態遷移処理
	void AnimState(void);

	bool IsAttackState(void);

	bool IsUpdateFrame(void)override;

	void Move(void)override;

private:

	// 初期行動状態
	static constexpr ACTION_STATE ACTION_STATE_START = ACTION_STATE::SPAWN;
};