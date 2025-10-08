#include "../Object.h"
#include "EnemyWarrior.h"
#include <map>
#include "../../Manager/ResourceManager.h"
#include "../Status/StatusData.h"
#include "../Status/StatusEnemy.h"
#include "../AnimationController.h"
#include "../Player.h"

EnemyWarrior::EnemyWarrior(Player& player)
		:Enemy(StatusEnemy::TYPE::SKELETON_WARRIOR, player)
{
	animSpeed_.clear();
}

void EnemyWarrior::SetParam(void)
{
	//paramChara_.frameNames[] = "Skeleton_Warrior_Eyes";
	Enemy::SetParam();
}

void EnemyWarrior::InitAnim(void)
{
	int type;
	float speed = 0.0f;

	// アニメーション初期化
	anim_ = new AnimationController(paramChara_.handle);

	animSpeed_.emplace(ANIM_STATE::IDLE, status_.GetAnimSpeedIdle());
	animSpeed_.emplace(ANIM_STATE::ATTACK, status_.GetAnimSpeedAtk());
	animSpeed_.emplace(ANIM_STATE::WALK, status_.GetAnimSpeedWalk());
	animSpeed_.emplace(ANIM_STATE::SPAWN, status_.GetAnimSpeedSpawn());
	

	for (auto& anim : animSpeed_)
	{
		type = static_cast<int>(anim.first);

		speed = animSpeed_[anim.first];

		// アニメーション割り当て
		anim_->AddInFbx(type, speed, type);
	}

	// 待機アニメーション再生
	anim_->Play(static_cast<int>(ANIM_STATE::IDLE));
}
