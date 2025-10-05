#pragma once
#include "../Object.h"
#include "../Status/StatusEnemy.h"
#include <string>
class StatusEnemy;

class Enemy : public Object
{
public:

	enum class ACTION_STATE
	{
		NONE = -1,
		INACTIVE, // 非表示状態
		SPAWN, 
		IDLE,
		ATTACK_START,
		ATTACK_ACTIVE,
		ATTACK_END,
		MAX,
	};

	static constexpr float LOCAL_ANGLE_Y = (180.0f * (DX_PI_F / 180.0f));

	// モデルの位置調整値
	static constexpr VECTOR MODEL_OFFSET = { 0.0f, 0.0f, 0.0f };

	
	Enemy(StatusEnemy::TYPE type);

	virtual ~Enemy(void) = default;

	void Load(void)override;

	void Init(const VECTOR& pos, float angleY = 0.0f, ACTION_STATE state = ACTION_STATE::IDLE);

	void Update(void)override;

	void Release(void)override;


	void SetParam(void) override;

	/// <summary>
	/// 行動状態取得
	/// </summary>
	ACTION_STATE GetActionState(void)const { return actionState_; };

	bool GetIsActive(void)const { return (actionState_ != ACTION_STATE::INACTIVE);  };


protected:

	struct ENEMY_PARAM
	{
		std::string name;

		StatusEnemy::TYPE type;

		float curAtkInterval;

		float atkRange;

		float searchRange;
	};

	ENEMY_PARAM paramEnemy_;

	ACTION_STATE actionState_;

	StatusEnemy& status_;


	void LoadResource(void);

	virtual void InitAnim(void) = 0;

	//StatusEnemy& data_;

};