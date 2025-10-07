#pragma once
#include "../Common/Vector2.h"
#include <DxLib.h>

class UtilityCollision
{
public:

	/// <summary>
	/// 2D矩形当たり判定
	/// </summary>
	/// <param name="_pos1">対象１の2点座標</param>
	/// <param name="_pos2">対象２の2点座標</param>
	/// <param name="_size1">対象１のスケール</param>
	/// <param name="_size2">対象２のスケール</param>
	static bool IsHitBoxToBox(const Vector2& _pos1, const Vector2& _pos2, const Vector2& _size1, const Vector2& _size2);
	/// <summary>
	/// ボックス同士の当たり判定
	/// </summary>
	/// <param name="_pos1">対象１の中央座標</param>
	/// <param name="_pos2">対象２の中央座標</param>
	/// <param name="_size1">対象１のスケール</param>
	/// <param name="_size2">対象２のスケール</param>
	bool IsHitBoxToBox(const VECTOR& _pos1, const VECTOR& _pos2, const VECTOR& _size1, const VECTOR& _size2);


	/// <summary>
	/// 2D円形当たり判定
	/// </summary>
	/// <param name="_pos1">対象１の座標</param>
	/// <param name="_pos2">対象２の座標</param>
	/// <param name="_radius1">対象１の半径</param>
	/// <param name="_radius2">対象２の半径</param>
	static bool IsHitCircleCollision2D(const Vector2& _pos1, const Vector2& _pos2, float _radius1, float _radius2);

	/// <summary>
	/// 球形同士の当たり判定
	/// </summary>
	/// <param name="_pos1">対象１の座標</param>
	/// <param name="_pos2">対象２の座標</param>
	/// <param name="_radius1">対象１の半径</param>
	/// <param name="_radius2">対象２の半径</param>
	static bool IsHitSphareToSphere(const VECTOR& _pos1, const VECTOR& _pos2, float _radius1, float _radius2);

	/// <summary>
	/// 球体とカプセルの衝突判定
	/// </summary>
	/// <param name="_sphPos">球体の位置</param>
	/// <param name="_sphRadius">球体の半径</param>
	/// <param name="_capPos1">カプセルの位置１</param>
	/// <param name="_capPos2">カプセルの位置２</param>
	/// <param name="_capRadius">カプセルの半径</param>
	static bool IsHitSphereToCapsule(const VECTOR& _sphPos, float _sphRadius,
		                             const VECTOR& _capPos1, const VECTOR& _capPos2, float _capRadius);


	/// <summary>
	/// カプセル同士の衝突判定
	/// </summary>
	/// <param name="_capTop1">カプセル１の上位置</param>
	/// <param name="_capBottom1">カプセル１の下位置</param>
	/// <param name="_radius1">カプセル１の半径</param>
	/// <param name="_capPos1">カプセル２の上位置</param>
	/// <param name="_capPos2">カプセル２の下位置</param>
	/// <param name="_capRadius">カプセル２の半径</param>
	static bool IsHitCapsuleToCapsule(const VECTOR& _capTop1, const VECTOR& _capBottom1, float _radius1,
		                              const VECTOR& _capTop2, const VECTOR& _capBottom2, float _radius2);


private:

	/// <summary>
	/// カプセル同士の当たり判定
	/// </summary>
	/// <param name="_capPosTop1">カプセル１の上位置</param>
	/// <param name="_capPosBot1">カプセル１の下位置</param>
	/// <param name="_capCtc1">カプセル１の接点</param>
	/// <param name="_capPosTop2">カプセル２の上位置</param>
	/// <param name="_capPosBot2">カプセル２下位置</param>
	/// <param name="_capCtc2">カプセル２の接点</param>
	static float _CapPointBitween(const VECTOR& _capTop1, const VECTOR& _capBottom1, VECTOR& _capCtc1,
		const VECTOR& _capTop2, const VECTOR& _capBottom2, VECTOR& _capCtc2);
};