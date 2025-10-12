#include "Enemy.h"
#include <DxLib.h>
#include "../Object.h"
#include "../Player.h"
#include "../AnimationController.h"
#include "../Status/StatusData.h"
#include "../Status/StatusEnemy.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/AsoUtility.h"
#include "../../Common/Quaternion.h"

Enemy::Enemy(StatusEnemy::TYPE type, Player& player)
	:status_(StatusData::GetInstance().GetEnemyStatus(static_cast<int>(type))),
	 player_(player)
{
	
}

void Enemy::Load(void)
{
	LoadResource();
}
void Enemy::LoadResource(void)
{
	int type = static_cast<int>(status_.GetEnemyType());
	ResourceManager::SRC src = static_cast<ResourceManager::SRC>(type);

	paramChara_.handle = ResourceManager::GetInstance().LoadModelDuplicate(src);
}

void Enemy::Init(const VECTOR& pos, float angleY, Enemy::ACTION_STATE state)
{
	paramChara_.pos = paramChara_.prePos = pos;

	float rotY = AsoUtility::Deg2Rad(angleY);
	paramChara_.quaRot = Quaternion::Euler({ 0.0f, rotY, 0.0f });

	Object::Init();

	paramEnemy_.actionState = state;

	paramEnemy_.isHearing = false;

	paramChara_.isActive = true;
}
void Enemy::SetParam(void)
{
	paramEnemy_.name = status_.GetName();

	int frameNum = FindFrameNum(paramChara_.colList[Object::COLLISION_TYPE::BODY]->name);// フレーム名割り当て
	VECTOR sub = VSub(paramChara_.frames[frameNum].pos, paramChara_.pos);
	paramChara_.posLocal = VSub(paramChara_.pos, sub);

	paramEnemy_.type = status_.GetEnemyType();
	paramEnemy_.actionState = ACTION_STATE::IDLE;
	paramEnemy_.searchRange = status_.GetSearchRange();

	paramChara_.quaRot = Quaternion::Identity();
	paramChara_.quaRotLocal = Quaternion::Identity(); // ローカル回転初期化
	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(LOCAL_ANGLE_Y, AsoUtility::AXIS_Y));

	VECTOR rotLocal = VAdd(AsoUtility::AXIS_X, AsoUtility::AXIS_Z);

	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(0.0f, rotLocal));

	float scale = status_.GetScale();
	paramChara_.scale = { scale, scale, scale };
	paramChara_.radius = status_.GetRadius();

	paramChara_.hp = status_.GetMaxHp();
	paramChara_.power = status_.GetPower();
	paramChara_.speed = status_.GetSpeed();
	paramChara_.speedAcc = status_.GetSpeedAcc();
}

void Enemy::SetDamage(int _damage)
{
	Object::SetDamage(_damage);

	// ダメージ演出
	DamagePerform();
}

void Enemy::Update(void)
{
	if (!paramChara_.isActive) return;

	Object::Update();

	// 索敵範囲有効
	SearchField();

	// 状態更新
	UpdateState();
	
	// アニメーション更新
	UpdateAnim();

	// 行列更新
	SetMatrixModel();
}
void Enemy::UpdateState(void)
{
	if (paramEnemy_.actionState == ACTION_STATE::IDLE) { UpdateStateIdle(); }
	if (paramEnemy_.actionState == ACTION_STATE::SEARCH) { UpdateStateSearch(); }
	if (paramEnemy_.actionState == ACTION_STATE::MOVE) { UpdateStateMove(); }
	//if (paramEnemy_.actionState == ACTION_STATE::IDLE) { UpdateStateIdle(); }
}

void Enemy::UpdateStateIdle(void)
{
	if (paramEnemy_.isHearing)
	{
		ChangeActionState(ACTION_STATE::SEARCH);
	}

	if (!AsoUtility::EqualsVZero(paramChara_.velocity))
	{
		UpdateModelFrames();
	}
}

void Enemy::UpdateStateSpawn(void)
{
}

void Enemy::UpdateStateSearch(void)
{
	if (!paramEnemy_.isHearing)
	{
		ChangeActionState(ACTION_STATE::IDLE);
	}

	// プレイヤー追従
	LookPlayerPos();
}

void Enemy::UpdateStateMove(void)
{
	// プレイヤー追従
	LookPlayerPos();

	if (!AsoUtility::EqualsVZero(paramChara_.velocity))
	{
		UpdateModelFrames();
	}
}

void Enemy::UpdateStateAtk(void)
{
}

void Enemy::ChangeActionState(ACTION_STATE state)
{
	paramEnemy_.actionState = state;
}


void Enemy::Draw(void)
{
	if (!paramChara_.isActive) return;

	SceneManager& scene = SceneManager::GetInstance();

	if (scene.GetIsDebugMode())
	{
		DrawSearchRange();
	}

	Object::Draw();
}
void Enemy::Release(void)
{
	Object::Release();
}


void Enemy::LookPlayerPos(void)
{
	VECTOR pPos = player_.GetPos();

	// プレイヤーと敵の距離
	VECTOR diff = VSub(paramChara_.pos, pPos);

	// プレイヤーの方向を追尾
	paramChara_.dir = VNorm(diff);
	float rotY = atan2f(diff.x, diff.z);
	Quaternion pDir = Quaternion::Euler({ 0.0f, rotY, 0.0f });

	paramChara_.quaRot = Quaternion::Mult(paramChara_.quaRotLocal, pDir);
}

void Enemy::UpdateAnim(void)
{
	// アニメーション更新
	anim_->Update();

	AnimState();
}

void Enemy::SearchField(void)
{
	VECTOR pos = VAdd(paramChara_.pos, { paramChara_.posLocal.x, 0.0f,paramChara_.posLocal.z });
	VECTOR vec = VSub(player_.GetPos(), pos);

	float diff = ((vec.x * vec.x) + (vec.z * vec.z));

	// 視野左右内に入っている
	paramEnemy_.isHearing = (diff <= (paramEnemy_.searchRange * paramEnemy_.searchRange));
}
void Enemy::DrawSearchRange(void)
{
	/* 索敵範囲描画 */
	unsigned int color = ((paramEnemy_.isHearing) ? 0xffff00 : 0x00aaaa);
	MATRIX mat = Quaternion::ToMatrix(paramChara_.quaRot);

	int split = 36;
	float angle = (360.0f / split);

	// 前方方向
	VECTOR forward = paramChara_.quaRot.GetForward();

	MATRIX leftMat, rightMat = MGetIdent();
	VECTOR left, right = {};

	VECTOR leftPos, rightPos = {};

	VECTOR pos = VAdd(paramChara_.pos, { paramChara_.posLocal.x, 0.0f,paramChara_.posLocal.z });

	// 360°の索敵範囲描画
	for (int i = 0; i < split; i++)
	{
		leftMat = MMult(mat, MGetRotY(AsoUtility::Deg2Rad(angle * i)));
		left = VTransform(AsoUtility::DIR_F, leftMat);

		rightMat = MMult(mat, MGetRotY(AsoUtility::Deg2Rad(angle * (i + 1))));
		right = VTransform(AsoUtility::DIR_F, rightMat);

		leftPos = VAdd(pos, VScale(left, paramEnemy_.searchRange));
		rightPos = VAdd(pos, VScale(right, paramEnemy_.searchRange));

		DrawTriangle3D(pos, leftPos, rightPos, color, true);
	}
}