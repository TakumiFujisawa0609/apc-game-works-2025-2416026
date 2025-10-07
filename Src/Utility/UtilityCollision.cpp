#include "UtilityCollision.h"
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "./AsoUtility.h"


bool UtilityCollision::IsHitBoxToBox(const Vector2& _pos1, const Vector2& _pos2, const Vector2& _size1, const Vector2& _size2)
{
	/*　2D矩形当たり判定処理　*/
	bool isX = (_pos1.x < (_pos2.x + _size2.x)) && ((_pos1.x + _size1.x) > _pos2.x);
	bool isY = (_pos1.y < (_pos2.y + _size2.y)) && ((_pos1.y + _size1.y) > _pos2.y);

	return (isX && isY);
}
bool UtilityCollision::IsHitBoxToBox(const VECTOR& _pos1, const VECTOR& _pos2, const VECTOR& _size1, const VECTOR& _size2)
{
	/*　3D矩形当たり判定処理　*/

	bool isX = (_pos1.x < (_pos2.x + _size2.x)) && ((_pos1.x + _size1.x) > _size2.x);
	bool isY = (_pos1.y < (_pos2.y + _size2.y)) && ((_pos1.y + _size1.y) > _size2.y);
	bool isZ = (_pos1.z < (_pos2.z + _size2.z)) && ((_pos1.z + _size1.z) > _size2.z);

	return (isX && isY && isZ);
}

bool UtilityCollision::IsHitCircleCollision2D(const Vector2& _pos1, const Vector2& _pos2, float _radius1, float _radius2)
{
	/*　2D円形当たり判定処理　*/

	// 両者の座標の距離を取得
	Vector2 dis = _pos1;
	dis.x -= _pos2.x;
	dis.y -= _pos2.y;


	float midPos = ((dis.x * dis.x) + (dis.y * dis.y));
	float rad = (_radius1 + _radius2); // 両者の半径

	// 両者の距離が半径よりも短い時、true
	return (midPos <= (rad * rad));
}

bool UtilityCollision::IsHitSphareToSphere(const VECTOR& _pos1, const VECTOR& _pos2,
	                                       float _radius1, float _radius2)
{
	/*　3D円形同士の当たり判定処理　*/

	// 両者の座標の距離を取得
	VECTOR dis = _pos1;

	// 座標を減算
	dis = VSub(dis, _pos2);


	// ベクトル間の長さ
	float length = ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z));

	// 両者の半径
	float rad = (_radius1 + _radius2);

	// 両者の距離が半径よりも短い時、true
	return (length <= (rad * rad));
}

bool UtilityCollision::IsHitSphereToCapsule(const VECTOR& _sphPos, float _sphRadius, const VECTOR& _capTopPos1, const VECTOR& _capBottomPos2, float _capRadius)
{
	/*　球体とカプセルの当たり判定　*/


	bool ret = false;

	// カプセル球体の中心を繋ぐベクトル
	VECTOR _cap1to2 = VSub(_capBottomPos2, _capTopPos1);

	// ベクトルを正規化
	VECTOR _cap1to2ENor = VNorm(_cap1to2);


	// カプセル繋ぎの単位ベクトルと、
	// そのベクトル元から球体へのベクトルの内積を取る
	float dot = VDot(_cap1to2ENor, VSub(_sphPos, _capTopPos1));

	// 内積で求めた射影距離を使って、カプセル繋ぎ上の座標を取る
	VECTOR _capRidePos = VAdd(_capTopPos1, VScale(_cap1to2ENor, dot));

	// カプセル繋ぎのベクトルの長さを取る
	float len = AsoUtility::MagnitudeF(_cap1to2);

	// 球体がカプセル繋ぎ上にいるか判別するため、比率を取る
	float rate = (dot / len);

	VECTOR centerPos;


	// 球体の位置が３エリアに分割されたカプセル形状のどこにいるか判別
	if (rate > 0.0f && rate <= 1.0f)
	{
		// ①球体がカプセル繋ぎ上にいる
		centerPos = VAdd(_capTopPos1, VScale(_cap1to2ENor, dot));
	}
	else if (rate > 1.0f)
	{
		// ②球体がカプセルの終点側にいる
		centerPos = _capBottomPos2;
	}
	else if (rate < 0.0f)
	{
		// ③球体がカプセルの始点側にいる
		centerPos = _capTopPos1;
	}
	else
	{
		// ここにきてはいけない
		return false;
	}

	// 球体同士の当たり判定
	if (IsHitSphareToSphere(centerPos, _sphPos, _sphRadius, _capRadius))
	{
		ret = true;
	}

	return ret;
}
bool UtilityCollision::IsHitCapsuleToCapsule(const VECTOR& _capTop1, const VECTOR& _capBottom1, float _radius1,
	                                         const VECTOR& _capTop2, const VECTOR& _capBottom2, float _radius2)
{
	bool ret = false;

	// 最短距離
	VECTOR near1, near2;

	float dis = _CapPointBitween(
		_capTop1, _capBottom1, near1,
		_capTop2, _capBottom2, near2);

	// カプセルの最短距離が半径の合計以下の時、衝突
	if (dis <= (_radius1 + _radius2))
	{
		ret = true;
	}
	return ret;
}

float UtilityCollision::_CapPointBitween(const VECTOR& _capTop1, const VECTOR& _capBottom1, VECTOR& _capCtc1,
	                                     const VECTOR& _capTop2, const VECTOR& _capBottom2, VECTOR& _capCtc2)
{
	/*　カプセル同士の当たり判定　*/


	bool ret = false;

	// カプセル１の方向ベクトル
	VECTOR _capVec1 = VSub(_capTop1, _capBottom1);

	// カプセル２の方向ベクトル
	VECTOR _capVec2 = VSub(_capTop2, _capBottom2);

	// ２つの半径の合計
	VECTOR _radius = VSub(_capTop1, _capTop2);


	// 内積
	float _capLine1 = VDot(_capVec1, _capVec1);
	float _capLine2 = VDot(_capVec2, _capVec2);
	float _capRad = VDot(_capVec2, _radius);


	float s, t;

	// 両方の線分が点の場合(計算誤差を考慮)
	if (_capLine1 <= 1e-6f && _capLine2 <= 1e-6f)
	{
		s = t = 0.0f;

		_capCtc1 = _capTop1;
		_capCtc2 = _capTop2;

		// ベクトル同士の接点同士の距離を返す
		return VSize(VSub(_capCtc1, _capCtc2));
	}

	// 線分１が点の場合(計算誤差を考慮)
	if (_capLine1 <= 1e-6f)
	{
		s = 0.0f;
		t = (_capRad / _capLine1);


		t = fmaxf(0.0f, fminf(1.0f, t));
	}

	// 線分２が点の場合(計算誤差を考慮)
	else if (_capLine2 <= 1e-6f)
	{
		t = 0.0f;
		s = -VDot(_capVec1, _radius) / _capLine1;

		t = fmaxf(0.0f, fminf(1.0f, s));
	}

	// 通常時
	else
	{
		float b = VDot(_capVec1, _capVec2);

		float denom = (_capLine1 * _capLine2) - (b * b);

		if (denom != 0)
		{
			s = fmaxf(0.0f, fminf(1.0f, (b * _capRad - VDot(_capVec1, _radius) * _capLine2)));
		}
		else
		{
			s = 0.0f;
		}

		t = (b * s + _capRad) / _capLine2;

		if (t < 0.0f)
		{
			t = 0.0f;
			s = fmaxf(0.0f, fminf(1.0f, -VDot(_capVec1, _radius) / _capLine1));
		}
		else if (t > 1.0f)
		{
			t = 1.0f;
			s = fmaxf(0.0f, fminf(1.0f, (b - VDot(_capVec1, _radius)) / _capLine1));
		}
	}

	_capCtc1 = VAdd(_capTop1, VScale(_capVec1, s));
	_capCtc2 = VAdd(_capTop2, VScale(_capVec2, t));

	return VSize(VSub(_capCtc1, _capCtc2));
}