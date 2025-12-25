#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "Transform.h"

Transform::Transform(void)
	:
	modelId(-1),
	scl(AsoUtility::VECTOR_ONE),
	rot(AsoUtility::VECTOR_ZERO),
	pos(AsoUtility::VECTOR_ZERO),
	localPos(AsoUtility::VECTOR_ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()),
	quaRot(Quaternion()),
	quaRotLocal(Quaternion())

{
}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{
	// 大きさ
	matScl = MGetScale(scl);

	// 回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// 位置
	matPos = MGetTranslate(pos);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// 行列をモデルに判定
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

}

void Transform::Release(void)
{
}

void Transform::SetModel(int model)
{
	modelId = model;
}

void Transform::InitTransform(const VECTOR& _scl, const Quaternion& _rot, const Quaternion& _rotLocal, const VECTOR& _pos)
{
	scl = _scl;
	quaRot = _rot;
	quaRotLocal = _rotLocal;
	pos = _pos;

	Update();
}
void Transform::InitTransform(float _scl, const Quaternion& _rot, const Quaternion& _rotLocal, const VECTOR& _pos)
{
	InitTransform({ _scl,_scl,_scl }, _rot, _rotLocal, _pos);
}
void Transform::InitTransform(float _scl, const Quaternion& _rot, const Quaternion& _rotLocal)
{
	scl = VGet(_scl, _scl, _scl);
	quaRot = _rot;
	quaRotLocal = _rotLocal;

	Update();
}
void Transform::InitTransform(void)
{
	InitTransform(AsoUtility::VECTOR_ONE,
				  Quaternion::Identity(), Quaternion::Identity(),
				  AsoUtility::VECTOR_ZERO);
}


void Transform::Translate(const VECTOR& _movePow)
{
	pos = VAdd(pos, _movePow);
	Update();
}

void Transform::Translate(const VECTOR& _dir, float _movePow)
{
	// 正規化して移動
	VECTOR dir = AsoUtility::VNormalize(_dir);
	VECTOR moveVec = VScale(dir, _movePow);
	Translate(moveVec);
}

void Transform::Rotate(const Quaternion& rot)
{
	quaRot = Quaternion::Mult(quaRot, rot);
	Update();
}
void Transform::Rotate(const VECTOR& _axis, float _pow)
{
	Quaternion qua = Quaternion::AngleAxis(_pow, _axis);
	Rotate(qua);
}

VECTOR Transform::GetForward(void) const
{
	return GetDir(AsoUtility::DIR_FORWARD);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(AsoUtility::DIR_BACK);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(AsoUtility::DIR_RIGHT);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(AsoUtility::DIR_LEFT);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(AsoUtility::DIR_UP);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(AsoUtility::DIR_DOWN);
}

VECTOR Transform::GetDir(const VECTOR& dir) const
{
	return quaRot.PosAxis(dir);
}
