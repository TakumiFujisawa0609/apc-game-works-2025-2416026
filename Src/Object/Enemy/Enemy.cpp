#include "Enemy.h"
#include <DxLib.h>
#include "../Object.h"
#include "../Player/Player.h"
#include "../Common/AnimationController.h"
#include "../Status/StatusData.h"
#include "../Status/StatusEnemy.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/AsoUtility.h"
#include "../../Common/Quaternion.h"

Enemy::Enemy(StatusEnemy::TYPE type, Player& player, bool _isElite)
	:status_(StatusData::GetInstance().GetEnemyStatus(static_cast<int>(type))),
	player_(player)
{
	paramEnemy_.isElite = _isElite;
	Load();
}

void Enemy::LoadPost(void)
{
	// 敵のハンドル割り当て
	int type = static_cast<int>(status_.GetEnemyType());
	type += static_cast<int>(ResourceManager::SRC::MODEL_ENEMY);
	ResourceManager::SRC src = static_cast<ResourceManager::SRC>(type);

	paramChara_.handle = ResourceManager::GetInstance().LoadModelDuplicate(src);
}

void Enemy::SetParam(void)
{
	paramEnemy_.name = status_.GetName();

	int frameNum = FindFrameNum(paramChara_.colList[Object::COLLISION_TYPE::BODY]->name);// フレーム名割り当て
	VECTOR sub = VSub(paramChara_.frames[frameNum].pos, paramChara_.pos);
	paramChara_.posLocal = VSub(paramChara_.pos, sub);

	paramEnemy_.type = status_.GetEnemyType();
	paramEnemy_.searchRange = status_.GetSearchRange();
	paramEnemy_.atkRange = status_.GetAtkRange();

	paramChara_.quaRot = Quaternion::Identity();
	paramChara_.quaRotLocal = Quaternion::Identity(); // ローカル回転初期化
	
	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(LOCAL_ANGLE_Y, AsoUtility::AXIS_Y));

	float scale = status_.GetScale();
	paramChara_.scale = { scale * (1.0f - SCALE_DIFF),
						  scale * (1.0f + SCALE_DIFF),
						  scale * (1.0f - SCALE_DIFF) };
	paramChara_.radius = status_.GetRadius();

	paramChara_.hp = status_.GetMaxHp();
	paramChara_.power = status_.GetPower();
	paramChara_.speed = status_.GetSpeed();
	paramChara_.speedAcc = status_.GetSpeedAcc();

	paramEnemy_.timeAtkInterval = status_.GetAtkInterval();
	paramEnemy_.isHearing = false;
	paramChara_.isActive = true;

}
void Enemy::InitPost(void)
{
	// 行動状態
	ChangeActionState(ACTION_STATE_START);
}


void Enemy::Update(void)
{
	Object::Update();

	// HP０で死亡状態アニメーション終了とき、処理終了
	if (paramEnemy_.animState == ANIM_STATE::DEATH &&
		anim_->IsEnd() && paramChara_.hp <= 0) { return; }

	if (paramChara_.isActive &&
		paramEnemy_.actionState != ACTION_STATE::SPAWN)
	{
		// 索敵範囲判定
		SearchField();

		// 攻撃範囲判定
		SearchAttackField();
	}

	//UpdateAnim();

	// 状態更新
	UpdateState();

	// 行列更新
	SetMatrixModel();
}
void Enemy::UpdateState(void)
{
	// 生成状態
	if (paramEnemy_.actionState == ACTION_STATE::SPAWN) { UpdateStateSpawn(); }

	// 待機状態
	else if (paramEnemy_.actionState == ACTION_STATE::IDLE) { UpdateStateIdle(); }

	// 移動状態
	else if (paramEnemy_.actionState == ACTION_STATE::MOVE) { UpdateStateMove(); }

	// 攻撃状態
	else if (IsAttackState()) { UpdateStateAtk(); }

	// 吹っ飛ばし状態
	else  if (paramEnemy_.actionState == ACTION_STATE::KNOCK) { UpdateStateKnock(); }
}

void Enemy::UpdateStateSpawn(void)
{
	const float delta = SceneManager::GetInstance().GetDeltaTime();

	if (paramChara_.timeInv <= 0.0f && 
		anim_->IsEnd())
	{
		ChangeActionState(ACTION_STATE::IDLE);
	}
}
void Enemy::UpdateStateIdle(void)
{
	if (paramEnemy_.isHearing)
	{
		ChangeActionState(ACTION_STATE::MOVE);
	}
}
void Enemy::UpdateStateMove(void)
{
	if (!paramEnemy_.isHearing) { ChangeActionState(ACTION_STATE::IDLE); }

	//if (paramEnemy_.isAttack) { ChangeActionState(ACTION_STATE::ATTACK_ACTIVE); }

	// 無敵中・撃破時・移動アニメーション以外のとき、処理終了
	if (paramChara_.timeInv > 0.0f || paramChara_.hp <= 0) { return; }

	if (paramEnemy_.isAttack)
	{
		// 攻撃間隔減少
		float delta = SceneManager::GetInstance().GetDeltaTime();
		paramEnemy_.timeAtkInterval -= ((paramEnemy_.timeAtkInterval > 0.0f) ? delta : 0.0f);

		// 一定時間プレイヤーが攻撃範囲内進入時、攻撃状態化
		if (paramEnemy_.timeAtkInterval <= 0.0f)
		{
			ChangeActionState(ACTION_STATE::ATTACK);
		}
	}

	// プレイヤー追従
	LookPlayerPos();

	// 吹っ飛ばされていないとき、移動
	if (AsoUtility::EqualsVZero(paramChara_.knockBack))
	{
		Move();

		if (AsoUtility::EqualsVZero(paramChara_.velocity))
		{
			ChangeActionState(ACTION_STATE::IDLE);
		}
	}
}
void Enemy::UpdateStateKnock(void)
{
	if (AsoUtility::EqualsVZero(paramChara_.knockBack) &&
		anim_->IsEnd()) 
	{
		ChangeActionState(ACTION_STATE::IDLE);
	}
}
void Enemy::UpdateStateAtk(void)
{
	paramChara_.atkMotion.Update();

	// 攻撃有効
	if (paramChara_.atkMotion.GetIsActionAttack())
	{

	}
	if (paramChara_.atkMotion.GetAttackState() == AttackMotion::ATTACK_STATE::NONE/* &&
		paramChara_.atkMotion.GetTimeAttack() < 0.0f*/)
	{
		paramEnemy_.timeAtkInterval = status_.GetAtkInterval();
		paramEnemy_.isAttack = false;
		ChangeActionState(ACTION_STATE::IDLE);
	}
}

void Enemy::ChangeActionState(ACTION_STATE state)
{
	paramEnemy_.actionState = state;
	ANIM_STATE animState = ANIM_STATE::NONE;
	bool isLoop = false;


	if (state == ACTION_STATE::SPAWN)
	{
		animState = ANIM_STATE::SPAWN;

		// 無敵時間をランダムで割り当て
		paramChara_.timeInv = SPAWN_TIME + GetRand(SPAWN_TIME_RANGE);
	}

	else if (state == ACTION_STATE::IDLE)
	{
		animState = ANIM_STATE::IDLE;
		isLoop = true;
	}

	else if (state == ACTION_STATE::MOVE)
	{
		animState = ANIM_STATE::WALK;
		isLoop = true;
	}

	else if (state == ACTION_STATE::ATTACK)
	{
		const float ACTIVE_TIME = 1.0f;
		const float END_TIME = 1.0f;
		const float ATTACK_TIME = 0.75f;
		animState = ANIM_STATE::ATTACK;
		paramChara_.atkMotion.SetMotion(ACTIVE_TIME, END_TIME, ATTACK_TIME, false);
	}

	else if (state == ACTION_STATE::KNOCK)
	{
		// HPによってダメージ演出を変える
		animState = ((paramChara_.hp > 0) ? ANIM_STATE::HIT_2 : ANIM_STATE::DEATH);
	}

	// アニメーション再生
	ChangeAnimState(animState, isLoop);
}


void Enemy::DrawPost(void)
{
	SceneManager& scene = SceneManager::GetInstance();

	// HPが0で撃破アニメーション終了の時、処理終了
	if (paramEnemy_.animState == ANIM_STATE::DEATH &&
		anim_->IsEnd() && paramChara_.hp <= 0) { return; }


	UtilityCommon::Color color = { 255, 255, 255, 1.0f };

	if (scene.GetIsDebugMode())
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawSearchRange();

		DrawAttackRange();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (paramChara_.timeInv > 0.0f && paramEnemy_.animState != ANIM_STATE::DEATH)
		{
			// 赤に描画
			color = { 175, 0, 0, 25 };
		}

		if (SceneManager::GetInstance().GetIsDebugMode())
		{
			color = { 255,0,0 };

			// 正面座標
			if (paramChara_.atkMotion.GetIsActionAttack())
			{
				float forwardRad = status_.GetAtkRange();
				DrawSphere3D(paramChara_.posForward, forwardRad,
					16, UtilityCommon::SetColor(color), 0xffffff, false);
			}
		}
	}
}


void Enemy::DamageProc(void)
{
	ChangeActionState(ACTION_STATE::KNOCK);
}

bool Enemy::GetIsCollisionActive(void)
{
	bool ret = true;

	// 無効時・HP0時・生成状態時、当たり判定無効
	if (!GetIsActive() || GetCurHp() <= 0 ||
		paramEnemy_.actionState == ACTION_STATE::SPAWN)
	{
		ret = false;
	}
	return ret;
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
void Enemy::UpdatePost(void)
{
}

void Enemy::Move(void)
{
	VECTOR pos = VAdd(paramChara_.pos, paramChara_.posLocal);

	// Z軸の移動速度
	float speedZ = ((pos.z < player_.GetPos().z) ? paramChara_.speed : -paramChara_.speed);
	float accZ = ((pos.z < player_.GetPos().z) ? paramChara_.speedAcc : -paramChara_.speedAcc);
	accZ = ((pos.z == player_.GetPos().z) ? 0.0f : accZ);

	// X軸の移動速度
	float speedX = ((pos.x < player_.GetPos().x) ? paramChara_.speed : -paramChara_.speed);
	float accX = ((pos.x < player_.GetPos().x) ? paramChara_.speedAcc : -paramChara_.speedAcc);
	accX = ((pos.x == player_.GetPos().x) ? 0.0f : accZ);

	VECTOR velo;
	velo.x = _Move(&paramChara_.velocity.x, accX, speedX);
	velo.y = 0.0f;
	velo.z = _Move(&paramChara_.velocity.z, accZ, speedZ);

	paramChara_.velocity = VAdd(paramChara_.velocity, velo);

	VECTOR dir = { paramChara_.dir.x, 0.0f, paramChara_.dir.z };

	paramChara_.pos = VAdd(paramChara_.pos, VAdd(dir, velo));
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

	// 円の分割数
	const int split = 36;
	const float angle = (360.0f / split);

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
		DrawLine3D(leftPos, rightPos, 0x0);
	}
}

void Enemy::DrawAttackRange(void)
{
	unsigned int color = ((paramEnemy_.isAttack) ? 0xff0000 : 0xaa00ff);
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


void Enemy::AnimState(void)
{
	// 生成アニメーション時は処理終了
	if (paramEnemy_.animState == ANIM_STATE::SPAWN ||
		!AsoUtility::EqualsVZero(paramChara_.knockBack)) { return; }


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
		if (paramEnemy_.actionState == ACTION_STATE::MOVE &&
			paramChara_.timeInv <= 0.0f)
		{
			//ChangeAnimState(ANIM_STATE::WALK);
		}
		else
		{
			//ChangeAnimState(ANIM_STATE::IDLE);
		}
	}
}

bool Enemy::IsAttackState(void)
{
	return (paramEnemy_.actionState == ACTION_STATE::ATTACK ||
			paramEnemy_.actionState == ACTION_STATE::ATTACK_START ||
			paramEnemy_.actionState == ACTION_STATE::ATTACK_ACTIVE ||
			paramEnemy_.actionState == ACTION_STATE::ATTACK_END);
}

bool Enemy::IsUpdateFrame(void)
{
	bool ret = false;
	if (paramChara_.isActive)
	{
		ret = true;
	}
	return ret;
}