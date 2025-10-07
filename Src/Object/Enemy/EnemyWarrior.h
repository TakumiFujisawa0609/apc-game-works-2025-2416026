#pragma once
#include "../Status/StatusEnemy.h"
#include "./Enemy.h"
#include <map>
class Player;

class EnemyWarrior : public Enemy
{
public:

	enum class ANIM_STATE
	{
		NONE = 0,
		ATTACK = 10,
		IDLE = 42,
		WALK = 56,
		SPAWN = 81,
		MAX,
	};

	EnemyWarrior(Player& player);

	~EnemyWarrior(void) = default;


protected:

	void SetParam(void)override;

	void InitAnim(void)override;

private:

	// アニメーション速度
	std::map<ANIM_STATE, float> animSpeed_;
	
	ANIM_STATE animState_;

	/// <summary>
	/// アニメーション速度割り当て
	/// </summary>
	/// <param name="_state">指定するアニメーション</param>
	/// <param name="_speed">アニメーション速度</param>
	void SetAnimSpeed(ANIM_STATE _state, float _speed)
		{ animSpeed_.emplace(_state, _speed); };
};