#include "EnemyWarrior.h"
#include <map>
#include "../../Manager/ResourceManager.h"
#include "../Status/StatusData.h"
#include "../Status/StatusEnemy.h"
#include "../AnimationController.h"
#include "../Object.h"
#include "../Player.h"

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
	int index;
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
	int type;
	float speed = 0.0f;

	// アニメーション初期化
	anim_ = new AnimationController(paramChara_.handle);

	animSpeed_.emplace(WARRIER_ANIM::IDLE, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::IDLE));
	animSpeed_.emplace(WARRIER_ANIM::ATTACK, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::ATTACK));
	animSpeed_.emplace(WARRIER_ANIM::WALK, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::WALK));
	animSpeed_.emplace(WARRIER_ANIM::SPAWN, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::SPAWN));
	animSpeed_.emplace(WARRIER_ANIM::HIT_1, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::HIT_1));
	animSpeed_.emplace(WARRIER_ANIM::HIT_2, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::HIT_2));
	animSpeed_.emplace(WARRIER_ANIM::DEATH, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::DEATH));
	

	for (auto& anim : animSpeed_)
	{
		type = static_cast<int>(anim.first);

		speed = animSpeed_[anim.first];

		// アニメーション割り当て
		anim_->AddInternal(type, speed, type);
	}

	// 待機アニメーション再生
	ChangeAnimState(ANIM_STATE::IDLE);
}

void EnemyWarrior::DamagePerform(void)
{
	if (paramChara_.hp > 0)
	{
		ChangeAnimState(ANIM_STATE::HIT_2, false);
	}
	else
	{
		ChangeAnimState(ANIM_STATE::DEATH, false);
	}
}

void EnemyWarrior::AnimState(void)
{
	// 撃破アニメーション時、無効化
	if (paramEnemy_.animState == ANIM_STATE::DEATH)
	{
		if (anim_->IsEnd())
		{
			paramChara_.hp = 0;
			paramChara_.isActive = false;
		}
	}
	else
	{
		if (paramEnemy_.animState == ANIM_STATE::HIT_1 ||
			paramEnemy_.animState == ANIM_STATE::HIT_2)
		{
			if (anim_->IsEnd())
			{
				ChangeAnimState(ANIM_STATE::IDLE);
			}
			return;
		}

		if (paramEnemy_.actionState == ACTION_STATE::MOVE && paramChara_.timeInv <= 0.0f)
		{
			ChangeAnimState(ANIM_STATE::WALK);
		}
		else
		{
			ChangeAnimState(ANIM_STATE::IDLE);
		} 
	}
}

void EnemyWarrior::ChangeAnimState(ANIM_STATE _state, bool _isLoop)
{
	paramEnemy_.animState = _state;
	WARRIER_ANIM animType = WARRIER_ANIM::NONE;

	if (_state == ANIM_STATE::SPAWN) { animType = WARRIER_ANIM::SPAWN; }
	if (_state == ANIM_STATE::IDLE) { animType = WARRIER_ANIM::IDLE; }
	if (_state == ANIM_STATE::WALK) { animType = WARRIER_ANIM::WALK; }
	if (_state == ANIM_STATE::ATTACK) { animType = WARRIER_ANIM::ATTACK; }
	if (_state == ANIM_STATE::HIT_1) { animType = WARRIER_ANIM::HIT_1; }
	if (_state == ANIM_STATE::HIT_2) { animType = WARRIER_ANIM::HIT_2; }
	if (_state == ANIM_STATE::DEATH) { animType = WARRIER_ANIM::DEATH; }

	anim_->Play(static_cast<int>(animType), _isLoop);
}

