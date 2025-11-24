#include "EnemyBoss.h"
#include <unordered_map>
#include <vector>
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/EffectController.h"
#include "../Status/StatusData.h"
#include "../Status/StatusEnemy.h"
#include "../Common/AnimationController.h"
#include "../../Utility/AsoUtility.h"
#include "../Object.h"
#include "../Player/Player.h"

EnemyBoss::EnemyBoss(Player& player)
	:Enemy(StatusEnemy::TYPE::SKELETON_BOSS, player, true),
	isSpawnCircle_(true), circleRadius_(0.0f),
	circlePos_(AsoUtility::VECTOR_ZERO)
{

}


void EnemyBoss::InitModelFrame(void)
{
	Object::InitModelFrame();

	const std::string FRAME_NAME_BODY = "chest";
	const std::string FRAME_NAME_HEAD = "head";
	const std::string FRAME_NAME_BOTTOM = "Rig";
	const std::string FRAME_NAME_HAND_L = "hand.l";
	const std::string FRAME_NAME_HAND_R = "hand.r";

	// 当たり判定のフレーム割り当て
	int index = -1;
	index = FindFrameNum(FRAME_NAME_BODY);
	auto temp = &paramChara_.frames[index];
	paramChara_.colList.emplace(COLLISION_TYPE::BODY, &paramChara_.frames[index]);

	index = FindFrameNum(FRAME_NAME_HEAD);
	paramChara_.colList.emplace(COLLISION_TYPE::HEAD, &paramChara_.frames[index]);

	index = FindFrameNum(FRAME_NAME_BOTTOM);
	paramChara_.colList.emplace(COLLISION_TYPE::BOTTOM, &paramChara_.frames[index]);

	index = FindFrameNum(FRAME_NAME_HAND_L);
	paramChara_.colList.emplace(COLLISION_TYPE::HAND_L, &paramChara_.frames[index]);

	index = FindFrameNum(FRAME_NAME_HAND_R);
	paramChara_.colList.emplace(COLLISION_TYPE::HAND_R, &paramChara_.frames[index]);
}
void EnemyBoss::InitAnim(void)
{
	SetAnimSpeed(BOSS_ANIM::SPAWN, STATUS_ANIM_TYPE::SPAWN);
	SetAnimSpeed(BOSS_ANIM::IDLE, STATUS_ANIM_TYPE::IDLE);
	SetAnimSpeed(BOSS_ANIM::ATTACK, STATUS_ANIM_TYPE::ATTACK);
	SetAnimSpeed(BOSS_ANIM::WALK, STATUS_ANIM_TYPE::WALK);
	SetAnimSpeed(BOSS_ANIM::HIT_1, STATUS_ANIM_TYPE::HIT_1);
	SetAnimSpeed(BOSS_ANIM::HIT_2, STATUS_ANIM_TYPE::HIT_2);
	SetAnimSpeed(BOSS_ANIM::DEATH, STATUS_ANIM_TYPE::DEATH);

	for (auto& [type, speed] : animSpeed_)
	{
		speed = animSpeed_[type];

		if (type < static_cast<int>(BOSS_ANIM::MAX))
		{
			// 内部アニメーション割り当て
			anim_->AddInternal(static_cast<int>(type), speed);
		}
		else
		{
			// 外部部アニメーション割り当て
			//anim_->AddExternal(static_cast<int>(type), speed);
		}
	}

	anim_->Play(static_cast<int>(BOSS_ANIM::SPAWN), false);
}

void EnemyBoss::InitPost(void)
{
	ChangeActionState(ACTION_STATE::SPAWN);

	isSpawnCircle_ = true;

	const float SPAWN_TIME = 3.35f;
	spawnTime_ = SPAWN_TIME;

	InitSpawnCircle();
}
void EnemyBoss::InitSpawnCircle(void)
{
	/* 魔法陣初期化 */

	const VECTOR INIT_NORM = AsoUtility::DIR_UP;
	COLOR_U8 INIT_DIFUSECOLOR = SPAWN_CIRCLE_COLOR;

	circlePos_ = VAdd(SPAWN_POS, CIRCLE_POS_OFFSET);

	// 左前
	circleVertex_[0].pos = VAdd(circlePos_, { -SPAWN_SIZE_HALF, 0.0f, -SPAWN_SIZE_HALF });
	circleVertex_[0].norm = INIT_NORM;
	circleVertex_[0].dif = INIT_DIFUSECOLOR;
	circleVertex_[0].u = 1.0f;
	circleVertex_[0].v = 1.0f;
	circleVertex_[0].su = 1.0f;
	circleVertex_[0].sv = 1.0f;

	// 左奥
	circleVertex_[1].pos = VAdd(circlePos_, { -SPAWN_SIZE_HALF, 0.0f, SPAWN_SIZE_HALF });
	circleVertex_[1].norm = INIT_NORM;
	circleVertex_[1].dif = INIT_DIFUSECOLOR;
	circleVertex_[1].u = 1.0f;
	circleVertex_[1].v = 0.0f;
	circleVertex_[1].su = 1.0f;
	circleVertex_[1].sv = 0.0f;

	// 右前
	circleVertex_[2].pos = VAdd(circlePos_, { SPAWN_SIZE_HALF, 0.0f, -SPAWN_SIZE_HALF });
	circleVertex_[2].norm = INIT_NORM;
	circleVertex_[2].dif = INIT_DIFUSECOLOR;
	circleVertex_[2].u = 0.0f;
	circleVertex_[2].v = 1.0f;
	circleVertex_[2].su = 0.0f;
	circleVertex_[2].sv = 1.0f;

	// 右奥
	circleVertex_[3].pos = VAdd(circlePos_, { SPAWN_SIZE_HALF, 0.0f, SPAWN_SIZE_HALF });
	circleVertex_[3].norm = INIT_NORM;
	circleVertex_[3].dif = INIT_DIFUSECOLOR;
	circleVertex_[3].u = 0.0f;
	circleVertex_[3].v = 0.0f;
	circleVertex_[3].su = 0.0f;
	circleVertex_[3].sv = 0.0f;

	circleImage_ = ResourceManager::GetInstance().LoadHandleId(ResourceManager::IMG_SPAWNCIRCLE);

	const float rad = 350.0f;
	circleRadius_ = rad;
}


void EnemyBoss::UpdateStateSpawn(void)
{
	if (isSpawnCircle_)
	{
		anim_->Stop();
		return;
	}

	if (spawnTime_ > 0.0f)
	{
		spawnTime_ -= SceneManager::GetInstance().GetDeltaTime();

		if (spawnTime_ <= 0.0f)
		{
			// アニメーション再生
			anim_->Stop(false);
		}
		return;
	}

	Enemy::UpdateStateSpawn();
}

void EnemyBoss::DrawMagicCircle(void)
{
	WORD index[6];
	/*
	index[0] = static_cast<int>(TYPE::LEFTUP);
	index[1] = static_cast<int>(TYPE::LEFTDOWN);
	index[2] = static_cast<int>(TYPE::RIGHTUP);
	index[3] = static_cast<int>(TYPE::LEFTDOWN);
	index[4] = static_cast<int>(TYPE::RIGHTUP);
	index[5] = static_cast<int>(TYPE::RIGHTDOWN);
	index[0] = static_cast<int>(TYPE::LEFTUP);
	index[1] = static_cast<int>(TYPE::LEFTDOWN);
	index[2] = static_cast<int>(TYPE::RIGHTUP);*/
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;

	index[3] = 3;
	index[4] = 2;
	index[5] = 1;

	if (isSpawnCircle_)
	{
#ifdef _DEBUG
		DrawSphere3D(circlePos_, circleRadius_, 16, 0xffff00, 0xffffff, false);
#endif
	}

	// 魔法陣描画
	if (paramEnemy_.actionState == ACTION_STATE::SPAWN ||
		paramEnemy_.actionState == ACTION_STATE::NONE)
	{
		DrawPolygonIndexed3D(circleVertex_, 4, index, 2, circleImage_, true);
	}
	
}

void EnemyBoss::SetIsSpawnCircle(bool _flag)
{
	if (!_flag && isSpawnCircle_)
	{
		EffectController& effect = SceneManager::GetInstance().GetEffects();
		effect.SetEffect(EffectController::EFFECT_TYPE::SPAWN_SKELETON,
						 circlePos_, 2.0f, AsoUtility::VECTOR_ZERO,
						 VGet(75.0f, 75.0f, 75.0f));
		
	}

	isSpawnCircle_ = _flag;
}


void EnemyBoss::SetAnimSpeed(BOSS_ANIM _type, STATUS_ANIM_TYPE _speedType)
{
	int type = static_cast<int>(_type);
	float speed = status_.GetAnimSpeed(static_cast<int>(_speedType));
	animSpeed_.emplace(type, speed);
}


void EnemyBoss::ChangeAnimState(ANIM_STATE _state, bool _isLoop, float _blendTime)
{
	paramEnemy_.animState = _state;
	BOSS_ANIM animType = BOSS_ANIM::NONE;

	if (_state == ANIM_STATE::SPAWN) { animType = BOSS_ANIM::SPAWN; }
	else if (_state == ANIM_STATE::IDLE) { animType = BOSS_ANIM::IDLE; }
	else if (_state == ANIM_STATE::WALK) { animType = BOSS_ANIM::WALK; }
	else if (_state == ANIM_STATE::ATTACK) { animType = BOSS_ANIM::ATTACK; }
	else if (_state == ANIM_STATE::HIT_1) { animType = BOSS_ANIM::HIT_1; }
	else if (_state == ANIM_STATE::HIT_2) { animType = BOSS_ANIM::HIT_2; }
	else if (_state == ANIM_STATE::DEATH) { animType = BOSS_ANIM::DEATH; }

	anim_->Play(static_cast<int>(animType), _isLoop, _blendTime);
}