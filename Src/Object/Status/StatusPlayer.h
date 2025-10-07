#pragma once
#include <string>
#include <array>
#include <vector>
#include <map>
#include "../../Utility/UtilityCommon.h"

class StatusPlayer
{
public:

	// プレイヤー保存データ
	enum class PARAM
	{
		START_NAME,  // 初期の名前
		HANDLE_PATH, // モデルのハンドルパス
		SCALE, // モデルのスケール
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

	enum class MORTION_TYPE
	{
		NONE = -1,
		JUB_1,
		JUB_2,
		JUB_3,

		SPECIAL,
		STRONG_1,
		STRONG_2,
		STRONG_3,

		MAX,
	};
	enum class MORTION_PARAM
	{
		NAME = 0,
		START,
		ACTIVE,
		END,
		SPEED,

		MAX,
	};


	StatusPlayer(void);

	~StatusPlayer(void) = default;

	void LoadStatusParam(const std::array<std::string, static_cast<int>(PARAM::MAX)>& _loadString)
	{
		startName_  = _loadString[static_cast<int>(PARAM::START_NAME)];
		handlePath_ = _loadString[static_cast<int>(PARAM::HANDLE_PATH)];

		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SCALE)], scale_, 0.0f);
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

	void LoadMortionParam(MORTION_TYPE type, const std::array<std::string, static_cast<int>(MORTION_PARAM::MAX)>& _loadString)
	{
		Mortion mortion = Mortion();

		mortion.name = _loadString[static_cast<int>(MORTION_PARAM::NAME)];
		UtilityCommon::ChangeString(_loadString[static_cast<int>(MORTION_PARAM::START)], mortion.start, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(MORTION_PARAM::ACTIVE)], mortion.active, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(MORTION_PARAM::END)], mortion.end, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(MORTION_PARAM::SPEED)], mortion.animSpeed, 0.0f);
		mortionTimes_.emplace(type, mortion);
	}


	std::string& GetStartName(void) { return startName_; }

	std::string& GetHandlePath(void) { return handlePath_; }

	float GetScale(void) { return scale_; }

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
	

	// モーション開始時間
	float& GetMortionStart(MORTION_TYPE type) { return mortionTimes_[type].start; };

	// 有効時間
	float& GetMortionActive(MORTION_TYPE type) { return mortionTimes_[type].start; };

	// モーション終了時間
	float& GetMortionEnd(MORTION_TYPE type) { return mortionTimes_[type].start; };

	// モーション終了時間
	float& GetMortionSpeed(MORTION_TYPE type) { return mortionTimes_[type].animSpeed; };


private:

	std::string startName_;

	std::string handlePath_;

	float scale_;

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

	struct Mortion
	{
		Mortion(void);

		std::string name;
		float start;
		float active;
		float end;
		float animSpeed;
	};

	std::map<MORTION_TYPE, Mortion> mortionTimes_;
};