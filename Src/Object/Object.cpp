#include "Object.h"
#include <DxLib.h>
#include <string>
#include <cassert>
#include "./Status/StatusData.h"
#include "./AnimationController.h"
#include "../Application.h"
#include "../Common/Quaternion.h"
#include "../Common/Vector2.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/Sound.h"

Object::Object(void)
{
	paramChara_.handle = -1;
	paramChara_.velocity = {};

	paramChara_.pos = paramChara_.prePos = {};
	paramChara_.posLocal = {};

	paramChara_.quaRot = Quaternion::Identity();
	paramChara_.quaRotLocal = Quaternion::Identity();

	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(AsoUtility::Deg2Rad(0.0f), AsoUtility::AXIS_X));

	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(AsoUtility::Deg2Rad(0.0f), AsoUtility::AXIS_Y));

	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
		Quaternion::AngleAxis(AsoUtility::Deg2Rad(0.0f), AsoUtility::AXIS_Z));

	
	paramChara_.timeInv = 0.0f;
	paramChara_.isGround = true;

	anim_ = nullptr;
	//collision_ = nullptr;

	Load();
}

void Object::Init(void)
{
	paramChara_.velocity = AsoUtility::VECTOR_ZERO;
	paramChara_.dir = {};

	// フレーム初期化処理
	InitModelFrame();

	SetParam();

	// アニメーション初期化処理
	InitAnim();

	// モデル位置割り当て処理
	SetMatrixModel();
}
void Object::InitModelFrame(void)
{
	int max = MV1GetFrameNum(paramChara_.handle);

	for (int i = 0; i < max; i++)
	{
		Frame frame;
		frame.name = MV1GetFrameName(paramChara_.handle, i);
		frame.num = i;
		frame.pos = MV1GetFramePosition(paramChara_.handle, i);
		frame.localMat = MV1GetFrameLocalMatrix(paramChara_.handle, i);

		// フレーム全表示
		frame.isVisible = true;
		MV1SetFrameVisible(paramChara_.handle, i, frame.isVisible);

		// フレーム格納
		paramChara_.frames.push_back(frame);
	}
}

void Object::Draw(void)
{
	/*　描画処理　*/

	COLOR_F defaultColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	COLOR_F color = defaultColor;

	if (paramChara_.timeInv > 0.0f)
	{
		int team = static_cast<int>(paramChara_.timeAct * 10.0f);
		if (team % COLOR_TEAM == 0)
		{
			color = paramChara_.damageColor;
		}
	}

	MV1SetDifColorScale(paramChara_.handle, color);

	// モデル描画処理
	MV1DrawModel(paramChara_.handle);

	VECTOR pos = VAdd(paramChara_.pos, paramChara_.posLocal);
	DrawSphere3D(pos, paramChara_.radius, 8, 0xaaaaaa, 0xaaaaaa, false);
}

void Object::DrawDebug(void)
{
	//#ifdef _DEBUG
	if (SceneManager::GetInstance().GetIsDebugMode())
	{
		// 向き描画
		VECTOR pos = VAdd(paramChara_.pos, paramChara_.posLocal);
		AsoUtility::DrawLineXYZ(pos, paramChara_.quaRot);
	}
	//#endif
}


void Object::SetMatrixModel(void)
{
	// 大きさ
	MATRIX matScl = MGetScale(paramChara_.scale);

	// 回転
	Quaternion mixRot = Quaternion::Identity();

	// ローカル回転を加える
	mixRot = Quaternion::Mult(paramChara_.quaRot, paramChara_.quaRotLocal);

	// 位置
	VECTOR pos = VAdd(paramChara_.pos, paramChara_.posLocal);
	MATRIX matPos = MGetTranslate(pos);


	/* 行列の合成 */

	// 行列初期化
	MATRIX mat = MGetIdent();

	// 大きさ
	mat = MMult(mat, matScl);

	// 回転を行列に変換
	mat = MMult(mat, mixRot.ToMatrix());

	// 位置
	mat = MMult(mat, matPos);


	// 行列(大きさ、角度、位置)をモデルに設定
	MV1SetMatrix(paramChara_.handle, mat);

	// クォータニオン角からオイラー角に割り当て
	paramChara_.rot = paramChara_.quaRot.ToEuler();
}

void Object::UpdateModelFrame(Object::COLLISION_TYPE _type)
{
	std::string name = paramChara_.colList[_type]->name;
	int index = FindFrameNum(name);

	// フレーム状態更新
	paramChara_.frames[index].pos = MV1GetFramePosition(paramChara_.handle, index);
	paramChara_.frames[index].localMat = MV1GetFrameLocalMatrix(paramChara_.handle, index);
}
int Object::FindFrameNum(const std::string& name)
{
	int num = -1;
	for (auto& frame : paramChara_.frames)
	{
		// 名前と一致時フレーム番号を返す
		if (frame.name == name)
		{
			num = frame.num;
			break;
		}
	}
	assert("\nフレーム名が一致しませんでした。\n");
	return num;
}


void Object::RevertPosXZ(const VECTOR& revertVec, float power)
{
	// 押し出すベクトルを正規化
	VECTOR dir = { revertVec.x, 0.0f, revertVec.z };
	dir = VNorm(dir);

	// 押し戻す倍率をスカラー倍
	VECTOR revert = VScale(dir, power);


	if (revert.x != 0.0f)
	{
		// X軸加速度無効化
		paramChara_.velocity.x = 0.0f;

		// 現在位置を前位置に戻す
		paramChara_.pos.x = paramChara_.prePos.x;

		// 現在位置を一定の方向に戻す
		paramChara_.pos.x += revert.x;
	}

	if (revert.z != 0.0f)
	{
		// Z軸加速度無効化
		paramChara_.velocity.z = 0.0f;

		// 現在位置を前位置に戻す
		paramChara_.pos.z = paramChara_.prePos.z;

		// 現在位置を一定の方向に戻す
		paramChara_.pos.z += revert.z;
	}
}

void Object::RevertPosY(float revPosY, bool isVeloReset)
{
	if (isVeloReset)
	{
		// 初期化を許可した時、Y軸加速度初期化
		paramChara_.velocity.y = 0.0f;

		// 着地処理
		paramChara_.isGround = true;
	}

	// 着地位置に戻す
	paramChara_.prePos.y = revPosY;
	paramChara_.pos.y = paramChara_.prePos.y;
}

void Object::SetDamage(int damage)
{
	/*　被ダメージ処理　*/

	if (damage == 0)
	{
		// ダメージが０の時は処理終了
		return;
	}

	// ダメージがマイナス時、正の数に変換する
	int damageValue = ((damage < 0) ? -damage : damage);

	// HP自体にダメージ
	paramChara_.hp -= damageValue;


	if (paramChara_.hp < 0)
	{
		paramChara_.hp = 0;
	}
}

void Object::Gravity(void)
{
	if (paramChara_.isGround) return;

	// 重力加速
	paramChara_.velocity.y -= Application::GRAVITY_ACC;


	// 下への加速度の制限
	if (paramChara_.velocity.y < VELOCITY_Y_MIN)
	{
		paramChara_.velocity.y = VELOCITY_Y_MIN;
	}

	// 重力加速制限
	if (paramChara_.velocity.y > Application::GRAVITY_MAX)
	{
		paramChara_.velocity.y = Application::GRAVITY_MAX;
	}

}


float Object::WeightCalc(float weight, float weightPower, float velocity)
{
	float num = velocity;

	// 重量の係数
	float param = (WEIGHT_START - weight);

	if (param > 0.0f)
	{
		// 重さの倍率を加算((1 - 現在の重さ) × 重さ上昇倍率)
		num += (param * weightPower);
	}

	return num;
}

void Object::KnockBack(const VECTOR& targetPos, float invTime,
	float powerY, float powerXZ, float minPowerXZ)
{
	// 吹っ飛ばす方向
	VECTOR knockVelo = VSub(paramChara_.pos, targetPos);

	// Y軸調整
	knockVelo.y = powerY;

	// 吹っ飛ばし方向を正規化
	knockVelo = AsoUtility::VNormalize(knockVelo);

	float power = powerXZ;

	// 重さを計算に含める
	knockVelo.x *= power;
	knockVelo.z *= power;
	knockVelo.y *= powerY;


	float gravity = Application::GRAVITY_ACC;
	knockVelo.y += gravity;


	// Yの加速度が負の値の時は、Y吹っ飛ばしを０にする
	if (knockVelo.y < 0.0f) knockVelo.y = 0.0f;

	// 無敵時間割り当て
	paramChara_.timeInv = invTime;

	// 移動量に加算
	paramChara_.velocity = VAdd(paramChara_.velocity, knockVelo);
}

bool Object::CheckActiveAttack(void) const
{
	bool ret = false;

	// 攻撃判定が有効中か否か
	if (paramChara_.attackState == ATTACK_STATE::ACTIVE &&
		paramChara_.timeAct > 0.0f)
	{
		// 攻撃有効時、true
		ret = true;
	}

	return ret;

}

float Object::_Move(const float* _curVelo, float _movePow, float _maxVelo)
{
	// 現在移動量
	float velocity = 0.0f;

	// 移動加算量が0時、処理終了
	if (_movePow == 0.0f) return 0.0f;


	// 移動方向と反対に移動時
	if (*_curVelo < 0.0f && _movePow > 0.0f ||
		*_curVelo > 0.0f && _movePow < 0.0f)
	{
		// 移動量に加速度を渡す
		velocity = -(*_curVelo);
	}

	// 加速度が移動量を移動制限がかからない時、移動量が
	if (*_curVelo < _maxVelo && _movePow > 0.0f ||
		*_curVelo > _maxVelo && _movePow < 0.0f)
	{
		//移動量で移動量を増減
		velocity += _movePow;
	}


	return velocity;
}

void Object::Rotation(bool isRevert)
{
	/*　回転処理　*/
	VECTOR velo = GetPosDelta();

	// Y軸は無効
	velo.y = 0.0f;

	// 静止時は処理を終了

	if (velo.x == AsoUtility::VECTOR_ZERO.x &&
		velo.z == AsoUtility::VECTOR_ZERO.z ||
		paramChara_.pos.x == paramChara_.prePos.x &&
		paramChara_.pos.z == paramChara_.prePos.z) return;

	// 反転フラグ有効時向きを反転
	velo = ((isRevert) ? VScale(velo, -1) : velo);

	// 進行方向にプレイヤーを向かせる
	Quaternion rot = Quaternion::LookRotation(velo, AsoUtility::AXIS_Y);
	paramChara_.quaRot = rot;
}

float Object::DecVelocityXZ(const float* acc)
{
	float vel = *acc;

	// 移動量が０の時、処理終了
	if (*acc == 0.0f) return 0.0f;


	float speedDec = MOVE_DEC_POWER;

	if (vel < 0.0f)
	{
		vel -= MOVE_DEC_POWER; // 減速処理

		// 移動減少値がゼロクロス時
		if (vel >= 0.0f)
		{
			// 移動量初期化
			vel = 0.0f;
		}
	}
	else
	{
		vel += MOVE_DEC_POWER;  // 減速処理

		// 移動減少値がゼロクロス時
		if (vel <= 0.0f)
		{
			// 移動量初期化
			vel = 0.0f;
		}
	}

	return vel;
}




void Object::ChangeAttackState(ATTACK_STATE state, float _activeTime)
{
	// 状態割り当て
	paramChara_.attackState = state;

	// 行動時間割り当て
	paramChara_.timeAct = _activeTime;
}

void Object::ChangeAttackStateNext(float _activeTime)
{
	// 次の行動状態に遷移
	ATTACK_STATE state = paramChara_.attackState;
	int next = (static_cast<int>(state) + 1);
	state = static_cast<ATTACK_STATE>(next);

	// 攻撃が終了時、無効状態に戻す
	if (state == ATTACK_STATE::MAX) { state = ATTACK_STATE::NONE; }

	// 状態割り当て
	paramChara_.attackState = state;

	// 行動時間割り当て
	paramChara_.timeAct = _activeTime;
}

VECTOR& Object::GetPosChatch(void)
{
	/* 持ちあげ位置を取得 */
	return paramChara_.posChatch;
}

const VECTOR& Object::GetPosDelta(void) const
{
	/* 移動量を取得 */
	return VSub(paramChara_.pos, paramChara_.prePos);
}

const VECTOR& Object::GetFramePos(COLLISION_TYPE _type)
{
	std::string name = paramChara_.colList[_type]->name;
	int frameIndex = FindFrameNum(name);

	// 座標更新して値を返す
	VECTOR pos = MV1GetFramePosition(paramChara_.handle, frameIndex);
	paramChara_.frames[frameIndex].pos = pos;

	return paramChara_.frames[frameIndex].pos;
}
