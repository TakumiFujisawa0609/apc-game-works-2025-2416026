#pragma once
#include "../Common/Vector2.h"
#include <DxLib.h>

class UtilityCollision
{
public:

	
	/// @brief 2D矩形当たり判定
	/// @param _pos1 対象１の2点座標
	/// @param _size1 対象１のスケール
	/// @param _pos2 対象２の2点座標
	/// @param _size2 対象２のスケール
	static bool IsHitBoxToBox(const Vector2& _pos1, const Vector2& _size1, const Vector2& _pos2, const Vector2& _size2);

	/// @brief ボックス同士の当たり判定
	/// @param _pos1 対象１の中央座標
	/// @param _size1 対象１のスケール
	/// @param _pos2 対象２の中央座標
	/// @param _size2 対象２のスケール
	static bool IsHitBoxToBox(const VECTOR& _pos1, const VECTOR& _size1, const VECTOR& _pos2, const VECTOR& _size2);


	/// @brief 2D円形当たり判定
	/// @param _pos1 対象１の座標
	/// @param _radius1 対象１の半径
	/// @param _pos2 対象２の座標
	/// @param _radius2 対象２の半径
	static bool IsHitCircleCollision2D(const Vector2& _pos1, float _radius1, const Vector2& _pos2, float _radius2);

	
	/// @brief 球形同士の当たり判定
	/// @param _pos1 対象１の座標
	/// @param _radius1 対象１の半径
	/// @param _pos2 対象２の座標
	/// @param _radius2 対象２の半径
	static bool IsHitSphereToSphere(const VECTOR& _pos1, float _radius1, const VECTOR& _pos2, float _radius2);

	/// @brief 球体とカプセルの衝突判定
	/// @param _sphPos 球体の位置
	/// @param _sphRadius 球体の半径
	/// @param _capPos1 カプセルの位置１
	/// @param _capPos2 カプセルの位置２
	/// @param _capRadius カプセルの半径
	static bool IsHitSphereToCapsule(const VECTOR& _sphPos, float _sphRadius,
		                             const VECTOR& _capPos1, const VECTOR& _capPos2, float _capRadius);


	/// @brief カプセル同士の衝突判定
	/// @param _capTop1 カプセル１の上位置
	/// @param _capBottom1 カプセル１の下位置
	/// @param _radius1 カプセル１の半径
	/// @param _capTop2 カプセル２の上位置
	/// @param _capBottom2 カプセル２の下位置
	/// @param _radius2 カプセル２の半径
	static bool IsHitCapsuleToCapsule(const VECTOR& _capTop1, const VECTOR& _capBottom1, float _radius1,
		                              const VECTOR& _capTop2, const VECTOR& _capBottom2, float _radius2);

	/// @brief 横反発処理(両者を反発)
	/// @param _targetPos1 対象１の位置
	/// @param _targetRad1 対象１の半径
	/// @param _targetPos2 対象２の位置
	/// @param _targetRad2 対象２の半径
	static void CollisionsReflectXZ(VECTOR& _targetPos1, float _targetRad1, VECTOR& _targetPos2, float _targetRad2);

	/// @brief 横反発処理(反発対象のみ反発)
	/// @param _targetPos 対象の位置
	/// @param _fromRad 反発対象の半径
	/// @param _toPos 反発しない対象の位置
	/// @param _toRad 反発しないの半径
	/// @returns 反発ベクトル
	static const VECTOR& CollisionReflectXZ(float _targetPosY, const VECTOR& _fromPos, float _fromRad, const VECTOR& _toPos, float _toRad);


private:

	/// @brief カプセル同士の当たり判定
	/// @param _capTop1 カプセル１の上位置
	/// @param _capBottom1 カプセル１の下位置
	/// @param _capCtc1 カプセル１の接点
	/// @param _capTop2 カプセル２の上位置
	/// @param _capBottom2 カプセル２下位置
	/// @param _capCtc2 カプセル２の接点
	static float _CapPointBitween(const VECTOR& _capTop1, const VECTOR& _capBottom1, VECTOR& _capCtc1,
		                          const VECTOR& _capTop2, const VECTOR& _capBottom2, VECTOR& _capCtc2);
};