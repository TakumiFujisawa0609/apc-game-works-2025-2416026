#include "Enemy.h"
#include "../Object.h"
#include "../Status/StatusData.h"
#include "../Status/StatusEnemy.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"

Enemy::Enemy(StatusEnemy::TYPE type)
	:status_(StatusData::GetInstance().GetEnemyStatus(static_cast<int>(type)))
{
	
}

void Enemy::Load(void)
{
	LoadResource();
}
void Enemy::LoadResource(void)
{
	int type = static_cast<int>(status_.GetEnemyType());
	ResourceManager::SRC src = static_cast<ResourceManager::SRC>(type + 1);

	paramChara_.handle = ResourceManager::GetInstance().LoadModelDuplicate(src);
}

void Enemy::SetParam(void)
{
	paramEnemy_.name = status_.GetName();

	paramEnemy_.type = status_.GetEnemyType();
	actionState_ = ACTION_STATE::IDLE;
	paramChara_.dir = {};

	paramChara_.quaRot = Quaternion::Identity();
	paramChara_.quaRotLocal = Quaternion::Identity(); // ÉçÅ[ÉJÉãâÒì]èâä˙âª
	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(LOCAL_ANGLE_Y, AsoUtility::AXIS_Y));

	VECTOR rotLocal = VAdd(AsoUtility::AXIS_X, AsoUtility::AXIS_Z);

	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(0.0f, rotLocal));

	float scale = status_.GetScale();
	paramChara_.scale = { scale, scale, scale };
	paramChara_.hp = status_.GetMaxHp();
	paramChara_.power = status_.GetPower();
	paramChara_.speed = status_.GetSpeed();
	paramChara_.speedAcc = status_.GetSpeedAcc();
}

void Enemy::Init(const VECTOR& pos, float angleY, Enemy::ACTION_STATE state)
{
	paramChara_.pos = paramChara_.prePos = pos;
	float rotY = AsoUtility::Deg2Rad(angleY);
	paramChara_.quaRot = Quaternion::Euler({ 0.0f, rotY, 0.0f });

	Object::Init();
	actionState_ = state;
}

void Enemy::Update(void)
{
	SetMatrixModel();
}

void Enemy::Release(void)
{
	
}