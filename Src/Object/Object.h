#pragma once

#include "../Common/Quaternion.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "./Common/AnimationController.h"
#include "./Common/AttackMotion.h"
#include <DxLib.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class StatusData;
class AnimationController;

class Object
{
	// リソース
	using RES_SRC = ResourceManager::SRC;


public:

	enum class COLLISION_TYPE
	{
		HEAD,
		BODY,
		BOTTOM,
		HAND_L,
		HAND_R,
		FORWARD,

		MAX,
	};


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

	struct Frame
	{
		// フレーム名
		std::string name;

		// フレーム番号
		int num;

		// ワールド座標
		VECTOR pos;

		MATRIX localMat;

		// 表示フラグ
		bool isVisible;
	};

	/// @brief プレイヤーのパラメータ
	struct ParamChara
	{
		// 位置
		VECTOR pos;

		// 前フレームの位置
		VECTOR prePos;

		// つかみ位置
		VECTOR posChatch;

		// 正面の位置
		VECTOR posForward;

		// モデル位置調整値
		VECTOR posLocal;


		// 移動量
		VECTOR velocity;

		// 吹っ飛ばし量
		VECTOR knockBack;

		// 移動方向
		VECTOR dir;

		// 回転(オイラー角)
		VECTOR rot;

		// 回転(クォータニオン)
		Quaternion quaRot;

		// ローカル回転(クォータニオン)
		Quaternion quaRotLocal;

		// スケール
		VECTOR scale;

		// 半径
		float radius;

		// 正面の半径
		float radiusForward;

		// ハンドルID
		int handle;

		// フレームのリスト
		std::vector<Frame> frames;

		std::unordered_map<COLLISION_TYPE, Frame*> colList;

		// HP
		int hp;

		// 攻撃力
		int power;

		// 移動速度
		float speed;

		// 移動時の加速度
		float speedAcc;


		// 地面にいるか否か
		bool isGround;

		bool isActive;
		
		// 無敵時間
		float timeInv;

		// 攻撃モーション処理
		AttackMotion atkMotion;
	};
	ParamChara paramChara_;

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

	/// @brief 回転処理
	virtual void Rotation(bool isRevert = false);

	/// @brief 重力加算処理
	void GravityKnock(void);

	/// <summary>
	/// 移動量減少処理
	/// </summary>
	/// <param name="acc"></param>
	/// <returns></returns>
	float DecVelocityXZ(const float* acc);


	/// @brief パラメータ割り当て
	virtual void SetParam(void) = 0;

	/// @brief 各オブジェクトの読み込み処理
	virtual void LoadPost(void) = 0;

	/// @brief アニメーション初期化
	virtual void InitAnim(void) {};

	/// @brief フレーム初期化
	virtual void InitModelFrame(void);

	/// @brief その他処理初期化 
	virtual void InitPost(void) = 0;

	/// @brief アニメーション更新処理
	virtual void UpdateAnim(void) = 0;

	// @brief サブクラス独自更新処理
	virtual void UpdatePost(void) = 0;

	/// @brief 各オブジェクト独自の描画
	virtual void DrawPost(void) {};

	virtual void SetPosForward(void);
	
	/// @brief フレーム番号を検索
	/// @param _name フレーム名
	int FindFrameNum(const std::string& _name);

	/// @brief フレーム更新する時の条件
	virtual bool IsUpdateFrame(void) = 0;

	// ダメージ時の処理
	virtual void DamageProc(void) = 0;

	/// @brief 各オブジェクトのメモリ解放
	virtual void ReleasePost(void) {};

public:


	/// @brief デフォルトコンストラクタ
	Object(void);

	/// @brief デフォルトデストラクタ
	virtual ~Object(void) = default;

	void Load(void);

	void Init(const VECTOR& _pos, float _angleY = 0.0f);

	/// @brief 更新処理
	virtual void Update(void);

	/// @brief 描画処理
	virtual void Draw(void);

	/// @brief デバッグ描画
	virtual void DrawDebug(void) {};

	/// @brief 解放処理
	void Release(void);


	/// @brief 行列割り当て処理
	void SetMatrixModel(void);


	/// @brief 当たり判定フレーム全更新
	void UpdateModelFrames(void);

	/// @brief フレーム更新処理
	void UpdateModelFrame(COLLISION_TYPE _type);
	
	/// @brief XZ軸を戻す処理
	/// @param _pos 戻す位置
	/// @param _power 戻す力
	void RevertPosXZ(const VECTOR& _pos, float _power = 1.0f);

	/// <summary>
	/// Y軸を戻す処理
	/// </summary>
	/// <param name="_pos">戻すY軸位置</param>
	/// <param name="_isVeloReset">加速度を初期化するか否か</param>
	void RevertPosY(float _revPos, bool _isVeloReset = false);

	/// @brief 被ダメージ処理\
	/// @param _damage ダメージ量
	void SetDamage(int _damage = 1);

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
	/// <param name="_knockDir">吹っ飛ばし方向</param>
	/// <param name="invTime">無敵時間</param>
	/// <param name="powerY">上方向の吹っ飛ばし</param>
	/// <param name="powerXZ">横方向の吹っ飛ばし力</param>
	/// <param name="minPowerXZ">最低限の横方向の吹っ飛ばし力</param>
	void KnockBack(const VECTOR& _knockDir, float invTime,
		float powerY, float powerXZ = 1.0f, float minPowerXZ = 1.0f);
	
	/// @brief 攻撃をできるかどうかの判定
	bool CheckActiveAttack(void) const;


	/// @brief 位置割り当て
	void SetPos(const VECTOR& pos) { paramChara_.pos = pos; };

	/// @brief 前位置を割り当て
	void SetPrePos(const VECTOR& pos) { paramChara_.prePos = pos; };

	/// @brief 加速度を割り当て
	void SetVelocity(const VECTOR& velo) { paramChara_.velocity = velo; };

	/// <summary>
	/// 地面判定を割り当て
	/// </summary>
	/// <param name="flag">着地している否か</param>
	void SetIsGround(bool flag) { if (paramChara_.isGround != flag) { paramChara_.isGround = flag; } };

	void SetIsActive(bool flag) { paramChara_.isActive = flag; };


	/// @brief 現在位置取得
	VECTOR& GetPos(void) { return paramChara_.pos; };

	/// @brief 前フレーム位置取得
	VECTOR& GetPrePos(void) { return paramChara_.prePos; };

	/// @brief ローカル座標取得
	const VECTOR& GetPosLocal(void) const { return paramChara_.posLocal; };

	/// @brief 持ちあげ位置取得
	VECTOR& GetPosChatch(void);

	/// @brief 正面の座標取得
	VECTOR& GetPosForward(void) { return paramChara_.posForward; };

	/// @brief 加速度取得
	const VECTOR& GetVelocity(void) const { return paramChara_.velocity; };

	/// @brief 移動量取得
	const VECTOR& GetPosDelta(void) const;

	/// @brief 向きベクトル取得
	const VECTOR& GetDir(void) const { return paramChara_.dir; };


	/// @brief オイラー角の向き取得
	const VECTOR& GetRotationEuler(void) const { return paramChara_.rot; };

	/// @brief クォータニオン角の向き取得
	Quaternion& GetRotation(void) { return paramChara_.quaRot; };

	/// @brief ローカル座標のクォータニオン角の向き取得
	const Quaternion& GetRotationLocal(void) const { return paramChara_.quaRotLocal; };


	/// @brief スケール取得
	const VECTOR& GetScale(void) const { return paramChara_.scale; };

	/// @brief 半径取得
	/// @param 当たり判定の種類
	float GetRadius(COLLISION_TYPE _type);

	virtual float GetRadiusForward(void)const { return paramChara_.radiusForward; };
	

	/// @brief キャラモデルハンドル取得
	int GetHandle(void) const { return paramChara_.handle; };


	/// @brief 現在HP取得
	int GetCurHp(void) const { return paramChara_.hp; };

	/// @brief 攻撃力取得
	int GetPower(void) const { return paramChara_.power; };

	/// @brief 移動速度取得
	float GetSpeed(void) const { return paramChara_.speed; };

	/// @brief 無敵時間取得
	float GetTimeInv(void) const { return paramChara_.timeInv; };

	/// @brief 地面にあるか否か
	bool GetIsGround(void) const { return paramChara_.isGround; };

	bool GetIsActive(void) const { return paramChara_.isActive; };

	const VECTOR& GetKnockVelo(void) { return paramChara_.knockBack; };

	bool GetIsAnimEnd(void)const { return anim_->IsEnd(); };

	/// @brief 攻撃有効する猶予時間取得 
	const AttackMotion& GetActionMotion(void) const { return paramChara_.atkMotion; };

	/// @brief フレームの座標取得
	/// @param _type 当たり判定の種類
	const VECTOR& GetFramePos(COLLISION_TYPE _type);
};