#pragma once

#include "../Utility/Quaternion.h"
#include <DxLib.h>
#include <memory>
#include <map>
#include <string>

class StatusData;
class AnimationController;

class Object
{
public:

	// 行動状態

	// 殴る最低限の横吹っ飛ばし量
	static constexpr float KNOCK_PUNCH_XZ_MIN = 5.0f;

	// 殴る横吹っ飛ばし倍率
	static constexpr float KNOCK_PUNCH_XZ = 3.0f;

	// 殴る縦吹っ飛ばし量
	static constexpr float KNOCK_PUNCH_Y = (7.25f * KNOCK_PUNCH_XZ);


	// 投げる最低限の横吹っ飛ばし量
	static constexpr float KNOCK_THROW_XZ_MIN = 10.0f;

	// 投げる横吹っ飛ばし倍率
	static constexpr float KNOCK_THROW_XZ = 25.0f;

	// 投げる縦吹っ飛ばし量
	static constexpr float KNOCK_THROW_Y = 15.0f;


	// ゲームオーバー横吹っ飛ばし量
	static constexpr float KNOCK_GAMEOVER_XZ = 20.0f;

	// ゲームオーバー縦吹っ飛ばし量
	static constexpr float KNOCK_GAMEOVER_Y = 30.0f;

	// つかまれ状態の最低時間
	static constexpr float CAUGHT_TIME = 0.5f;

	// つかまれ状態の重量変化する時間
	static constexpr float WEIGHT_CAUGHT_TIME = 5.0f;


	// 殴る状態の無敵時間
	static constexpr float TIME_INV_PUNCH = 0.3f;

	// 投げられ状態の無敵時間
	static constexpr float TIME_INV_THROWN = 0.2f;

	// ゲームオーバー状態の無敵時間
	static constexpr float TIME_INV_GAMEOVER = 1.0f;


	// 殴るの重量変化での最大増加量
	static constexpr float WEIGHT_POW_PUNCH_XZ = 7.5f;
	static constexpr float WEIGHT_POW_PUNCH_Y = 6.0f;

	// 吹っ飛ばしの重量変化での最大増加量
	static constexpr float WEIGHT_POW_THROW_XZ = 15.0f;
	static constexpr float WEIGHT_POW_THROW_Y = 5.0f;

	// 投げられ時の重量変化での最大増加値
	static constexpr float WEIGHT_INV_THROW = 2.25f;


	// パンチ時のヒットストップ時間
	static constexpr float HIT_STOP_PUNCH = 0.05f;

	// 投げ時のヒットストップ時間
	static constexpr float HIT_STOP_THROWN = 0.085f;


protected:

	/// <summary>
	/// プレイヤーのパラメータ
	/// </summary>
	struct CHARA_PARAM
	{
		VECTOR pos;		  // 位置
		VECTOR prePos;	  // 前フレームの位置
		VECTOR posChatch; // つかみ位置


		VECTOR velocity;  // 移動量
		VECTOR knockBack; // 吹っ飛ばし量
		VECTOR dir;       // 移動方向

		VECTOR rot;				// 回転(オイラー角)
		Quaternion quaRot;		// 回転(クォータニオン)
		Quaternion quaRotLocal; // ローカル回転(クォータニオン)

		VECTOR scale;		// スケール
		VECTOR modelOffset; // モデル位置調整値

		int handle;   // ハンドルID
		int frameMax; // メッシュ数
		std::map<int, std::string> frameNameIndex; // メッシュ描画リスト

		int hp;	      // HP

		int power;   // 攻撃力

		float speed;  // 移動速度

		float speedAcc;  // 移動時の加速度

		float timeAct; // 攻撃時間

		bool isGround; // 地面にいるか否か

		float timeInv; // 無敵時間

		COLOR_F damageColor; // ダメージ時の色
	};
	CHARA_PARAM paramChara_;

	// アニメーション
	AnimationController* anim_;



	// 開始重量
	static constexpr float WEIGHT_START = 1.0f;

	// 移動の重量変化する倍率
	static constexpr float WEIGHT_POW_MOVE = 20.0f;

	// ジャンプの重量変化する倍率
	static constexpr float WEIGHT_POW_JUMP = 15.0f;

	// 重力加算の重量変化する倍率
	static constexpr float WEIGHT_POW_GRAVITY = -1.0f;


	// 重量の変化する間隔
	static constexpr float WEIGHT_DELTA = 0.01f;

	// Y軸の加速度の下限値
	static constexpr float VELOCITY_Y_MIN = -7.5f;


	// 初期ジャンプ力
	static constexpr float START_JUMP_POWER = 12.5f;

	// 捕まれキャンセル時のジャンプ力
	static constexpr float JUMP_CAUGHT = 15.0f;

	// 加速度上昇値
	static constexpr float SPEED_ACC_POWER = 6.5f;

	// 移動量減少値
	static constexpr float MOVE_DEC_POWER = -2.0f;


	// ダメージ色有効間隔
	static constexpr int COLOR_TEAM = 2;


	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="_curVelo">現在の加速させるベクトル</param>
	/// <param name="_movePow">加算量</param>
	/// <param name="_maxVelo">最大加算量限度</param>
	/// <returns>加算後の値</returns>
	float _Move(const float* _curVelo, float _movePow, float _maxVelo);

	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation(bool isRevert = false);

	/// <summary>
	/// 重力加算処理
	/// </summary>
	void Gravity(void);

	/// <summary>
	/// 移動量減少処理
	/// </summary>
	/// <param name="acc"></param>
	/// <returns></returns>
	float DecVelocityXZ(const float* acc);

	/// <summary>
	/// アニメーション割り当て処理
	/// </summary>
	virtual void SetAnim(void) = 0;


public:


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	Object(void);

	/// <summary>
	/// 通常のデストラクタ
	/// </summary>
	virtual ~Object(void) = default; // defaultさん やっておしまいなさい

	virtual void Load(void) = 0;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">位置</param>
	virtual void Init(const VECTOR& pos,float angleY = 0.0f) = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	virtual void Release(void) {};


	/// <summary>
	/// 待機更新処理
	/// </summary>
	virtual void Update_Idle(void);

	/// <summary>
	/// 攻撃更新処理
	/// </summary>
	virtual void Update_Attack(void);

	/// <summary>
	/// ゲームオーバー状態の更新処理
	/// </summary>
	virtual void Update_GameOver(void);


	/// <summary>
	/// パラメータ割り当て
	/// </summary>
	virtual void SetParam(void) = 0;


	/// <summary>
	/// 行列割り当て処理
	/// </summary>
	void SetMatrixModel(void);

	/// <summary>
	/// XZ軸を戻す処理
	/// </summary>
	/// <param name="pos">戻す位置</param>
	/// <param name="power">戻す力</param>
	void RevertPosXZ(const VECTOR& pos, float power = 1.0f);

	/// <summary>
	/// Y軸を戻す処理
	/// </summary>
	/// <param name="pos">戻すY軸位置</param>
	/// <param name="isVeloReset">加速度を初期化するか否か</param>
	void RevertPosY(float revPos, bool isVeloReset = false);

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void SetDamage(int damage = 1);

	/// <summary>
	/// 重量の計算
	/// </summary>
	/// <param name = "weight">重力</param>
	/// <param name = "weightPower">移動量</param>
	/// <param name = "velocity">移動量</param>
	static float WeightCalc(float weight, float weightPower = 1.0f, float velocity = 0.0f);

	/// <summary>
	/// 吹っ飛ばし処理
	/// </summary>
	/// <param name="targetPos">吹っ飛ばしてくる相手の位置</param>
	/// <param name="invTime">無敵時間</param>
	/// <param name="powerY">上方向の吹っ飛ばし</param>
	/// <param name="powerXZ">横方向の吹っ飛ばし力</param>
	/// <param name="minPowerXZ">最低限の横方向の吹っ飛ばし力</param>
	void KnockBack(const VECTOR& targetPos, float invTime,
		float powerY, float powerXZ = 1.0f, float minPowerXZ = 1.0f);



	/// <summary>
	/// 位置割り当て
	/// </summary>
	/// <param name="pos">移動後の位置</param>
	void SetPos(const VECTOR& pos) { paramChara_.pos = pos; };

	/// <summary>
	/// 前位置を割り当て
	/// </summary>
	/// <param name="pos">移動後の位置</param>
	void SetPrePos(const VECTOR& pos) { paramChara_.prePos = pos; };

	/// <summary>
	/// 加速度を割り当て
	/// </summary>
	/// <param name="velo">加速度</param>
	void SetVelocity(const VECTOR& velo) { paramChara_.velocity = velo;  };

	/// <summary>
	/// 地面判定を割り当て
	/// </summary>
	/// <param name="flag">着地している否か</param>
	void SetIsGround(bool flag) { if (paramChara_.isGround != flag) { paramChara_.isGround = flag; } };

	/// <summary>
	/// 現在位置取得
	/// </summary>
	VECTOR& GetPos(void) { return paramChara_.pos; };

	/// <summary>
	/// 前フレーム位置取得
	/// </summary>
	VECTOR& GetPrePos(void) { return paramChara_.prePos; };

	/// <summary>
	/// 持ちあげ位置取得
	/// </summary>
	VECTOR& GetPosChatch(void);


	/// <summary>
	/// 加速度取得
	/// </summary>
	const VECTOR& GetVelocity(void) const { return paramChara_.velocity; };

	/// <summary>
	/// 移動量取得
	/// </summary>
	const VECTOR& GetPosDelta(void) const;

	/// <summary>
	/// 向きベクトル取得
	/// </summary>
	const VECTOR& GetDir(void) const { return paramChara_.dir; };


	/// <summary>
	/// オイラー角の向き取得
	/// </summary>
	const VECTOR& GetRotationEuler(void) const { return paramChara_.rot; };

	/// <summary>
	/// クォータニオン角の向き取得
	/// </summary>
	Quaternion& GetRotation(void) { return paramChara_.quaRot; };

	/// <summary>
	/// ローカル座標のクォータニオン角の向き取得
	/// </summary>
	const Quaternion& GetRotationLocal(void) const { return paramChara_.quaRotLocal; };


	/// <summary>
	/// スケール取得
	/// </summary>
	const VECTOR& GetScale(void) const;

	/// <summary>
	/// モデル調整値取得
	/// </summary>
	const VECTOR& GetModelOffset(void) const;

	/// <summary>
	/// キャラモデルハンドル取得
	/// </summary>
	int GetHandle(void) const { return paramChara_.handle; };


	/// <summary>
	/// 現在HP取得
	/// </summary>
	int GetCurHp(void) const { return paramChara_.hp; };

	/// <summary>
	/// 攻撃力取得
	/// </summary>
	int GetPower(void) const { return paramChara_.power; };

	/// <summary>
	/// 移動速度取得
	/// </summary>
	float GetSpeed(void) const { return paramChara_.speed; };

	/// <summary>
	/// 無敵時間取得
	/// </summary>
	float GetTimeInv(void) const { return paramChara_.timeInv; };

	/// <summary>
	/// 地面にあるか否か
	/// </summary>
	bool GetIsGround(void) const { return paramChara_.isGround; };


	/// <summary>
	/// 攻撃有効する猶予時間取得
	/// </summary>
	float GetActionTime(void) const { return paramChara_.timeAct; };
};