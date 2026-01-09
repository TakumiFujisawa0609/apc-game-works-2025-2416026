#include "EnemySkeleton.h"
#include <map>
#include "../../Manager/ResourceManager.h"
#include "../Status/StatusData.h"
#include "../Status/StatusEnemy.h"
#include "../Common/AnimationController.h"
#include "../Actor/Object.h"
#include "../Player/Player.h"

EnemySkeleton::EnemySkeleton(Player& player)
		:Enemy(StatusEnemy::TYPE::SKELETON, player, false)
{

}

void EnemySkeleton::InitModelFrame(void)
{
	Object::InitModelFrame();

	// 当たり判定のフレーム割り当て
	int index = -1;
	index = FindFrameNum("chest");
	paramChara_.colList.emplace(COLLISION_TYPE::BODY, &paramChara_.frames[index]);

	index = FindFrameNum("head");
	paramChara_.colList.emplace(COLLISION_TYPE::HEAD, &paramChara_.frames[index]);

	index = FindFrameNum("Rig");
	paramChara_.colList.emplace(COLLISION_TYPE::BOTTOM, &paramChara_.frames[index]);

	index = FindFrameNum("hand.l");
	paramChara_.colList.emplace(COLLISION_TYPE::HAND_L, &paramChara_.frames[index]);

	index = FindFrameNum("hand.r");
	paramChara_.colList.emplace(COLLISION_TYPE::HAND_R, &paramChara_.frames[index]);
}

void EnemySkeleton::InitAnim(void)
{
	SetAnimSpeed(SKELETON_ANIM::SPAWN, STATUS_ANIM_TYPE::SPAWN);
	SetAnimSpeed(SKELETON_ANIM::IDLE, STATUS_ANIM_TYPE::IDLE);
	SetAnimSpeed(SKELETON_ANIM::ATTACK, STATUS_ANIM_TYPE::ATTACK);
	SetAnimSpeed(SKELETON_ANIM::WALK, STATUS_ANIM_TYPE::WALK);
	SetAnimSpeed(SKELETON_ANIM::HIT_1, STATUS_ANIM_TYPE::HIT_1);
	SetAnimSpeed(SKELETON_ANIM::HIT_2, STATUS_ANIM_TYPE::HIT_2);
	SetAnimSpeed(SKELETON_ANIM::DEATH, STATUS_ANIM_TYPE::DEATH);
	
	for (auto& [type, speed] : animSpeed_)
	{
		speed = animSpeed_[type];

		// 内部アニメーション割り当て
		anim_->AddInternal(static_cast<int>(type), speed);
	}
}
void EnemySkeleton::SetAnimSpeed(SKELETON_ANIM _type, STATUS_ANIM_TYPE _speedType)
{
	int type = static_cast<int>(_type);
	float speed = status_.GetAnimSpeed(static_cast<int>(_speedType));
	animSpeed_.emplace(type, speed);
}


void EnemySkeleton::ChangeAnimState(ANIM_STATE _state, bool _isLoop, float _blendTime)
{
	paramEnemy_.animState = _state;
	SKELETON_ANIM animType = SKELETON_ANIM::NONE;

	if (_state == ANIM_STATE::SPAWN) { animType = SKELETON_ANIM::SPAWN; }
	else if (_state == ANIM_STATE::IDLE) { animType = SKELETON_ANIM::IDLE; }
	else if (_state == ANIM_STATE::WALK) { animType = SKELETON_ANIM::WALK; }
	else if (_state == ANIM_STATE::ATTACK) { animType = SKELETON_ANIM::ATTACK; }
	else if (_state == ANIM_STATE::HIT_1) { animType = SKELETON_ANIM::HIT_1; }
	else if (_state == ANIM_STATE::HIT_2) { animType = SKELETON_ANIM::HIT_2; }
	else if (_state == ANIM_STATE::DEATH) { animType = SKELETON_ANIM::DEATH; }

	anim_->Play(static_cast<int>(animType), _isLoop, _blendTime);
}