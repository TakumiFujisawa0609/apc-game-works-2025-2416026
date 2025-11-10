#include "EnemyWarrior.h"
#include <map>
#include "../../Manager/ResourceManager.h"
#include "../Status/StatusData.h"
#include "../Status/StatusEnemy.h"
#include "../Common/AnimationController.h"
#include "../Object.h"
#include "../Player/Player.h"

EnemyWarrior::EnemyWarrior(Player& player)
		:Enemy(StatusEnemy::TYPE::SKELETON_WARRIOR, player)
{

}

void EnemyWarrior::SetParam(void)
{
	Enemy::SetParam();
}

void EnemyWarrior::InitModelFrame(void)
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

void EnemyWarrior::InitAnim(void)
{
	SetAnimSpeed(WARRIER_ANIM::SPAWN, STATUS_ANIM_TYPE::SPAWN);
	SetAnimSpeed(WARRIER_ANIM::IDLE, STATUS_ANIM_TYPE::IDLE);
	SetAnimSpeed(WARRIER_ANIM::ATTACK, STATUS_ANIM_TYPE::ATTACK);
	SetAnimSpeed(WARRIER_ANIM::WALK, STATUS_ANIM_TYPE::WALK);
	SetAnimSpeed(WARRIER_ANIM::HIT_1, STATUS_ANIM_TYPE::HIT_1);
	SetAnimSpeed(WARRIER_ANIM::HIT_2, STATUS_ANIM_TYPE::HIT_2);
	SetAnimSpeed(WARRIER_ANIM::DEATH, STATUS_ANIM_TYPE::DEATH);
	
	for (auto& [type, speed] : animSpeed_)
	{
		speed = animSpeed_[type];

		// 内部アニメーション割り当て
		anim_->AddInternal(static_cast<int>(type), speed);
	}
}
void EnemyWarrior::SetAnimSpeed(WARRIER_ANIM _type, STATUS_ANIM_TYPE _speedType)
{
	int type = static_cast<int>(_type);
	float speed = status_.GetAnimSpeed(static_cast<int>(_speedType));
	animSpeed_.emplace(type, speed);
}


void EnemyWarrior::ChangeAnimState(ANIM_STATE _state, bool _isLoop)
{
	paramEnemy_.animState = _state;
	WARRIER_ANIM animType = WARRIER_ANIM::NONE;

	if (_state == ANIM_STATE::SPAWN) { animType = WARRIER_ANIM::SPAWN; }
	else if (_state == ANIM_STATE::IDLE) { animType = WARRIER_ANIM::IDLE; }
	else if (_state == ANIM_STATE::WALK) { animType = WARRIER_ANIM::WALK; }
	else if (_state == ANIM_STATE::ATTACK) { animType = WARRIER_ANIM::ATTACK; }
	else if (_state == ANIM_STATE::HIT_1) { animType = WARRIER_ANIM::HIT_1; }
	else if (_state == ANIM_STATE::HIT_2) { animType = WARRIER_ANIM::HIT_2; }
	else if (_state == ANIM_STATE::DEATH) { animType = WARRIER_ANIM::DEATH; }

	anim_->Play(static_cast<int>(animType), _isLoop);
}