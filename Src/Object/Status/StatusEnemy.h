#pragma once
#include <string>
#include <array>
#include <unordered_map>
#include "../../Utility/UtilityCommon.h"


class StatusEnemy
{
public:

	// 敵の種類
	enum class TYPE
	{
		NONE = 0,
		SKELETON_WARRIOR,
		SKELETON_MAGE,
		MAX,
	};

	// 敵保存データ
	enum class PARAM
	{
		NAME,            // 敵名
		HANDLE_PATH,	 // ハンドルパス
		ENEMY_TYPE,		 // 敵の種類
		SCALE,			 // モデルのスケール
		HP,              // HP
		POWER,           // 攻撃力
		SPEED,			 // 移動速度
		SPEED_ACC,       // 移動時の加速度
		ATTACK_INTERVAL, // 攻撃間隔
		ATTACK_RANEGE,   // 攻撃範囲
		SEARCH_RANGE,    // 索敵範囲
		TIME_INVINCIBLE, // 無敵時間

		ANIM_SPEED_IDLE, // 待機アニメーション速度
		ANIM_SPEED_ATTACK, // 攻撃アニメーション速度
		ANIM_SPEED_WALK, // 移動アニメーション速度
		ANIM_SPEED_SPAWN, // 生成時のアニメーション速度

		MAX,
	};


	StatusEnemy(void);

	~StatusEnemy(void) = default;


	void LoadParam(const std::array<std::string, static_cast<int>(PARAM::MAX)>& _loadString)
	{
		name_ = _loadString[static_cast<int>(PARAM::NAME)];
		handlePath_ = _loadString[static_cast<int>(PARAM::HANDLE_PATH)];
		SetType(_loadString[static_cast<int>(PARAM::ENEMY_TYPE)]);

		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SCALE)], scale_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::HP)], hp_, 1);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::POWER)], power_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SPEED)], speed_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SPEED_ACC)], speedAcc_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ATTACK_INTERVAL)], atkInterval_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ATTACK_RANEGE)], atkRange_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SEARCH_RANGE)], searchRange_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_IDLE)], animSpeed_.idle, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_ATTACK)], animSpeed_.attack, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_WALK)], animSpeed_.walk, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_SPAWN)], animSpeed_.spawn, 0.0f);
	}


	std::string& GetName(void) { return name_; }

	std::string& GetHandlePath(void) { return handlePath_; }

	TYPE& GetEnemyType(void) { return type_; };

	float GetScale(void) { return scale_; };

	int GetMaxHp(void) { return hp_; };

	int GetPower(void) { return power_; };

	float GetSpeed(void) { return speed_; };

	float GetSpeedAcc(void) { return speedAcc_; }

	float GetAtkInterval(void) { return atkInterval_; }

	float GetAtkRange(void) { return atkRange_; }

	float GetSearchRange(void) { return searchRange_; }

	float GetAnimSpeedIdle(void) { return animSpeed_.idle; }

	float GetAnimSpeedAtk(void) { return animSpeed_.attack; }

	float GetAnimSpeedWalk(void) { return animSpeed_.walk; }

	float GetAnimSpeedSpawn(void) { return animSpeed_.spawn; }

	

private:

	// 敵の種類名
	const std::unordered_map<TYPE, std::string> TYPE_STRING =
	{
		{TYPE::SKELETON_WARRIOR, "SKELETON_WARRIOR"},
		{TYPE::SKELETON_MAGE, "SKELETON_MAGE"}
	};


	std::string name_;

	std::string handlePath_;

	TYPE type_;

	float scale_;

	int hp_;

	int power_;

	float speed_;

	float speedAcc_;

	float atkInterval_;

	float atkRange_;

	float searchRange_;

	struct AnimSpeed
	{
		float idle;
		float attack;
		float walk;
		float spawn;
	};
	AnimSpeed animSpeed_;

	void SetType(const std::string& text);
};