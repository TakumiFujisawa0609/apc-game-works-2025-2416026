#include "StatusEnemy.h"

StatusEnemy::StatusEnemy(void)
{
	name_ = "";

	handlePath_ = "";

	hp_ = 0;

	power_ = 0;

	speed_ = 0.0f;

	speedAcc_ = 0.0f;

	atkInterval_ = 0.0f;

	atkRange_ = 0.0f;

	searchRange_ = 0.0f;

	animSpeedIdle_ = 0.0f;

	animSpeedAtk_ = 0.0f;
}
