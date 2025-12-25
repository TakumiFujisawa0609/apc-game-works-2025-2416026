#pragma once
#include "../Common/Vector2.h"
#include "./Input.h"
#include <vector>
#include <map>
#include <functional>
#include <unordered_map>
#include <DxLib.h>

class InputManager
{
public:

	enum class TYPE
	{
		SELECT_RIGHT,			// 選択右
		SELECT_LEFT,			// 選択左
		SELECT_DOWN,			// 選択下
		SELECT_UP,				// 選択上
		SELECT_DECISION,		// 選択決定
		SELECT_CANCEL,			// 選択キャンセル
		
		MAX,
	};
	/// @brief 入力状態
	enum class INPUT_TYPE
	{
		KEYBOARD_ONLY,
		KEYBOARD_MOUSE,
		CONTROLLER,
		MAX,
	};

	enum class MOUSE_MODE
	{
		NONE,     // 変更なし
		LOCKED,   // 画面中央固定
		CONFINED, // ウィンドウ内
		CONFINED_SIDE, // ウィンドウ内の反対側に行く
	};


private:

	


public:

	// アナログキーの入力受付しきい値(0.0～1.0)
	static constexpr float ALGKEY_THRESHOLD = 0.5f;

	// 十字キーの入力受付しきい値(0.0～1.0)
	static constexpr float DPAD_THRESHOLD = (1.0f * 4500.0f);

	// マウスホイール移動最大
	static constexpr int MOUSE_WHEEL_MAX = 2;


	using KeyMap    = std::unordered_map<int, Input::Trigger*>;
	using MouseMap  = std::unordered_map<int, Input::Trigger*>;


	/// @brief インスタンスを明示的に生成
	static void CreateInstance(void);

	/// @brief インスタンスの取得
	/// @return 入力マネージャ
	static InputManager& GetInstance(void) { return *instance_; };


	/// @brief 初期化処理
	void Init(void);

	/// @brief 更新処理
	void Update(void);

	/// @brief インスタンス削除
	void Destroy(void);

	bool IsNew(TYPE _type, Input::PAD_NO _padNum = Input::PAD_NO::PAD1);
	bool IsTrgDown(TYPE _type, Input::PAD_NO _padNum = Input::PAD_NO::PAD1);
	bool IsTrgUp(TYPE _type, Input::PAD_NO _padNum = Input::PAD_NO::PAD1);


	/*　キーボード入力　*/

	/// @brief 判定を行うキーを追加
	/// @param _type 追加対象
	void AddKey(unsigned int _type);

	/// @brief キーの入力判定
	/// @param _keyType 判定するキー
	/// @return 入力しているか否か
	bool KeyIsNew(unsigned int _keyType) const { return FindKey(_keyType).inputNew; };

	/// @brief キーを入力した瞬間の判定
	/// @param _keyType 判定するキー
	/// @return キー入力したか否か
	bool KeyIsTrgDown(unsigned int _keyType) const { return FindKey(_keyType).trgDown; };

	/// @brief キーを離した瞬間の判定
	/// @param _keyType 判定するキー
	/// @return キーを離したか否か
	bool KeyIsTrgUp(unsigned int _keyType) const { return FindKey(_keyType).trgUp; };

	/// @brief 全てのキーの入力判定
	bool KeyIsNewAll(void) const;


	/* マウス入力　*/

	/// @brief 判定を行うキーを追加
	/// @param _type 追加対象
	void AddMouse(unsigned int _type);

	/// @brief マウスの移動量取得
	/// @returns マウス座標
	Vector2 GetMousePos(void) const;

	/// @brief マウス入力処理
	/// @param _mouseType 判定するマウスキー
	/// @return クリックしているか否か
	bool MouseIsNew(unsigned int _mouseType) const { return FindMouse(_mouseType).inputNew; };

	/// @brief マウスクリックを入力した瞬間の判定
	/// @param _mouseType 判定するマウスキー
	/// @return クリックしたか否か
	bool MouseIsTrgDown(unsigned int _mouseType) const { return FindMouse(_mouseType).trgDown; };

	/// @brief マウスクリックを離した瞬間の判定
	/// @param _mouseType 判定するマウスクリックの種類
	/// @return クリックを離したか否か
	bool MouseIsTrgUp(unsigned int _mouseType) const { return FindMouse(_mouseType).trgUp; };

	/// <summary>
	/// マウスの回転量を取得(正の値：上(奥)方向)
	/// </summary>
	int GetMouseWheelRot(void);

	/// @brief マウスカーソル状態割り当て
	//void SetCursorMode(CURSOR_MODE mode, bool _isVisible = true);


	/*　コントローラー入力　*/

	

	/// <summary>
	/// コントローラのスティック横移動量を取得
	/// </summary>
	/// <param name="_padNum">コントローラの対象</param>
	/// <param name="_algKey">スティック番号</param>
	int PadAlgKeyX(Input::PAD_NO _padNum, Input::JOYPAD_ALGKEY _algKey)const;
	/// <summary>
	/// コントローラのスティック横移動量を取得
	/// </summary>
	/// <param name="_padNum">コントローラの対象</param>
	/// <param name="_algKey">スティック番号</param>
	int PadAlgKeyX(int _padNum, int _algKey)const;

	/// @brief コントローラのスティック縦移動量を取得
	/// @param _padNum コントローラの対象
	/// @param _algKey スティック番号
	int PadAlgKeyY(Input::PAD_NO _padNum, Input::JOYPAD_ALGKEY _algKey)const;
	/// @brief コントローラのスティック縦移動量を取得
	/// @param _padNum コントローラの対象
	/// @param _algKey スティック番号
	int PadAlgKeyY(int _padNum, int _algKey)const;

	/// @brief コントローラのスティックの傾き方向を取得(XZ平面)
	/// @param _padNum コントローラの対象
	/// @param _algKey スティック番号
	const VECTOR& GetAlgKeyDirXZ(Input::PAD_NO _padNum, Input::JOYPAD_ALGKEY _algKey)const;

	/// <summary>
	/// いずれかのコントローラの入力判定
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PadAnyInput(void)const;


private:

	struct PadInfo
	{
		
	};
	struct InputInfo
	{
		std::unordered_map<unsigned int, Input::Trigger*> keyList;
		std::unordered_map<Input::MOUSE_TYPE, Input::Trigger*> mouseList;
		std::unordered_map<Input::PAD_BTN, Input::Trigger*> buttonList;
		std::unordered_map<Input::JOYPAD_ALGKEY, Vector2> algStickList;
	};
	// シングルトンインスタンス
	static InputManager* instance_;

	// アナログキーの最大値
	static constexpr float ALGKEY_VAL_MAX = 1000.0f;

	Input::Trigger* triggerEmpty_;

	// 各入力の判定処理
	using FunctionMap = std::unordered_map<TYPE, std::vector<std::function<bool(TYPE, Input::PAD_NO)> >>;
	FunctionMap funcMapIsNew_;
	FunctionMap funcMapIsTrgUp_;
	FunctionMap funcMapIsTrgDown_;

	//void RegisterTrigger

	/*　キーボード　*/

	// キー情報配列
	KeyMap keys_;


	/*　マウス　*/

	// マウス情報配列
	MouseMap mouse_;

	// カーソル状態
	MOUSE_MODE cursorMode_;

	/*　コントローラのメンバ変数　*/


	/// @brief デフォルトコンストラクタ
	InputManager(void);

	/// @brief デフォルトデストラクタ
	~InputManager(void) = default;

	// コピーコンストラクタ対策
	//InputManager(const InputManager&) = delete;
	//InputManager& operator=(const InputManager&) = delete;
	//InputManager(InputManager&&) = delete;
	//InputManager& operator=(InputManager&&) = delete;

	// 各入力更新処理
	void UpdateKey(void);
	void UpdateMouse(void);
	void UpdatePad(void);
	void UpdateCursor(void);


	/// <summary>
	/// 配列内から対象のキーの入力を取得
	/// </summary>
	/// <param name="keyType">探索するキーの種類</param>
	/// <returns>キーの情報(定数)</returns>
	Input::Trigger& FindKey(unsigned int keyType)const;

	/// @brief 判定する入力キーを割り当て
	void SetInputKey(void);


	/*　マウス　*/

	/// <summary>
	/// 配列内から対象のマウスボタンの入力を取得
	/// </summary>
	/// <param name="keyType">探索するマウスボタンの種類</param>
	/// <returns>マウスボタンの情報(定数)</returns>
	const Input::Trigger& FindMouse(unsigned int mouseType) const;

	/// @brief 判定するマウスを割り当て
	void SetInputMouse(void);

	void SetCursorOtherSide(void);


	/*　コントローラ　*/

	/// @brief コントローラの入力情報
	/// @param _padNum コントローラの対象
	Input::Joypad& GetPadInputState(Input::PAD_NO _padNum);

	/// <summary>
	/// 接続されたコントローラを識別して取得
	/// </summary>
	/// <param name="_padNum">判定するコントローラ番号</param>
	/// <returns>コントローラ識別情報</returns>
	Input::JOYPAD_TYPE GetPadType(Input::PAD_NO _padNum);

	/// <summary>
	/// DirectInputの入力取得
	/// </summary>
	/// <param name="_padNum">判定するコントローラ番号</param>
	/// <returns>DirectInput入力情報</returns>
	DINPUT_JOYSTATE GetPadDInputState(Input::PAD_NO _padNum);

	/// <summary>
	/// コントローラボタンの入力取得
	/// </summary>
	/// <param name="_padNum">判定するコントローラ番号</param>
	/// <returns>コントローラボタン入力情報</returns>
	XINPUT_STATE GetPadXInputState(Input::PAD_NO _padNum);

	/// <summary>
	/// コントローラを識別して取得
	/// </summary>
	/// <param name="jpadNum">判定するコントローラ番号</param>
	/// <returns>コントローラ識別情報</returns>
	void SetPadInState(Input::PAD_NO jpadNum);

	/// <summary>
	/// 接続されたコントローラの識別を取得
	/// </summary>
	/// <param name="jPadNo"></param>
	Input::JOYPAD_TYPE GetJPadType(Input::PAD_NO jPadNo);

};