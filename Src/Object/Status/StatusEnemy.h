#pragma once
#include <string>
#include <array>
#include "../../Utility/UtilityCommon.h"


class StatusEnemy
{
public:

	// 敵の種類
	enum class TYPE
	{
		SKELETON_WARROR,
		SKELETON_MAGE,
		MAX,
	};

	// 敵保存データ
	enum class PARAM
	{
		NAME,            // 敵名
		HANDLE_PATH,	 // ハンドルパス
		HP,              // HP
		POWER,           // 攻撃力
		SPEED,			 // 移動速度
		ATTACK_INTERVAL, // 攻撃間隔
		ATTACK_RANEGE,   // 攻撃範囲
		SEARCH_RANGE,    // 索敵範囲
		TIME_INVINCIBLE, // 無敵時間
		ANIM_SPEED_IDLE, // 待機アニメーション速度
		ANIM_SPEED_ATTACK, // 攻撃アニメーション速度

		MAX,
	};


	StatusEnemy(void);

	~StatusEnemy(void) = default;


	void LoadParam(const std::array<std::string, static_cast<int>(PARAM::MAX)>& _loadString)
	{
		name_ = _loadString[0];
		handlePath_ = _loadString[1];

		UtilityCommon::ChangeString(_loadString[2], hp_, 1);
		UtilityCommon::ChangeString(_loadString[3], power_, 0);
		UtilityCommon::ChangeString(_loadString[4], speed_, 0.0f);
		UtilityCommon::ChangeString(_loadString[5], atkInterval_, 0.0f);
		UtilityCommon::ChangeString(_loadString[6], atkRange_, 0.0f);
		UtilityCommon::ChangeString(_loadString[7], searchRange_, 0.0f);
		UtilityCommon::ChangeString(_loadString[8], animSpeedIdle_, 0.0f);
		UtilityCommon::ChangeString(_loadString[9], animSpeedAtk_, 0.0f);
	}


	std::string GetName(void) { return name_; }

	std::string GetHandlePath(void) { return handlePath_; }

	int GetMaxHp(void) { return hp_; }

	int GetPower(void) { return power_; }

	float GetSpeed(void) { return speed_; }

	float GetAtkInterval(void) { return atkInterval_; }

	float GetAtkRange(void) { return atkRange_; }

	float GetSearchRange(void) { return searchRange_; }

	float GetAnimSpeedIdle(void) { return animSpeedIdle_; }

	float GetAnimSpeedAtk(void) { return animSpeedAtk_; }


private:

	std::string name_;

	std::string handlePath_;

	int hp_;

	int power_;

	float speed_;

	float atkInterval_;

	float atkRange_;

	float searchRange_;

	float animSpeedIdle_;

	float animSpeedAtk_;
};