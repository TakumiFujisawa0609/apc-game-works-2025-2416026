#pragma once
#include "../Status/StatusEnemy.h"
#include "./Enemy.h"
#include <map>
class Player;

class EnemyWarrior : public Enemy
{
public:

	enum class WARRIER_ANIM
	{
		NONE = 0,
		ATTACK = 10,
		DEATH = 25,
		DEATH_STOP = 26,
		HIT_1 = 39,
		HIT_2 = 40,
		IDLE = 42,
		WALK = 56,
		SPAWN = 74,
		MAX,
	};

	EnemyWarrior(Player& player);

	~EnemyWarrior(void) = default;


protected:

	void SetParam(void)override;

	/// @brief ÉtÉåÅ[ÉÄèâä˙âª
	void InitModelFrame(void)override;

	void InitAnim(void)override;

	void ChangeAnimState(ANIM_STATE _state, bool isLoop = true)override;


private:

	void SetAnimSpeed(WARRIER_ANIM _type, STATUS_ANIM_TYPE _speedType);
};