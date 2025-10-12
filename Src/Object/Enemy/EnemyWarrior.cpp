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
	animSpeed_.emplace(WARRIER_ANIM::SPAWN, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::SPAWN));
	animSpeed_.emplace(WARRIER_ANIM::HIT_1, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::HIT_1));
	animSpeed_.emplace(WARRIER_ANIM::HIT_2, status_.GetAnimSpeed(StatusEnemy::ANIM_TYPE::HIT_2));
	animSpeed_.emplace(WARRIER_ANIM::DEATH, 30);
	

	for (auto& anim : animSpeed_)
	{
		type = static_cast<int>(anim.first);

		speed = animSpeed_[anim.first];

		// アニメーション割り当て
		anim_->AddInFbx(type, speed, type);
	}

	// 待機アニメーション再生
	animState_ = WARRIER_ANIM::IDLE;
	anim_->Play(static_cast<int>(WARRIER_ANIM::IDLE));
}

void EnemyWarrior::DamagePerform(void)
{
	if (paramChara_.hp > 0)
	{
		animState_ = WARRIER_ANIM::HIT_1;
		anim_->Play(static_cast<int>(WARRIER_ANIM::HIT_1), false);
	}
	else
	{
		animState_ = WARRIER_ANIM::DEATH;
		anim_->Play(static_cast<int>(WARRIER_ANIM::DEATH), false);
	}
}

void EnemyWarrior::AnimState(void)
{

	if (animState_ == WARRIER_ANIM::HIT_1 && anim_->IsEnd())
	{
		animState_ = WARRIER_ANIM::IDLE;
		anim_->Play(static_cast<int>(WARRIER_ANIM::IDLE));
	}
}

