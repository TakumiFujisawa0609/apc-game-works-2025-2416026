#pragma once
#include <unordered_map>


class AttackMotion
{
public:

	// 行動状態
	enum class ATTACK_STATE
	{
		NONE = -1, // 攻撃していない
		ACTIVE,    // 攻撃有効
		END,       // モーション終了
		MAX
	};

	// ダメージ有効時間
	static constexpr float DAMAGE_ACTIVE_TIME = 0.05f;


	AttackMotion(void);

	~AttackMotion(void) = default;

	void Init(void);

	void Update(void);

	void Draw(void);


	// 行動状態取得
	ATTACK_STATE GetAttackState(void)const { return attackState_; }

	float GetTimeAttack(void) { return timeAttack_; };

	float GetTimeAction(void) { return curTimeAction_; };

	/// @brief 攻撃をしているか否か
	bool GetIsActionAttack(void)const;


	/// @brief 単体モーション割り当て処理
	/// @param _activeTime 有効時間
	/// @param _inputTime 入力猶予時間
	/// @param _endTime 終了時間
	/// @param _atkTime 攻撃時間
	/// @param _isConti 連続攻撃するか否か
	void SetMotion(float _activeTime, float _endTime, float _inputTime, float _atkTime, bool _isConti = false)
		{ _SetMotionTime(_activeTime, _endTime, _atkTime, _isConti, _inputTime); };

	/// @brief 単体モーション割り当て処理
	/// @param _activeTime 有効時間
	/// @param _endTime 終了時間
	/// @param _atkTime 攻撃時間
	/// @param _isConti 連続攻撃するか否か
	void SetMotion(float _activeTime, float _endTime, float _atkTime, bool _isConti)
		{ _SetMotionTime(_activeTime, _endTime, _atkTime, _isConti); };

	void SetAttackTime(float _time) { timeAttack_ = _time; };


	/// @brief モーション状態が遷移可能か否か
	bool GetIsChangeMotion(void) { return (timeChangeMotion_ <= 0.0f); };


private:

	// 行動状態
	ATTACK_STATE attackState_;

	// 行動時間
	float curTimeAction_;

	// 攻撃時間
	float timeAttack_;

	// 攻撃間隔時間
	float timeAtkMax_;

	// モーションが可能になる時間
	float timeChangeMotion_;

	// 各モーション時間のリスト
	float timeActions_[static_cast<int>(ATTACK_STATE::MAX)];

	// 連続攻撃フラグ
	bool isConti_;


	void UpdateActive(void);
	void UpdateEnd(void);

	void UpdateChangeMotion(void);

	void ChangeAction(void);

	void _SetMotionTime(float _activeTime, float _endTime, float _atkTime,
						bool _isConti = false, float _inputTime = 0.0f);
};