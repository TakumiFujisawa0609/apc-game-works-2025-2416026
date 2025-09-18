#pragma once
#include "../Object.h"
#include <string>
class StatusEnemy;

class Enemy : public Object
{
public:

	enum class TYPE
	{
		NONE,
		WARRIOR,
	};

	Enemy(TYPE type);

	~Enemy(void) = default;

	void Load(void)override;

	void Init(const VECTOR& pos, float angleY = 0.0f)override;

	void Update(void)override;

	void Draw(void)override;

	void SetParam(void) override;


private:

	struct ENEMY_PARAM
	{
		std::string name;

		int handle;

		float curAtkInterval;

		float atkRange;

		float searchRange;


		float animSpeedIdle;

		float curAnimSpeedIdle;

		float animSpeedAtk;

		float curAnimSpeedAtk;
	};
	ENEMY_PARAM paramEnemy_;

	StatusEnemy& status_;

	void LoadStatus(void);

	//StatusEnemy& data_;

};