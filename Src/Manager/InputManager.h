#pragma once
#include <map>
#include <functional>
#include <vector>
#include <memory>
#include "../Common/Vector2.h"
#include "Input.h"

class InputManager
{
public:

	/// <summary>
	/// 入力状況の種類
	/// </summary>
	enum class TYPE
	{
		NONE,

		PLAYER_MOVE_RIGHT, // 移動右
		PLAYER_MOVE_LEFT,  // 移動左
		PLAYER_MOVE_BACK,  // 移動上
		PLAYER_MOVE_FRONT, // 移動下

		PLAYER_ATTACK_JUB,	  // 弱攻撃
		PLAYER_ATTACK_STRONG, // 弱攻撃
		PLAYER_DASH,   // ダッシュ

		CAMERA_MOVE_RIGHT, // カメラ右移動
		CAMERA_MOVE_LEFT,  // カメラ左移動
		CAMERA_MOVE_UP,	   // カメラ上移動
		CAMERA_MOVE_DOWN,  // カメラ下移動

		GAME_STATE_CHANGE,		// ゲーム状態遷移

		SELECT_RIGHT,			// 選択右
		SELECT_LEFT,			// 選択左
		SELECT_DOWN,			// 選択下
		SELECT_UP,				// 選択上
		SELECT_DECISION,		// 選択決定
		SELECT_CANCEL,			// 選択キャンセル

		DEBUG_SCENE_CHANGE,		// デバッグシーン遷移
		DEBUG_MODE,

		PAUSE,					// ポーズ(開閉)

		MAX,
	};

	enum class MOUSE_MODE
	{
		NONE,     // 変更なし
		LOCKED,   // 画面中央固定
		CONFINED, // ウィンドウ内
		CONFINED_SIDE, // ウィンドウ内の反対側に行く
	};


	static void CreateInstance(void);

	static InputManager& GetInstance(void) { return *instance_; };


	/// @brief 初期化処理
	void Init(void);

	/// @brief 更新処理
	void Update(void);

	/// @brief 解放処理
	void Release(void);

	/// @brief リソースの破棄
	static void Destroy(void);


	/// <summary>
	/// 指定した状況での各入力機器の押下判定
	/// </summary>
	/// <param name="type">状況の種類</param>
	/// <param name="padNo">パッド番号(デフォルトはPAD1)</param>
	/// <returns>trueの場合入力中,falseの場合未入力</returns>
	bool IsNew(const TYPE type, const Input::JOYPAD_NO padNo = Input::JOYPAD_NO::PAD1);

	/// <summary>
	/// 指定した状況での各入力機器の押下判定(最初のみ)
	/// </summary>
	/// <param name="type">状況の種類</param>
	/// <param name="padNo">パッド番号(デフォルトはPAD1)</param>
	/// <returns>trueの場合入力中,falseの場合未入力</returns>
	bool IsTrgDown(const TYPE type, const Input::JOYPAD_NO padNo = Input::JOYPAD_NO::PAD1);

	/// <summary>
	/// 指定した状況での各入力機器の離した判定
	/// </summary>
	/// <param name="type">状況の種類</param>
	/// <param name="padNo">パッド番号(デフォルトはPAD1)</param>
	/// <returns>trueの場合離したタイミング,falseの場合まだ押下中</returns>
	bool IsTrgUp(const TYPE type, const Input::JOYPAD_NO padNo = Input::JOYPAD_NO::PAD1);

	/// <summary>
	/// マウス位置を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetMousePos(const Vector2& pos);

	/// <summary>
	/// マウス座標の取得
	/// </summary>
	/// <returns>マウス座標の取得</returns>
	Vector2 GetMousePos(void) const;

	/// @brief マウスの移動量 取得
	Vector2 GetMouseMove(void) const;

	/// @brief 左スティックの倒れた度合いを取得
	/// @param _num パッド番号
	Vector2 GetKnockLStickSize(Input::JOYPAD_NO num = Input::JOYPAD_NO::PAD1) const;
	
	/// @brief 右スティックの倒れた度合いを取得
	/// @param _num パッド番号
	Vector2 GetKnockRStickSize(Input::JOYPAD_NO num = Input::JOYPAD_NO::PAD1) const;

	/// @brief 左スティックの倒れた方向を取得
	/// @param _num パッド番号
	VECTOR GetDirXZ_LStick(Input::JOYPAD_NO num = Input::JOYPAD_NO::PAD1, float _threshold = STICK_THRESHOLD) const;
	
	/// @brief 右スティックの倒れた方向を取得
	/// @param _num パッド番号
	VECTOR GetDirXZ_RStick(Input::JOYPAD_NO num = Input::JOYPAD_NO::PAD1, float _threshold = STICK_THRESHOLD) const;


private:


	static constexpr float STICK_THRESHOLD = 1000.0f;
	static constexpr float MOUSE_THRESHOLD = 100.0f;

	static InputManager* instance_;

	// カーソル状態
	MOUSE_MODE cursorMode_;


	// 入力トリガーの情報
	struct TriggerInfo
	{
		std::vector<int> keys;
		std::vector<Input::JOYPAD_BTN> padButtons;
		std::vector<Input::JOYPAD_STICK> padSticks;
		Input::MOUSE mouse = Input::MOUSE::MAX;
	};

	// 入力判定クラス
	std::unique_ptr<Input> input_;

	// 入力状況別のトリガーを管理するマップ
	std::map<TYPE, TriggerInfo> triggerMap_;

	// 入力状況に応じた押下処理を管理するマップ
	std::map<TYPE, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>>> funcNewMap_;
	std::map<TYPE, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>>> funcTrgDownMap_;
	std::map<TYPE, std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>>> funcTrgUpMap_;


	// 入力状況に応じたトリガーを登録
	void RegisterTrigger(
		const TYPE type, 
		const std::vector<int> keys,
		const std::vector<Input::JOYPAD_BTN> padButtons,
		const std::vector<Input::JOYPAD_STICK> padSticks,
		const Input::MOUSE mouse = Input::MOUSE::MAX);

	// 処理の登録
	void RegisterTriggerFunction(
		const InputManager::TYPE type, 
		std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> newFuncs,
		std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgDownFuncs,
		std::vector<std::function<bool(TYPE, Input::JOYPAD_NO)>> trgUpFuncs);

	// キー関係の入力判定
	bool IsNewKey(const TYPE type);
	bool IsTrgDownKey(const TYPE type);
	bool IsTrgUpKey(const TYPE type);

	// パッドボタンの入力判定
	bool IsNewPadButton(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgDownPadButton(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgUpPadButton(const TYPE type, const Input::JOYPAD_NO padNo);

	// パッドスティックの入力判定
	bool IsNewPadStick(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgDownPadStick(const TYPE type, const Input::JOYPAD_NO padNo);
	bool IsTrgUpPadStick(const TYPE type, const Input::JOYPAD_NO padNo);

	// マウスの入力判定
	bool IsNewMouse(const TYPE type);
	bool IsTrgDownMouse(const TYPE type);
	bool IsTrgUpMouse(const TYPE type);


	/// @brief コンストラクタ
	InputManager(void);

	/// @brief デストラクタ
	~InputManager(void) = default;

	// コピーコンストラクタ対策
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(InputManager&&) = delete;

};