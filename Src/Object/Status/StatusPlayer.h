#pragma once
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
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
		RADIUS, // 半径
		HP,    // HP
		POWER, // 攻撃力
		LUCK,  // 幸運
		SPEED, // 移動速度
		SPEED_ACC, // 移動時の加速度
		DASH_MULT, // ダッシュ時の増加倍率
		WEAPON_NUM,      // 武器番号
		TIME_INVINCIBLE, // 無敵時間
		TIME_PARRY,		 // パリィ時間
		TIME_EVASION,    // 回避時間
		COMBO_MAG, 	     // コンボ倍率

		ANIM_SPEED_IDLE, // 待機アニメーション速度
		ANIM_SPEED_WALK, // 移動アニメーション速度
		ANIM_SPEED_DASH, // ダッシュアニメーション速度

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
		TIME_START,
		TIME_ACTIVE,
		TIME_END,
		SPEED,  // モーションアニメーション速度
		RADIUS, // 半径

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
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::RADIUS)], radius_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::POWER)], power_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::LUCK)], luck_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SPEED)], speed_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::SPEED_ACC)], speedAcc_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::DASH_MULT)], dashMult_, 1.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::WEAPON_NUM)], weaponId_, 0);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::TIME_INVINCIBLE)], timeInv_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::TIME_PARRY)], timeParry_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::TIME_EVASION)], timeEvasion_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::COMBO_MAG)], comboMag_, 0.0f);

		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_IDLE)], animSpeedIdle_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_WALK)], animSpeedWalk_, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(PARAM::ANIM_SPEED_DASH)], animSpeedDash_, 0.0f);
	}

	void LoadMortionParam(MORTION_TYPE _type, const std::array<std::string, static_cast<int>(MORTION_PARAM::MAX)>& _loadString)
	{
		Mortion mortion = Mortion();

		mortion.name = _loadString[static_cast<int>(MORTION_PARAM::NAME)];
		UtilityCommon::ChangeString(_loadString[static_cast<int>(MORTION_PARAM::TIME_START)], mortion.timeStart, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(MORTION_PARAM::TIME_ACTIVE)], mortion.timeActive, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(MORTION_PARAM::TIME_END)], mortion.timeEnd, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(MORTION_PARAM::SPEED)], mortion.animSpeed, 0.0f);
		UtilityCommon::ChangeString(_loadString[static_cast<int>(MORTION_PARAM::RADIUS)], mortion.radius, 0.0f);
		mortion_.emplace(_type, mortion);
	}


	std::string& GetStartName(void) { return startName_; }

	std::string& GetHandlePath(void) { return handlePath_; }

	float GetScale(void)const { return scale_; }

	float GetRadius(void)const { return radius_; }

	int GetHp(void)const { return hp_; }

	int GetPower(void)const { return power_; }

	int GetLuck(void)const { return luck_; }

	int GetWeaponId(void)const { return weaponId_; }

	float GetSpeed(void)const { return speed_; }

	float GetSpeedAcc(void)const { return speedAcc_; }

	float GetDashMult(void)const { return dashMult_; }

	/// <summary>
	/// 無敵時間取得
	/// </summary>
	float GetTimeInvicible(void)const { return timeInv_; }

	/// <summary>
	/// パリィ時間
	/// </summary>
	float GetTimeParry(void)const { return timeParry_; }

	/// <summary>
	/// 回避時間
	/// </summary>
	float GetTimeEvasion(void)const { return timeEvasion_; }

	/// <summary>
	/// コンボ倍率
	/// </summary>
	float GetComboMag(void)const { return comboMag_; }

	// アニメーション速度
	float GetAnimSpeedIdle(void)const { return animSpeedIdle_; }

	float GetAnimSpeedWalk(void)const { return animSpeedWalk_; }

	float GetAnimSpeedDash(void)const { return animSpeedDash_; }
	

	// モーション開始時間
	float GetMortionStart(MORTION_TYPE _type) { return mortion_[_type].timeStart; }

	// 有効時間
	float GetMortionActive(MORTION_TYPE _type) { return mortion_[_type].timeActive; }

	// モーション終了時間
	float GetMortionEnd(MORTION_TYPE _type) { return mortion_[_type].timeEnd; }

	// モーション終了時間
	float GetMortionSpeed(MORTION_TYPE _type) { return mortion_[_type].animSpeed; }

	// モーション半径取得
	float GetMortionRadius(MORTION_TYPE _type) { return mortion_[_type].radius; }


private:

	std::string startName_;

	std::string handlePath_;

	float scale_;

	float radius_;

	int hp_;

	int power_;

	int luck_;

	int weaponId_;

	float speed_;

	float speedAcc_;

	float dashMult_;

	float timeParry_;

	float timeEvasion_;

	// 回避時間
	float timeInv_;

	// コンボ倍率
	float comboMag_;


	float animSpeedIdle_;

	float animSpeedWalk_;

	float animSpeedDash_;

	struct Mortion
	{
		Mortion(void);

		std::string name;

		float timeStart;

		float timeActive;

		float timeEnd;

		float animSpeed;

		float radius;
	};

	std::unordered_map<MORTION_TYPE, Mortion> mortion_;
};