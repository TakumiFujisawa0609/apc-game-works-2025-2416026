#include "StatusPlayer.h"

StatusPlayer::StatusPlayer(void)
{
	startName_ = "";

	handlePath_ = "";

	scale_ = 0.0f;

	power_ = 0;

	luck_ = 0;

	weaponId_ = 0;

	speed_ = 0.0f;

	speedAcc_ = 0.0f;

	timeParry_ = 0.0f;

	timeEvasion_ = 0.0f;

	timeInv_ = 0.0f;

	comboMag_ = 0.0f;


	animSpeedIdle_ = 0.0f;

	animSpeedWalk_ = 0.0f;

	animSpeedRun_ = 0.0f;

	mortionTimes_.clear();
}

StatusPlayer::Mortion::Mortion(void)
{
	name = "";

	start = 0.0f;

	active = 0.0f;

	end = 0.0f;

	animSpeed = 0.0f;
}
