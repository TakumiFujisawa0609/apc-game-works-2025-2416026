#pragma once

class Input
{
public:

	/// @brief コントローラー認識番号
	enum class PAD_NO
	{
		NONE,
		PAD1,             // パッド１入力
		PAD2,             // パッド２入力
		PAD3,             // パッド３入力
		PAD4,             // パッド４入力
		INPUT_KEY = 4096, // キー入力
	};

	/// @brief ゲームコントローラータイプ
	enum class JOYPAD_TYPE
	{
		OTHER = 0,		  // その他
		XBOX_360,		  // XBOX 360
		XBOX_ONE,		  // XBOX ONE
		DUAL_SHOCK_4,
		DUAL_SENSE,		  // PlayStationコントローラ
		SWITCH_JOY_CON_L, // スイッチの右コントローラー
		SWITCH_JOY_CON_R, // スイッチの左コントローラー
		SWITCH_PRO_CTRL,  // スイッチプロコン
		MAX
	};

	enum class MOUSE_TYPE
	{
		CLICK_LEFT,   // 左クリック
		CLICK_RIGHT,  // 右クリック
		CLICK_MIDDLE, // 中央クリック

		WHEEL_FRONT, // ホイール前に回転
		WHEEL_BACK,  // ホイール奥に回転

		MOVE_LEFT,  // 左移動
		MOVE_RIGHT, // 右移動
		MOVE_FRONT, // 前移動
		MOVE_BACK,  // 奥移動

		MAX
	};

	/// @brief ゲームコントローラーボタン
	enum class PAD_BTN
	{
		NONE = - 1,
		RB_DOWN,   // A
		RB_RIGHT,  // B
		RB_LEFT,   // X
		RB_UP,     // Y

		L_BUTTON,  // L
		R_BUTTON,  // R

		BACK,      // BACK
		START,     // START
		L_TRIGGER, // 左Trigger
		R_TRIGGER, // 右Trigger
		L_STICK, // Lスティック押し込み
		R_STICK, // Rスティック押し込み


		MAX	// 要素数
	};

	/// @brief ゲームコントローラスティック
	enum class JOYPAD_ALGKEY
	{
		LEFT_UP,
		LEFT_DOWN,
		LEFT_LEFT,
		LEFT_RIGHT,

		RIGHT_UP,
		RIGHT_DOWN,
		RIGHT_LEFT,
		RIGHT_RIGHT,

		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,

		MAX
	};

	Input(void);

	~Input(void) = default;

	
	/// <summary>
	/// コントローラの入力判定
	/// </summary>
	/// <param name="_padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnNew(PAD_NO _padNum, PAD_BTN button) const;
	/// <summary>
	/// コントローラの入力判定
	/// </summary>
	/// <param name="_padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnNew(int _padNum, int button) const;

	/// <summary>
	/// コントローラのキーを押したか判定
	/// </summary>
	/// <param name="_padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnTrgDown(PAD_NO _padNum, PAD_BTN button) const;
	/// <summary>
	/// コントローラのキーを押したか判定
	/// </summary>
	/// <param name="_padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnTrgDown(int _padNum, int button) const;

	/// <summary>
	/// コントローラのキーを離したか判定
	/// </summary>
	/// <param name="_padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnTrgUp(PAD_NO _padNum, PAD_BTN button) const;
	/// <summary>
	/// コントローラのキーを離したか判定
	/// </summary>
	/// <param name="_padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnTrgUp(int _padNum, int button) const;

	/// @brief いずれかのコントローラのスティックが
	/// @param _padNum コントローラの対象
	bool PadIsAlgKeyNewAll(PAD_NO _padNum)const;

	/// @brief コントローラのスティックの入力判定
	/// @param _padNum コントローラの対象
	/// @param _algKey スティックの種類
	bool PadIsAlgKeyNew(PAD_NO _padNum, JOYPAD_ALGKEY _algKey)const;
	/// @brief コントローラのスティックの入力判定
	/// @param _padNum コントローラの対象
	/// @param _algKey スティック番号
	bool PadIsAlgKeyNew(int _padNum, int _algKey)const;

	/// @brief コントローラのスティックを入力した時の判定
	/// @param _padNum コントローラの対象
	/// @param _algKey スティックの種類
	bool PadIsAlgKeyTrgDown(PAD_NO _padNum, JOYPAD_ALGKEY _algKey)const;
	/// @brief コントローラのスティックを入力した時の判定
	/// @param _padNum コントローラの対象
	/// @param _algKey スティックの種類
	bool PadIsAlgKeyTrgDown(int _padNum, int _algKey)const;

	/// @brief コントローラのスティックを離した時の判定
	/// @param _padNum コントローラの対象
	/// @param _algKey スティックの種類
	bool PadIsAlgKeyTrgUp(PAD_NO _padNum, JOYPAD_ALGKEY _algKey)const;
	/// @brief コントローラのスティックを離した時の判定
	/// @param _padNum コントローラの対象
	/// @param _algKey スティックの種類
	bool PadIsAlgKeyTrgUp(int _padNum, int _algKey)const;
	

	struct Trigger
	{
		// 入力状態
		bool inputOld = false;
		bool inputNew = false;

		// 現フレームで入力されたか否か
		bool trgDown = false;
		bool trgUp = false;
	};

	/// @brief ゲームコントローラー入力情報
	struct Joypad
	{
		// ボタンの前フレーム入力判定
		unsigned char buttonOld[static_cast<int>(PAD_BTN::MAX)];

		// ボタンの現在フレーム入力判定
		unsigned char buttonNew[static_cast<int>(PAD_BTN::MAX)];

		// ボタン入力
		Trigger btnTrgger[static_cast<int>(PAD_BTN::MAX)];


		// スティックの横入力の傾き具合
		Vector2 algKey[static_cast<int>(JOYPAD_ALGKEY::MAX)];

		// スティックの入力判定
		Trigger algKeyTrigger[static_cast<int>(JOYPAD_ALGKEY::MAX)];;

		// 振動判定
		bool isVibration;

		// 振動の強さ(0～100)
		unsigned int vibrationPow;

		// 振動の時間
		float vibrationTime;
	};


private:


	// パッド情報
	Joypad padInfos_[static_cast<int>(PAD_NO::PAD4)];

	// DirectInputの入力状態
	DINPUT_JOYSTATE joyDInState_;

	// コントローラボタンの入力状態
	XINPUT_STATE joyXInState_;
};