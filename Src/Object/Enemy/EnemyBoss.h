#pragma once
#include "../Status/StatusEnemy.h"
#include "./Enemy.h"
class Player;

class EnemyBoss : public Enemy
{
public:

	enum class BOSS_ANIM
	{
		NONE = 0,
		ATTACK = 10,
		DEATH = 25,
		DEATH_STOP = 26,
		HIT_1 = 39,
		HIT_2 = 40,
		IDLE = 42,
		WALK = 56,
		SPAWN = 73,
		MAX,
	};

	static constexpr VECTOR SPAWN_POS = { 0.0f, 0.0f, 500.0f };

	EnemyBoss(Player& player);

	~EnemyBoss(void) = default;

	void DrawMagicCircle(void);

protected:

	/// @brief ÉtÉåÅ[ÉÄèâä˙âª
	void InitModelFrame(void)override final;

	void InitAnim(void)override final;

	void InitPost(void)override final;

	void ChangeAnimState(ANIM_STATE _state, bool isLoop = true,
						 float _blendTime = AnimationController::DEFAULT_BLENDTIME)override;


private:

	void SetAnimSpeed(BOSS_ANIM _type, STATUS_ANIM_TYPE _speedType);

	static constexpr int SPAWN_SIZE_HALF = (1024 / 2);

	static constexpr COLOR_U8 SPAWN_CIRCLE_COLOR = {0,255,0,255};

	static constexpr VECTOR CIRCLE_POS_OFFSET = { 0.0f, 0.0f,  1000.0f };

	VERTEX3D circleVertex_[4];
	int circleImage_;

};