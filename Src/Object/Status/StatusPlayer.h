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
		POWER, // 攻撃力
		LUCK,  // 幸運
		SPEED, // 移動速度
		WEAPON_NUM,      // 武器番号
		TIME_INVINCIBLE, // 無敵時間
		TIME_PARRY,		 // パリィ時間
		TIME_EVASION,    // 回避時間

		MAX,
	};


	StatusPlayer(void);

	~StatusPlayer(void) = default;

	void LoadParam(const std::array<std::string, static_cast<int>(PARAM::MAX)>& _loadString)
	{
		startName_  = _loadString[static_cast<int>(PARAM::START_NAME)];
		handlePath_ = _loadString[static_cast<int>(PARAM::HANDLE_PATH)];

		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::POWER)], power_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::LUCK)], luck_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SPEED)], speed_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::WEAPON_NUM)], weaponId_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::TIME_INVINCIBLE)], timeInv_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::TIME_PARRY)], timeParry_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::TIME_EVASION)], timeEvasion_, 0.0f);
	}


	std::string& GetStartName(void) { return startName_; }

	std::string& GetHandlePath(void) { return handlePath_; }

	int GetPower(void) { return power_; }

	int GetLuck(void) { return luck_; }

	int GetWeaponId(void) { return weaponId_; }

	float GetSpeed(void) { return speed_; }

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


private:

	std::string startName_;

	std::string handlePath_;

	int power_;

	int luck_;

	int weaponId_;

	float speed_;

	float timeParry_;

	float timeEvasion_;

	// 回避時間
	float timeInv_;
};