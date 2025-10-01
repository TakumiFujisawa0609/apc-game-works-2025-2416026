#pragma once
#include "./Object.h"
#include "../Utility/Vector2.h"
#include <DxLib.h>
#include <map>

class PlayerCollision;
class CollisionBase;
class AnimationController;
class StatusPlayer;

class Player : public Object
{
public:

	/// <summary>
	/// プレイヤー種類
	/// </summary>
	enum class PLAYER_TYPE
	{
		NONE = -1,
		PLAYER_BEAR, // くま
	};

	enum class ACTION_STATE
	{
		NONE = -1,
		IDLE,		 // 待機

		ATTACK_WAIT, // 攻撃準備
		ATTACK,		 // 攻撃
		ATTACK_END,  // 攻撃終了

		ATTACK_SPECIAL,

		GAME_OVER, // ゲームオーバー状態

	};

	enum class ANIM_STATE
	{
		NONE = -1,
		DEATH, // ゲームオーバー
		DEFEAT,
		IDLE,		 // 待機

		JUMP, // ジャンプ
		PICK_UP, // 拾う
		RECIEVE_HIT,
		ATTACK, // 攻撃
		ROLL,   // 回避
		RUN,    // 走る
		RUN_CARRY,
		SHOOT_ONEHANDED,
		SHIT_DOWN,
		STAND_UP,
		SWORD_SLASH,
		VICTORY,
		WALK, // 歩く
		WALK_CARRY,
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

		RUN,		  // ダッシュ

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
	void Init(const VECTOR& pos, float angleY = 0.0f) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void) override;

	/// <summary描画処理>
	/// 
	/// </summary>
	void Draw(void)override;

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
	/// 攻撃をできるかどうかの判定
	/// </summary>
	bool CheckActiveAttack(void) const;


	/// <summary>
	/// 行動状態
	/// </summary>
	void SetActionState(ACTION_STATE state);

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
		ACTION_STATE actionState; // 行動状態

		float parryTime; // パリィ時間
		float parryMag; // パリィ倍率

		float dodgeTime; // 回避時間
		float dodgeMag; // 回避倍率

		int luck; // 運

		int combo; // コンボ数

		int weaponId; // 武器ID

		bool isRun; // ダッシュフラグ

		std::map<ANIM_STATE, float> animSpeed;
	};
	PlayerParam paramPlayer_;

	StatusPlayer& status_;

	// プレイヤー種類
	PLAYER_TYPE type_;

	// 入力種類
	INPUT_TYPE inputType_;

	// 入力するキーの種類
	std::map<INPUT_TYPE, unsigned int> inputKey_;

	// 入力するゲームパッド識別番号
	int inputPad_;

	// ジャンプ力
	float jumpPower_;


	/// <summary>
	/// 待機更新処理
	/// </summary>
	void Update_Idle(void) override;

	/// <summary>
	/// 攻撃更新処理
	/// </summary>
	void Update_Attack(void)override;

	/// <summary>
	/// ゲームオーバー状態の更新処理
	/// </summary>
	void Update_GameOver(void)override;


	/// <summary>
	/// 移動処理
	/// </summary>
	void Move(void);

	/// <summary>
	/// アニメーション割り当て
	/// </summary>
	void SetAnim(void)override;

	/// <summary>
	/// アニメーション処理
	/// </summary>
	void Update_Animation(void);

	/// <summary>
	/// 各アニメーション遷移処理
	/// </summary>
	void AnimationState(void);

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
	bool IsInputRun(void);


	/// <summary>
	/// 行動処理
	/// </summary>
	/// <param name="waitState">行動の待機状態</param>
	/// <param name="activeState">行動の有効状態</param>
	/// <param name="activeTime">有効時間</param>
	/// <param name="endState">行動の終了状態</param>
	/// <param name="endTime">終了時間</param>
	void Action(ACTION_STATE waitState, ACTION_STATE activeState, float activeTime,
		ACTION_STATE endState, float endTime);

	/// <summary>
	/// キャラモデル割り当て処理
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	int SetCharaModel(const PLAYER_TYPE& type);

	/// <summary>
	/// 行動可能か否か判定
	/// </summary>
	bool IsActiveAction(void)const;
};