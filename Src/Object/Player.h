#pragma once
#include "./Object.h"
#include <DxLib.h>
#include <unordered_map>
#include "../Manager/InputManager.h"
#include "./Status/StatusPlayer.h"

class PlayerCollision;
class CollisionBase;
class AnimationController;
class StatusPlayer;

class Player : public Object
{
	using PAD_BTN = InputManager::PAD_BTN;
	using PAD_ALGKEY = InputManager::JOYPAD_ALGKEY;
	using PAD_NO = InputManager::PAD_NO;

public:

	using MOTION_TYPE = StatusPlayer::MOTION_TYPE;

	enum class ACTION_STATE
	{
		NONE = -1,
		IDLE, // 待機

		ATTACK_JUB,  // 弱攻撃状態(判定のみ)
		ATTACK_JUB_1, // 弱攻撃１回目(初回)
		ATTACK_JUB_2, // 弱攻撃２回目
		ATTACK_JUB_END, // 弱攻撃３回目

		ATTACK_SPECIAL, // 強攻撃単体(必殺技)

		ATTACK_STRONG,// 強攻撃状態(判定のみ)
		ATTACK_STRONG_1, // 弱１回 強攻撃
		ATTACK_STRONG_2, // 弱２回 強攻撃
		ATTACK_STRONG_3, // 弱３回 強攻撃

		DODGE, // 回避
		GAME_OVER, // ゲームオーバー状態
	};

	enum class ANIM_STATE
	{
		NONE = -1,
		DEATH = 0, // ゲームオーバー
		//DEFEAT,
		IDLE = 2,    // 待機
		JUMP = 3,    // ジャンプ
		PICK_UP = 4, // 拾う
		//RECIEVE_HIT,
		ATTACK = 6,  // 攻撃
		ROLL = 7,    // 回避
		DASH = 8,    // 走る
		//DASH_CARRY,
		//SHOOT_ONEHANDED,
		//SHIT_DOWN,
		//STAND_UP,
		SWORD_SLASH = 13,
		//VICTORY,
		WALK = 15, // 歩く
		//WALK_CARRY,
		MAX = 17,

		JUB_1,
		JUB_2,
		JUB_END,
		SPECIAL,
		STRONG_1,
		STRONG_2,
		STRONG_3,
	};


	enum class INPUT_TYPE
	{
		NONE = -1,
		MOVE_BACK,	// 奥移動
		MOVE_FRONT, // 前移動
		MOVE_LEFT,	// 左移動
		MOVE_RIGHT,	// 右移動

		ROTATION_LEFT,  // 左回転
		ROTATION_RIGHT,  // 左回転

		CHANGE_WEAPON, // 武器変更
		SWITCH_WEAPON_UP, // 武器切り替え
		SWITCH_WEAPON_DOWN, // 武器切り替え

		DODGE,		// 回避
		DEFENCE, 	// 防御
		ATTACK_JUB,    // 弱攻撃
		ATTACK_STRONG, // 強攻撃

		DASH,		  // ダッシュ

		MAX,
	};

	static constexpr COLOR_F COLOR_DAMAGE = { 1.0f, 0.0f, 1.0f, 1.0f };

	// 攻撃を有効前の待機時間
	static constexpr float ATTACK_TIME_WAIT = 0.3525f;

	// 攻撃の猶予時間
	static constexpr float ATTACK_TIME = 0.1f;

	// 攻撃の終了時間
	static constexpr float ATTACK_TIME_END = 0.5f;


	// 持ちあげる調整値
	static constexpr float CATCH_OFFSET = 75.0f;


	static constexpr VECTOR LOCAL_POS = { 0.0f, -100.0f, 0.0f };

	static constexpr float LOCAL_ANGLE_Y = (180.0f * (DX_PI_F / 180.0f));
	// モデルの位置調整値
	static constexpr VECTOR MODEL_OFFSET = { 0.0f, 0.0f, 0.0f };

	// モデルの大きさ
	static constexpr float MODEL_SIZE = 0.7f;


	// 通常アニメーションの再生速度
	static constexpr float ANIM_SPEED = 30.0f;

	// 攻撃アニメーション速度
	static constexpr float ANIM_ATTACK = 15.0f;

	// 移動アニメーションの再生速度
	static constexpr float ANIM_SPEED_MOVE = 40.0f;

	// ジャンプ起動アニメーションの再生速度
	static constexpr float ANIM_SPEED_JUMP_ACTIVE = 50.0f;


	/// @brief デフォルトコンストラクタ
	Player(void);

	/// @brief デフォルトデストラクタ
	~Player(void)override = default;

	/// @brief 読み込み処理
	void Load(void)override;

	/// @brief 初期化処理
	void Init(const VECTOR& pos, float angleY = 0.0f);

	/// @brief 更新処理
	void Update(void) override;

	/// @brief 描画処理
	void Draw(void)override;

	/// @brief デバッグ表示
	void DrawDebug(void);
	
	/// @brief 解放処理
	void Release(void) override;


	/// @brief パラメータ割り当て
	void SetParam(void) override;

	/// @brief 被ダメージ処理
	void SetDamage(int _damage = 1)override;


	/// @brief 行動状態
	void ChangeActionState(ACTION_STATE state);

	/// @breif パリィ増加倍率割り当て
	void SetParryMag(float mag) { paramPlayer_.parryMag = mag; };

	/// @brief パリィ時間割り当て
	void SetParryTime(float time) { paramPlayer_.parryTime = time; };

	void SetDodgeMag(float mag) { paramPlayer_.dodgeMag = mag; };

	void SetDodgeTime(float time) { paramPlayer_.dodgeTime = time; };

	void SetLuck(int luck) { paramPlayer_.luck = luck; };

	void SetCombo(int combo) { paramPlayer_.combo = combo; };

	void SetWeaponId(int id) { paramPlayer_.weaponId = id; };


	ACTION_STATE GetActionType(void) { return paramPlayer_.actionState; };

	float GetParryMag(void) { return paramPlayer_.parryMag; };

	float GetParryTime(void) { return paramPlayer_.parryTime; };

	float GetDodgeMag(void) { return paramPlayer_.dodgeMag; };

	float GetDodgeTime(void) { return paramPlayer_.dodgeTime; };

	int GetLuck(void) { return paramPlayer_.luck; };

	int GetCombo(void) { return paramPlayer_.combo; };

	int GetWeaponId(void) { return paramPlayer_.weaponId; };

	/// @brief 攻撃範囲取得
	/// @param _motion モーションの種類
	float GetRadiusAttack(int _motion)const { return status_.GetMotionRadius(_motion); };

	int GetMotionType(void) { return motionType_; };


protected:
	
	struct ParamPlayer
	{
		// 行動状態
		ACTION_STATE actionState;

		// 弱攻撃回数
		int jubCnt;

		// パリィ時間
		float parryTime;

		// パリィ倍率
		float parryMag;

		// 回避時間
		float dodgeTime;

		// 回避倍率
		float dodgeMag;

		// 運
		int luck;

		// コンボ数
		int combo;

		// 武器ID
		int weaponId;

		// ダッシュ時の増加場率
		float dashMult;

		// ダッシュフラグ
		bool isDash;

		std::unordered_map<ANIM_STATE, float> animSpeed;
	};
	ParamPlayer paramPlayer_;

	// 現在のモーション
	int motionType_;

	StatusPlayer& status_;

	// 入力種類
	INPUT_TYPE inputType_;

	// 入力するキーの種類
	std::unordered_map<INPUT_TYPE, unsigned int> inputKey_;

	

	/// @brief 状態更新処理
	void UpdateActionState(void);

	/// @brief 待機更新処理
	void UpdateStateIdle(void);

	/// @brief 攻撃更新処理
	void UpdateStateAtk(void);

	/// @brief ゲームオーバー状態の更新処理|
	void UpdateStateOver(void);


	/// @brief 移動処理
	void Move(void);

	/// <summary>
	/// ダッシュ処理
	/// </summary>
	/// <param name="_acc">加速度</param>
	/// <param name="_max">最大速度</param>
	void DashProc(float& _acc, float& _max);

	/// @brief モデルのフレーム初期化
	void InitModelFrame(void)override;

	/// @brief アニメーション割り当て
	void InitAnim(void)override;

	/// @briefアニメーション処理
	void UpdateAnim(void)override;


private:

	/// @brief待機状態のアニメーション遷移処理
	void AnimStateIdle(void);

	/// @brief モーション処理
	void UpdateMotion(void);

	void SetPosForward(void)override;

	/// @brief 外部アニメーション割り当て処理
	/// @param _state アニメーション種類
	/// @param _motion モーション種類
	/// @param _res ハンドルID
	void SetExternalAnim(ANIM_STATE _state, MOTION_TYPE _motion, int _res);

	bool IsActionJub(void);

	bool IsActionStrong(void);

	bool IsActionAttack(void);

	/// @brief行動可能か否か判定
	bool IsActiveAction(void)const;

	/// @brief 移動入力をしているか否か
	bool IsInputMove(void);

	/// @brief弱攻撃入力をしているか否か
	bool IsInputAtkJub(void);

	/// @brief 攻撃入力をしているか否か
	bool IsInputAtkStrong(void);

	/// @brief ダッシュ入力をしているか否か
	bool IsInputDash(void);

	/// @brief 再生するSEの種類を取得
	int GetSoundSrc(void);
};