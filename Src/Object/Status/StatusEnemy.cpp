#include "StatusEnemy.h"

StatusEnemy::StatusEnemy(void)
{
	name_ = "";

	handlePath_ = "";

	type_ = TYPE::NONE;

	scale_ = 0.0f;

	hp_ = 0;

	power_ = 0;

	speed_ = 0.0f;

	speedAcc_ = 0.0f;

	atkInterval_ = 0.0f;

	atkRange_ = 0.0f;

	searchRange_ = 0.0f;

	animSpeed_.idle = 0.0f;

	animSpeed_.attack = 0.0f;

	animSpeed_.walk = 0.0f;

	animSpeed_.spawn = 0.0f;
}

void StatusEnemy::SetType(const std::string& text)
{
	for (auto& type : TYPE_STRING)
	{
		if (type.second == text)
		{
			// テキストと状態が一致時に割り当て
			type_ = type.first;
		}
	}
}
