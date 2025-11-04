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

	// オイラー角からクォータニオンへ変換
	static Quaternion Euler(const VECTOR& rad);

	// オイラー角からクォータニオンへ変換
	static Quaternion Euler(double radX, double radY, double radZ);


	// クォータニオンの合成
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);

	// クォータニオンの合成
	Quaternion Mult(const Quaternion& q) const;


	// 指定軸を指定角分、回転させる
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
	/// <summary>
	/// 正面の位置取得
	/// </summary>
	VECTOR GetForward(void) const;

	/// <summary>
	/// 後ろの位置取得
	/// </summary>
	VECTOR GetBack(void) const;

	/// <summary>
	/// 左の位置取得
	/// </summary>
	VECTOR GetRight(void) const;

	/// <summary>
	/// 右の位置取得
	/// </summary>
	VECTOR GetLeft(void) const;

	/// <summary>
	/// 上の位置取得
	/// </summary>
	VECTOR GetUp(void) const;

	/// <summary>
	/// 下の位置取得
	/// </summary>
	VECTOR GetDown(void) const;
#pragma endregion

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="q1">クォータニオン１</param>
	/// <param name="q2">クォータニオン２</param>
	static double Dot(const Quaternion& q1, const Quaternion& q2);
	double Dot(const Quaternion& b) const;


	/// <summary>
	/// 対象を正規化して返す
	/// </summary>
	static Quaternion Normalize(const Quaternion& q);

	/// <summary>
	/// 自分自身を正規化して返す
	/// </summary>
	Quaternion Normalized(void) const;

	/// <summary>
	/// 自分自身を正規化
	/// </summary>
	void Normalize(void);

	/// <summary>
	/// 逆クォータニオン
	/// </summary>
	Quaternion Inverse(void) const;

	// 球面補間
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

private:

	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR dir) const;

	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);

};
