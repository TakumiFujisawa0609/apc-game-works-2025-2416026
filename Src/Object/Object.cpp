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

Object::Object(void)
{
	paramChara_.handle = -1;
	paramChara_.velocity = {};

	paramChara_.pos = paramChara_.prePos = {};
	paramChara_.posLocal = {};
	paramChara_.posForward = {};
	paramChara_.isActive = false;	

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

	// 当たり判定位置更新
	UpdateModelFrames();
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

void Object::Update(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	// 現在位置割り当て
	paramChara_.prePos = paramChara_.pos;

	// Y座標がマイナス時、0にする
	if (paramChara_.pos.y < 0.0f)
	{
		paramChara_.pos.y = paramChara_.prePos.y = 0.0f;
	}
	

	if(paramChara_.timeInv > 0.0f)
	{
		paramChara_.timeInv -= delta;
	}

	if (!AsoUtility::EqualsVZero(paramChara_.knockBack))
	{
		GravityKnock();

		UpdateModelFrames();
	}
}

void Object::Draw(void)
{
	/*　描画処理　*/

	// モデル描画処理
	MV1DrawModel(paramChara_.handle);

	if (SceneManager::GetInstance().GetIsDebugMode())
	{
		// 向き描画
		VECTOR pos = VAdd(paramChara_.pos, paramChara_.posLocal);
		AsoUtility::DrawLineXYZ(pos, paramChara_.quaRot);

		// 当たり判定表示
		unsigned color = 0xaaaaaa;

		for (auto& col : paramChara_.colList)
		{
			if (col.first == COLLISION_TYPE::BODY) continue;

			color = ((col.first == COLLISION_TYPE::HEAD) ? 0xff0000 : color);
			color = ((col.first == COLLISION_TYPE::BOTTOM) ? 0x00ff00 : color);
			color = ((col.first == COLLISION_TYPE::HAND_L) ? 0x0000ff : color);
			color = ((col.first == COLLISION_TYPE::HAND_R) ? 0x0000ff : color);

			// 当たり判定位置
			DrawSphere3D(col.second->pos, 8, 16, color, 0xffffff, true);

			// 当たり判定範囲
			DrawSphere3D(col.second->pos, paramChara_.radius / 2.0f, 16,
				color, 0xffffff, false);
		}

		// 胴体の当たり判定
		DrawSphere3D(paramChara_.colList[COLLISION_TYPE::BODY]->pos, paramChara_.radius, 8,
			0xffffff, 0xffffff, false);
	}
}

void Object::DrawDebug(void)
{
	//#ifdef _DEBUG
	
	//#endif
}

void Object::Release(void)
{
	//animSpeed_.clear();
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

void Object::UpdateModelFrames(void)
{
	// 当たり判定位置更新
	for (auto& col : paramChara_.colList)
	{
		UpdateModelFrame(col.first);
	}

	SetPosForward();
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

void Object::SetDamage(int _damage)
{
	/*　被ダメージ処理　*/
	
	const float invTime = 0.75f;
	// ダメージが０の時は処理終了
	if (_damage <= 0 || paramChara_.timeInv > 0.0f) { return; }

	// ダメージがマイナス時、正の数に変換する
	int damageValue = ((_damage < 0) ? -_damage : _damage);

	// HP減少
	paramChara_.hp -= damageValue;

	paramChara_.timeInv = invTime;

	if (paramChara_.hp < 0) { paramChara_.hp = 0; }
}

void Object::GravityKnock(void)
{
	// 重力加速
	paramChara_.knockBack.y -= Application::GRAVITY_ACC;

	if (paramChara_.knockBack.y > Application::GRAVITY_MAX)
	{
		paramChara_.knockBack.y = Application::GRAVITY_MAX;
	}

	// 加速度に反映
	paramChara_.velocity = VAdd(paramChara_.velocity, paramChara_.knockBack);

	// 位置に反映
	paramChara_.pos = VAdd(paramChara_.pos, paramChara_.velocity);
	
	if (paramChara_.knockBack.y < 0.0f &&
		paramChara_.pos.y <= 0.0f)
	{
		paramChara_.pos.y = 0.0f;
		paramChara_.velocity = paramChara_.knockBack = AsoUtility::VECTOR_ZERO;
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
	paramChara_.knockBack = VAdd(paramChara_.velocity, knockVelo);
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




void Object::SetPosForward(void)
{
	VECTOR forward = VScale(paramChara_.quaRot.GetForward(), paramChara_.radius);
	VECTOR pos = VAdd(paramChara_.colList[COLLISION_TYPE::BODY]->pos, forward);

	paramChara_.posForward = pos;
}

void Object::ChangeAttackState(ATTACK_STATE _state, float _activeTime, SoundManager::SRC _se)
{
	// 状態割り当て
	paramChara_.attackState = _state;

	// 行動時間割り当て
	paramChara_.timeAct = _activeTime;

	if (_state == ATTACK_STATE::ACTIVE)
	{
		// 効果音再生
		SoundManager::GetInstance().Play(_se, false);
	}
}

void Object::ChangeAttackStateNext(float _activeTime, SoundManager::SRC _se)
{
	// 次の行動状態に遷移
	ATTACK_STATE state = paramChara_.attackState;
	int next = (static_cast<int>(state) + 1);
	state = static_cast<ATTACK_STATE>(next);

	// 攻撃が終了時、無効状態に戻す
	if (state == ATTACK_STATE::MAX) { state = ATTACK_STATE::NONE; }

	// 効果音再生
	if (state == ATTACK_STATE::ACTIVE) { SoundManager::GetInstance().Play(_se, false); }

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

float Object::GetRadius(COLLISION_TYPE _type)
{
	//paramChara_.colList[_type].radius;

	return paramChara_.radius;
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
