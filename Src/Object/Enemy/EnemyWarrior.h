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
		SPAWN = 81,
		MAX,
	};

	EnemyWarrior(Player& player);

	~EnemyWarrior(void) = default;


protected:

	void SetParam(void)override;

	/// @brief フレーム初期化
	void InitModelFrame(void)override;

	void InitAnim(void)override;

	void DamagePerform(void)override;

	void AnimState(void)override;


private:

	// アニメーション速度
	std::map<WARRIER_ANIM, float> animSpeed_;
	
	WARRIER_ANIM animState_;
};