#pragma once
#include "../Status/StatusEnemy.h"
#include "./Enemy.h"
#include <map>
class Player;

class EnemySkeleton : public Enemy
{
public:

	enum class SKELETON_ANIM
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

	EnemySkeleton(Player& player);

	~EnemySkeleton(void) = default;


protected:

	/// @brief ÉtÉåÅ[ÉÄèâä˙âª
	void InitModelFrame(void)override;

	void InitAnim(void)override;

	void ChangeAnimState(ANIM_STATE _state, bool isLoop = true,
						 float _blendTime = AnimationController::DEFAULT_BLENDTIME)override;


private:

	void SetAnimSpeed(SKELETON_ANIM _type, STATUS_ANIM_TYPE _speedType);
};