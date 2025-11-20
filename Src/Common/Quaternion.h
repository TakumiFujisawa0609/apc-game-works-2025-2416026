#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>
class Quaternion
{

public:

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	Quaternion(void);
	Quaternion(const VECTOR& rad);
	Quaternion(double w, double x, double y, double z);

	~Quaternion(void) = default;

	Quaternion operator-(void) { return Quaternion(w,x,-y,z); };

	/// @brief オイラー角からクォータニオンへ変換
	static Quaternion Euler(const VECTOR& rad);

	/// @brief オイラー角からクォータニオンへ変換
	static Quaternion Euler(double radX, double radY, double radZ);


	/// @brief クォータニオンの合成
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);

	/// @brief 別のクォータニオンの合成
	Quaternion Mult(const Quaternion& q) const;


	/// @brief 指定軸を指定角分、回転させる
	/// @param rad 角度(ラジアン)
	/// @param axis 方向ベクトル
	static Quaternion AngleAxis(double rad, VECTOR axis);

	// 座標を回転させる
	static VECTOR PosAxis(const Quaternion& q, VECTOR pos);
	VECTOR PosAxis(VECTOR pos) const;

	// クォータニオンからオイラー角へ変換
	static VECTOR ToEuler(const Quaternion& q);
	VECTOR ToEuler(void) const;

	// クォータニオンから行列へ変換
	static MATRIX ToMatrix(const Quaternion& q);
	MATRIX ToMatrix(void) const;

	// ベクトルからクォータニオンに変換
	static Quaternion LookRotation(VECTOR dir);
	static Quaternion LookRotation(VECTOR dir, VECTOR up);

	// 行列からクォータニオンに変換
	static Quaternion GetRotation(MATRIX mat);

#pragma region 基本ベクトルを取得

	/// @brief 正面の位置取得
	VECTOR GetForward(void) const;

	/// @brief 後ろの位置取得
	VECTOR GetBack(void) const;

	/// @brief 左の位置取得
	VECTOR GetRight(void) const;

	/// @brief 右の位置取得
	VECTOR GetLeft(void) const;

	/// @brief 上の位置取得
	VECTOR GetUp(void) const;

	/// @brief 下の位置取得
	VECTOR GetDown(void) const;
#pragma endregion

	/// @brief 内積計算
	/// @param _q1 クォータニオン１
	/// @param _q2 クォータニオン２
	static double Dot(const Quaternion& _q1, const Quaternion& _q2);

	/// @brief 内積計算
	/// @param _q 別クォータニオン
	double Dot(const Quaternion& _q) const;


	/// @brief 対象を正規化して返す
	static Quaternion Normalize(const Quaternion& _q);

	/// @brief 自分自身を正規化して返す
	Quaternion Normalized(void) const;

	/// @brief 自分自身を正規化
	void Normalize(void);

	/// @brief 逆クォータニオン
	Quaternion Inverse(void) const;

	/// @brief 球面補間
	/// @param from 始点のクォータニオン
	/// @param to 終点のクォータニオン
	/// @param t 補間する値
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);

	// ２つのベクトル間の回転量を取得する
	static Quaternion FromToRotation(VECTOR fromDir, VECTOR toDir);
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);
	static double Angle(const Quaternion& q1, const Quaternion& q2);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion Identity(void);

	double Length(void) const;
	double LengthSquared(void) const;
	VECTOR xyz(void) const;

	// 対象方向の回転
	void ToAngleAxis(float* angle, VECTOR* axis);

	static bool EqualsQuaZero(const Quaternion& _qua1);

private:

	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR dir) const;

	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);

};
