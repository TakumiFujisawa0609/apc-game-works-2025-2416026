#pragma once
#include <string>
#include <array>
#include "../../Utility/UtilityCommon.h"

class StatusWeapon
{
public:

	// 武器の種類
	enum class TYPE
	{
		DEFAULT_SWORD,
		MAX
	};

	// 武器データ
	enum class PARAM
	{
		NAME,
		HANDLE_PATH,
		POWER,
		DIFENCE,
		SPEED,        // 移動速度上昇倍率
		DURABILITY,   // 耐久値
		ATTACK_RANGE, // 攻撃範囲
		PARRY_TIME,   // パリィ有効時間
		ANIM_SPEED_ATTACK, // 攻撃アニメーション速度

		MAX,
	};


	StatusWeapon(void);

	~StatusWeapon(void) = default;

	void LoadParam(const std::array<std::string, static_cast<int>(PARAM::MAX)>& _loadString)
	{
		name_ = _loadString[0];
		handlePath_ = _loadString[1];

		UtilityCommon::ChangeString(_loadString[2], power_, 0);
		UtilityCommon::ChangeString(_loadString[3], difence_, 0);
		UtilityCommon::ChangeString(_loadString[4], speed_, 1.0f);
		UtilityCommon::ChangeString(_loadString[5], durability_, 0);
		UtilityCommon::ChangeString(_loadString[6], attackRange_, 0.0f);
		UtilityCommon::ChangeString(_loadString[7], parryTime_, 0.0f);
		UtilityCommon::ChangeString(_loadString[8], animSpeedAtk_, 0.0f);
	};


	std::string GetName(void) { return name_; };

	std::string GetHandlePath(void) { return handlePath_; };

	int GetPower(void) { return power_; };

	int GetDifence(void) { return difence_; };

	float GetSpeed(void) { return speed_; };

	int GetDurability(void) { return durability_; };

	float GetAttackRange(void) { return attackRange_; };

	float GetParryTime(void) { return parryTime_; };

	float GetAnimSpeedAtk(void) { return animSpeedAtk_; };


private:

	std::string name_;

	std::string handlePath_;

	int power_;

	int difence_;

	float speed_;

	int durability_;

	float attackRange_;

	float parryTime_;

	float animSpeedAtk_;
};