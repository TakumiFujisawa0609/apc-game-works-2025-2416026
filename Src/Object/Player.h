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
	using PAD_NO = InputManager::JOYPAD_NO;

	using MORTION_TYPE = StatusPlayer::MORTION_TYPE;

public:


	enum class ACTION_STATE
	{
		NONE = -1,
		IDLE, // 待機

		ATTACK_JUB_1, // 弱攻撃１回目(初回)
		ATTACK_JUB_2, // 弱攻撃２回目
		ATTACK_JUB_3, // 弱攻撃３回目

		ATTACK_SPECIAL, // 強攻撃単体(必殺技)
		ATTACK_STRONG_1, // 弱１回 強攻撃
		ATTACK_STRONG_2, // 弱２回 強攻撃
		ATTACK_STRONG_3, // 弱３回 強攻撃

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
		MAX,
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


	// デフォルトコンストラクタ
	Player(void);

	// デフォルトデストラクタ
	~Player(void)override = default;

	/// <summary>
	/// 
	/// </summary>
	void Load(void)override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(const VECTOR& pos, float angleY = 0.0f);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void) override;


	/// <summary>
	/// パラメータ割り当て処理
	/// </summary>
	void SetParam(void) override;

	/// <summary>
	/// 移動入力をしているか否か
	/// </summary>
	bool IsInputMove(void);


	/// <summary>
	/// 攻撃処理
	/// </summary>
	/// <param name="flag"></param>
	void SetIsAttack(bool flag);


	/// <summary>
	/// 行動状態
	/// </summary>
	void ChangeActionState(ACTION_STATE state);

	/// <summary>
	/// パリィ増加倍率割り当て
	/// </summary>
	void SetParryMag(float mag) { paramPlayer_.parryMag = mag; };

	/// <summary>
	/// パリィ時間割り当て
	/// </summary>
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


protected:

	struct PlayerParam
	{
		// 行動状態
		ACTION_STATE actionState;

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
	PlayerParam paramPlayer_;


	StatusPlayer& status_;

	// 入力種類
	INPUT_TYPE inputType_;

	// 入力するキーの種類
	std::unordered_map<INPUT_TYPE, unsigned int> inputKey_;


	// ジャンプ力
	float jumpPower_;

	/// <summary>
	/// 状態更新処理
	/// </summary>
	void UpdateActionState(void);

	/// <summary>
	/// 待機更新処理
	/// </summary>
	void UpdateStateIdle(void);

	/// <summary>
	/// 攻撃更新処理
	/// </summary>
	void UpdateStateAtk(void);

	/// <summary>
	/// ゲームオーバー状態の更新処理
	/// </summary>
	void UpdateStateOver(void);


	/// <summary>
	/// 移動処理
	/// </summary>
	void Move(void);

	/// <summary>
	/// ダッシュ処理
	/// </summary>
	/// <param name="_acc">加速度</param>
	/// <param name="_max">最大速度</param>
	void DashProc(float& _acc, float& _max);

	/// <summary>
	/// アニメーション割り当て
	/// </summary>
	void InitAnim(void)override;

	/// <summary>
	/// アニメーション処理
	/// </summary>
	void UpdateAnim(void)override;

	/// <summary>
	/// 待機状態のアニメーション遷移処理
	/// </summary>
	void AnimStateIdle(void);

	/// <summary>
	/// 攻撃入力をしているか否か
	/// </summary>
	bool IsInputAtkStrong(void);

	/// <summary>
	/// 弱攻撃入力をしているか否か
	/// </summary>
	bool IsInputAtkJub(void);

	/// <summary>
	/// ダッシュ入力をしているか否か
	/// </summary>
	bool IsInputDash(void);


	/// <summary>
	/// モーション処理
	/// </summary>
	/// <param name="_type">モーションの種類</param>
	void UpdateMortion(MORTION_TYPE _type);

	/// <summary>
	/// 行動可能か否か判定
	/// </summary>
	bool IsActiveAction(void)const;
};