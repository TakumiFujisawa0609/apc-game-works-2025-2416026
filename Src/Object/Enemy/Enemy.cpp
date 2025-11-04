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

	Object::Load();
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
	paramEnemy_.atkRange = status_.GetAtkRange();

	paramChara_.quaRot = Quaternion::Identity();
	paramChara_.quaRotLocal = Quaternion::Identity(); // ローカル回転初期化
	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(LOCAL_ANGLE_Y, AsoUtility::AXIS_Y));

	VECTOR rotLocal = VAdd(AsoUtility::AXIS_X, AsoUtility::AXIS_Z);

	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(0.0f, rotLocal));

	float scale = status_.GetScale();
	paramChara_.scale = { scale * (1.0f - SCALE_DIFF),
						  scale * (1.0f + SCALE_DIFF),
						  scale * (1.0f - SCALE_DIFF) };
	paramChara_.radius = status_.GetRadius();

	paramChara_.hp = status_.GetMaxHp();
	paramChara_.power = status_.GetPower();
	paramChara_.speed = status_.GetSpeed();
	paramChara_.speedAcc = status_.GetSpeedAcc();
}

void Enemy::SetDamage(int _damage)
{
	if (paramChara_.timeInv > 0.0f) { return; }

	Object::SetDamage(_damage);

	const float knockXZ = 1.0f;
	const float knockY = 3.5f;

	//　吹っ飛ばし処理
	VECTOR dir = paramChara_.dir;
	dir.x /= 5.0f;
	dir.z /= 5.0f;

	// 後方+上に吹っ飛ばす
	dir.y = knockY;
	dir.x *= -(knockXZ);
	dir.z *= -(knockXZ);
	paramChara_.knockBack = dir;
	ChangeActionState(ACTION_STATE::KNOCK);

	// ダメージ演出
	DamagePerform();
}

void Enemy::Update(void)
{
	if (!paramChara_.isActive ||
		paramEnemy_.animState == ANIM_STATE::DEATH && anim_->IsEnd() && paramChara_.hp <= 0) return;

	Object::Update();

	// 索敵範囲有効
	SearchField();

	SearchAttackField();

	// 状態更新
	UpdateState();
	
	// アニメーション更新
	UpdateAnim();

	// 行列更新
	SetMatrixModel();
}
void Enemy::UpdateState(void)
{
	if (paramEnemy_.actionState == ACTION_STATE::SPAWN) { UpdateStateSpawn(); }
	if (paramEnemy_.actionState == ACTION_STATE::IDLE) { UpdateStateIdle(); }
	if (paramEnemy_.actionState == ACTION_STATE::MOVE) { UpdateStateMove(); }
	if (paramEnemy_.actionState == ACTION_STATE::KNOCK) { UpdateStateKnock(); }
	//if (paramEnemy_.actionState == ACTION_STATE::IDLE) { UpdateStateIdle(); }
}

void Enemy::UpdateStateSpawn(void)
{
}

void Enemy::UpdateStateIdle(void)
{
	if (paramEnemy_.isHearing)
	{
		ChangeActionState(ACTION_STATE::MOVE);
	}

	if (!AsoUtility::EqualsVZero(paramChara_.velocity))
	{
		UpdateModelFrames();
	}
}


void Enemy::UpdateStateMove(void)
{
	if (!paramEnemy_.isHearing) { ChangeActionState(ACTION_STATE::IDLE); }

	//if (paramEnemy_.isAttack) { ChangeActionState(ACTION_STATE::ATTACK_ACTIVE); }

	// 無敵中・撃破時・移動アニメーション以外のとき、処理終了
	if (paramChara_.timeInv > 0.0f || paramChara_.hp <= 0) { return; }

	// プレイヤー追従
	LookPlayerPos();

	VECTOR pos = VAdd(paramChara_.pos, paramChara_.posLocal);

	// Z軸の移動速度
	float speedZ = ((pos.z < player_.GetPos().z) ? paramChara_.speed : -paramChara_.speed);
	float accZ = ((pos.z < player_.GetPos().z) ? paramChara_.speedAcc : -paramChara_.speedAcc);
	accZ = ((pos.z == player_.GetPos().z) ? 0.0f : accZ);

	float speedX = ((pos.z < player_.GetPos().x) ? paramChara_.speed : -paramChara_.speed);
	float accX = ((pos.x < player_.GetPos().z) ? paramChara_.speedAcc : -paramChara_.speedAcc);
	accZ = ((pos.z == player_.GetPos().x) ? 0.0f : accZ);

	VECTOR velo;
	velo.x = _Move(&paramChara_.velocity.x, accX, speedX);
	velo.y = 0.0f;
	velo.z = _Move(&paramChara_.velocity.z, accZ, speedZ);

	paramChara_.velocity = VAdd(paramChara_.velocity, velo);

	VECTOR dir = { paramChara_.dir.x, 0.0f, paramChara_.dir.z };

	paramChara_.pos = VAdd(paramChara_.pos, VAdd(dir, velo));

	UpdateModelFrames();
}

void Enemy::UpdateStateKnock(void)
{
	UpdateModelFrames();
	if (AsoUtility::EqualsVZero(paramChara_.knockBack)) 
	{
		ChangeActionState(ACTION_STATE::IDLE);
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
	SceneManager& scene = SceneManager::GetInstance();

	// 無効状態・HPが0で撃破アニメーション終了の時、処理終了
	if (!paramChara_.isActive ||
		paramEnemy_.animState == ANIM_STATE::DEATH && anim_->IsEnd() && paramChara_.hp <= 0) { return; }


	COLOR_F color = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (scene.GetIsDebugMode())
	{
		DrawSearchRange();

		DrawAttackRange();

		if (paramChara_.timeInv > 0.0f && paramEnemy_.animState != ANIM_STATE::DEATH)
		{
			// 赤に描画
			color = { 75, 0, 0, 25 };
		}
	}

	MV1SetDifColorScale(paramChara_.handle, color);

	Object::Draw();
}
void Enemy::Release(void)
{
	Object::Release();
}


void Enemy::LookPlayerPos(void)
{
	VECTOR pPos = player_.GetPos();

	// ワールド座標
	VECTOR ePos = VAdd(paramChara_.pos, paramChara_.posLocal);

	// プレイヤーと敵の距離
	VECTOR diff = VSub(pPos, ePos);

	// 正規化したプレイヤーの方向を追尾
	paramChara_.dir = VNorm({ diff.x, 0.0f, diff.z });
	
	//float rotY = atan2f(diff.x, diff.z);
	//Quaternion pDir = Quaternion::Euler({ 0.0f, rotY, 0.0f });

	//VECTOR dir = { paramChara_.dir.x, 0.0f, paramChara_.dir.z };
	VECTOR dir = paramChara_.dir;
	// 向きに反映
	paramChara_.quaRot = Quaternion::LookRotation(dir, AsoUtility::AXIS_Y);
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

	float range = (paramEnemy_.searchRange * paramEnemy_.searchRange);
	// 視野左右内に入っている
	paramEnemy_.isHearing = (diff <= range);
}

void Enemy::SearchAttackField(void)
{
	VECTOR pPos = player_.GetPos();

	// 敵の向いている方向
	VECTOR eDir = VNorm(paramChara_.dir);

	// 敵とプレイヤーとの間のベクトル
	VECTOR diff = VSub(pPos, VAdd(paramChara_.pos, paramChara_.posLocal));

	// 敵から見たプレイヤーの方向
	VECTOR dirPlayerFromEnemy = VNorm(diff);


	// 内積を使ってベクトルの比較を行う
	// (+1.0)：２つのベクトルは同じ方向
	// (0.0)：２つのベクトルは直交
	// (-1.0)：２つのベクトルは逆方向
	float dot = VDot(eDir, dirPlayerFromEnemy);
	float angle = acosf(dot);

	const float colRad = AsoUtility::Deg2Rad(paramEnemy_.atkRange);

	// 視野左右内に入っている
	paramEnemy_.isAttack = (angle <= colRad);
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
		left = VTransform(AsoUtility::DIR_FORWARD, leftMat);

		rightMat = MMult(mat, MGetRotY(AsoUtility::Deg2Rad(angle * (i + 1))));
		right = VTransform(AsoUtility::DIR_FORWARD, rightMat);

		leftPos = VAdd(pos, VScale(left, paramEnemy_.searchRange));
		rightPos = VAdd(pos, VScale(right, paramEnemy_.searchRange));

		DrawTriangle3D(pos, leftPos, rightPos, color, true);
	}
}

void Enemy::DrawAttackRange(void)
{
	unsigned int color = ((paramEnemy_.isAttack) ? 0xffaaaa : 0xaa00ff);
	const float searchAngle = 50.0f;

	MATRIX mat = Quaternion::ToMatrix(paramChara_.quaRot);

	VECTOR pos = VAdd(paramChara_.pos, paramChara_.posLocal);

	// 前方方向
	VECTOR forward = paramChara_.quaRot.GetForward();

	// 左側方向
	MATRIX leftMat = MMult(mat, MGetRotY(AsoUtility::Deg2Rad(-searchAngle)));
	VECTOR left = VTransform(AsoUtility::DIR_FORWARD, leftMat);

	// 右側方向
	MATRIX rightMat = MMult(mat, MGetRotY(AsoUtility::Deg2Rad(searchAngle)));
	VECTOR right = VTransform(AsoUtility::DIR_FORWARD, rightMat);


	// 正面の位置
	VECTOR forwardPos = VAdd(pos, VScale(forward, paramEnemy_.atkRange));

	// 左の位置
	VECTOR leftPos = VAdd(pos, VScale(left, paramEnemy_.atkRange));

	// 右の位置
	VECTOR rightPos = VAdd(pos, VScale(right, paramEnemy_.atkRange));

	// 描画処理
	DrawTriangle3D(pos, leftPos, forwardPos, color, true);
	DrawTriangle3D(pos, forwardPos, rightPos, color, true);
}