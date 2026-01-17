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

	EnemyBoss(Player& player, float _circleRad);

	~EnemyBoss(void) = default;


	void DrawMagicCircle(void);

	void SetIsSpawnCircle(bool _flag);

	float GetSpawnCircleRadius(void) { return circleRadius_; };

	const VECTOR& GetSpawnCirclePos(void) { return circlePos_; };

	bool GetIsSpawnCircle(void) { return isSpawnCircle_; };


protected:

	/// @brief フレーム初期化
	void InitModelFrame(void)override final;

	void InitAnim(void)override final;

	void InitPost(void)override final;

	void UpdateStateSpawn(void)override final;

	void ChangeAnimState(ANIM_STATE _state, bool isLoop = true,
						 float _blendTime = AnimationController::DEFAULT_BLENDTIME)override;


private:

	static constexpr int SPAWN_SIZE_HALF = (1024 / 2);

	// 魔法陣色
	static constexpr COLOR_U8 SPAWN_CIRCLE_COLOR = {255, 0, 0, 255};

	// 無効魔法陣色
	static constexpr COLOR_U8 DESABLE_CIRCLE_COLOR = {100, 0, 0, 255};

	static constexpr float CIRCLE_POS_OFFSET_Y = 10.0f;


	float spawnTime_;

	VERTEX3D circleVertex_[4];
	int circleImage_;
	bool isSpawnCircle_;
	VECTOR circlePos_;
	float circleRadius_;

	void InitSpawnCircle(void);

	void SetAnimSpeed(BOSS_ANIM _type, STATUS_ANIM_TYPE _speedType);
};