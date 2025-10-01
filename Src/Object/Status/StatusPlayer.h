#pragma once
#include <string>
#include <array>
#include "../../Utility/UtilityCommon.h"

class StatusPlayer
{
public:

	// プレイヤー保存データ
	enum class PARAM
	{
		START_NAME,  // 初期の名前
		HANDLE_PATH, // モデルのハンドルパス
		HP,    // HP
		POWER, // 攻撃力
		LUCK,  // 幸運
		SPEED, // 移動速度
		SPEED_ACC, // 移動時の加速度
		WEAPON_NUM,      // 武器番号
		TIME_INVINCIBLE, // 無敵時間
		TIME_PARRY,		 // パリィ時間
		TIME_EVASION,    // 回避時間
		COMBO_MAG, 	     // コンボ倍率

		ANIM_SPEED_IDLE, // 待機アニメーション速度
		ANIM_SPEED_WALK, // 移動アニメーション速度
		ANIM_SPEED_RUN, // ダッシュアニメーション速度

		MAX,
	};


	StatusPlayer(void);

	~StatusPlayer(void) = default;

	void LoadParam(const std::array<std::string, static_cast<int>(PARAM::MAX)>& _loadString)
	{
		startName_  = _loadString[static_cast<int>(PARAM::START_NAME)];
		handlePath_ = _loadString[static_cast<int>(PARAM::HANDLE_PATH)];

		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::HP)], hp_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::POWER)], power_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::LUCK)], luck_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SPEED)], speed_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SPEED_ACC)], speedAcc_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::WEAPON_NUM)], weaponId_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::TIME_INVINCIBLE)], timeInv_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::TIME_PARRY)], timeParry_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::TIME_EVASION)], timeEvasion_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::COMBO_MAG)], comboMag_, 0.0f);

		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_IDLE)], animSpeedIdle_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_WALK)], animSpeedWalk_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_RUN)], animSpeedRun_, 0.0f);
	}


	std::string& GetStartName(void) { return startName_; }

	std::string& GetHandlePath(void) { return handlePath_; }

	int GetHp(void) { return hp_; }

	int GetPower(void) { return power_; }

	int GetLuck(void) { return luck_; }

	int GetWeaponId(void) { return weaponId_; }

	float GetSpeed(void) { return speed_; }

	float GetSpeedAcc(void) { return speed_; }

	/// <summary>
	/// 無敵時間取得
	/// </summary>
	float GetTimeInvicible(void) { return timeInv_; }

	/// <summary>
	/// パリィ時間
	/// </summary>
	float GetTimeParry(void) { return timeParry_; }

	/// <summary>
	/// 回避時間
	/// </summary>
	float GetTimeEvasion(void) { return timeEvasion_; }

	/// <summary>
	/// コンボ倍率
	/// </summary>
	float GetComboMag(void) { return comboMag_; }

	// アニメーション速度
	float& GetAnimSpeedIdle(void) { return animSpeedIdle_; };

	float& GetAnimSpeedWalk(void) { return animSpeedWalk_; };

	float& GetAnimSpeedRun(void) { return animSpeedRun_; };
	

private:

	std::string startName_;

	std::string handlePath_;

	int hp_;

	int power_;

	int luck_;

	int weaponId_;

	float speed_;

	float speedAcc_;

	float timeParry_;

	float timeEvasion_;

	// 回避時間
	float timeInv_;

	// コンボ倍率
	float comboMag_;


	float animSpeedIdle_;

	float animSpeedWalk_;

	float animSpeedRun_;
};