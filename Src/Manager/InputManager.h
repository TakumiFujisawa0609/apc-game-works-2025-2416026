#pragma once
#include "../Utility/Vector2.h"
#include <map>
#include <DxLib.h>

class InputManager
{
public:


	/// <summary>
	/// コントローラー認識番号
	/// </summary>
	enum class JOYPAD_NO
	{
		KEY_PAD1 = 0,	  // キー入力とパッド１
		PAD1,			  // パッド１入力
		PAD2,			  // パッド２入力
		PAD3,			  // パッド３入力
		PAD4 = 4,		  // パッド４入力
		INPUT_KEY = 4096, // キー入力
	};

	/// <summary>
	/// ゲームコントローラータイプ
	/// </summary>
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


	/// <summary>
	/// ゲームコントローラーボタン
	/// </summary>
	enum class PAD_BTN
	{
		DOWN = 0,  // A
		RIGHT,     // B
		LEFT,      // X
		UP,        // Y
		L_BUTTON,  // L
		R_BUTTON,  // R
		BACK,      // BACK
		START,     // START
		L_TRIGGER, // LT
		R_TRIGGER, // RT

		MAX	// 要素数
	};

	/// <sammary>
	/// ゲームコントローラスティック
	/// </sammary>
	enum class JOYPAD_ALGKEY
	{
		LEFT = 0, // 左スティック
		RIGHT,    // 右スティック
		D_PAD,    // 十字キー

		MAX
	};


	/// <summary>
	/// ゲームコントローラー入力情報
	/// </summary>
	struct JOYPAD_IN_STATE
	{
		// ボタンの前フレーム入力判定
		unsigned char ButtonOld[static_cast<int>(PAD_BTN::MAX)];

		// ボタンの現在フレーム入力判定
		unsigned char ButtonNew[static_cast<int>(PAD_BTN::MAX)];

		bool IsOld[static_cast<int>(PAD_BTN::MAX)];
		bool IsNew[static_cast<int>(PAD_BTN::MAX)];

		// 入力時の判定
		bool IsTrgDown[static_cast<int>(PAD_BTN::MAX)];

		// 離した時の判定
		bool IsTrgUp[static_cast<int>(PAD_BTN::MAX)];


		// スティックの横入力の傾き具合
		int AlgKeyX[static_cast<int>(JOYPAD_ALGKEY::MAX)];

		// スティックの縦入力の傾き具合
		int AlgKeyY[static_cast<int>(JOYPAD_ALGKEY::MAX)];

		// スティックの前フレームの入力判定
		bool IsOldAlgKey[static_cast<int>(JOYPAD_ALGKEY::MAX)];

		// スティックの現在フレーム入力判定
		bool IsNewAlgKey[static_cast<int>(JOYPAD_ALGKEY::MAX)];

		// スティックの入力時の判定
		bool IsTrgDownAlgKey[static_cast<int>(JOYPAD_ALGKEY::MAX)];

		// スティックを離した時の判定
		bool IsTrgUpAlgKey[static_cast<int>(JOYPAD_ALGKEY::MAX)];
	};


	/// <summary>
	/// インスタンスを明示的に生成
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns>入力マネージャ</returns>
	static InputManager& GetInstance(void);


	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// インスタンス削除
	/// </summary>
	void Release(void);

#pragma region キーボード入力

	/// <summary>
	/// 判定を行うキーを追加
	/// </summary>
	/// <param name="keyType">追加対象</param>
	void AddKey(int type);

	/// <summary>
	/// 判定を行うキーをクリア
	/// </summary>
	void Clear(void);

	/// <summary>
	/// キーの入力判定
	/// </summary>
	/// <param name="keyType">判定するキー</param>
	/// <returns>入力しているか否か</returns>
	bool KeyIsNew(int type) const;

	/// <summary>
	/// キーを入力した瞬間の判定
	/// </summary>
	/// <param name="keyType">判定するキー</param>
	/// <returns>キー入力したか否か</returns>
	bool KeyIsTrgDown(int keyType) const;

	/// <summary>
	/// キーを離した瞬間の判定
	/// </summary>
	/// <param name="keyType">判定するキー</param>
	/// <returns>キーを離したか否か</returns>
	bool KeyIsTrgUp(int keyType) const; // constで読み取り専用化

	/// <summary>
	/// 全てのキーの入力判定
	/// </summary>
	/// <returns></returns>
	bool KeyIsNewAll(void)const;
#pragma endregion

#pragma region マウス入力
	/// <summary>
	/// マウス座標の取得
	/// </summary>
	/// <returns>マウス座標</returns>
	Vector2 GetMousePos(void) const; // constで読み取り専用化

	/// <summary>
	/// マウスクリック状態を取得
	/// </summary>
	int GetMouse(void) const; // constで読み取り専用化

	/// <summary>
	/// 左クリック入力処理
	/// </summary>
	/// <returns>左クリックしたか否か</returns>
	bool IsClickMouseLeft(void) const;

	/// <summary>
	/// 右クリック入力処理
	/// </summary>
	/// <returns>右クリックしたか否か</returns>
	bool IsClickMouseRight(void) const;

	/// <summary>
	/// 左クリックした瞬間の判定
	/// </summary>
	/// <returns>左クリックを入力したか否か</returns>
	bool IsTrgClickMouseLeft(void) const;

	/// <summary>
	/// 右クリックした瞬間の判定
	/// </summary>
	/// <returns>右クリックを入力したか否か</returns>
	bool IsTrgClickMouseRight(void) const;

#pragma endregion

#pragma region コントローラー入力
	/// <summary>
	/// コントローラの入力情報
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	JOYPAD_IN_STATE& GetPadInputState(JOYPAD_NO padNum);

	/// <summary>
	/// コントローラの入力判定
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnNew(int padNum, PAD_BTN button) const;
	/// <summary>
	/// コントローラの入力判定
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnNew(int padNum, int button) const;

	/// <summary>
	/// コントローラのキーを押したか判定
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnTrgDown(int padNum, PAD_BTN button) const;
	/// <summary>
	/// コントローラのキーを押したか判定
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnTrgDown(int padNum, int button) const;

	/// <summary>
	/// コントローラのキーを離したか判定
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnTrgUp(int padNum, PAD_BTN button) const;
	/// <summary>
	/// コントローラのキーを離したか判定
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="button">コントローラ番号</param>
	bool PadIsBtnTrgUp(int padNum, int button) const;

	/// <sammary>
	/// コントローラのスティックの入力判定
	/// </sammary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	bool PadIsAlgKeyNew(int padNum, JOYPAD_ALGKEY algKey)const;
	/// <sammary>
	/// コントローラのスティックの入力判定
	/// </sammary>
	///<param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	bool PadIsAlgKeyNew(int padNum, int algKey)const;

	/// <sammary>
	/// コントローラのスティックを入力時の判定
	/// </sammary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	bool PadIsAlgKeyTrgDown(int padNum, JOYPAD_ALGKEY algKey)const;
	/// <sammary>
	/// コントローラのスティックを入力時の判定
	/// </sammary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	bool PadIsAlgKeyTrgDown(int padNum, int algKey)const;

	/// <sammary>
	/// コントローラのスティックを離した時の判定
	/// </sammary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	bool PadIsAlgKeyTrgUp(int padNum, JOYPAD_ALGKEY algKey)const;
	/// <sammary>
	/// コントローラのスティックを離した時の判定
	/// </sammary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	bool PadIsAlgKeyTrgUp(int padNum, int algKey)const;

	/// <summary>
	/// コントローラのスティック横移動量を取得
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	int PadAlgKeyX(int padNum, JOYPAD_ALGKEY algKey)const;
	/// <summary>
	/// コントローラのスティック横移動量を取得
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	int PadAlgKeyX(int padNum, int algKey)const;

	/// <summary>
	/// コントローラのスティック縦移動量を取得
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	int PadAlgKeyY(int padNum, JOYPAD_ALGKEY algKey)const;
	/// <summary>
	/// コントローラのスティック縦移動量を取得
	/// </summary>
	/// <param name="padNum">コントローラの対象</param>
	/// <param name="algKey">スティック番号</param>
	int PadAlgKeyY(int padNum, int algKey)const;

	/// <summary>
	/// いずれかのコントローラの入力判定
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PadAnyInput(void)const;

#pragma endregion

private:

	// シングルトンインスタンス
	static InputManager* instance_;

	// キー情報
	struct Key
	{
		int keyType;	   // キーの種類
		bool inputOld; // 1フレーム前の入力状態
		bool inputNew; // 現フレームの入力状態
		bool trgDown;  // 現フレームで入力されたか否か
		bool trgUp;	   // 現フレームで入力が終了したか否か
	};

	// マウス情報
	struct Mouse
	{
		int type;	   // 入力の種類
		bool inputOld; // 1フレーム前の入力状態
		bool inputNew; // 現フレームの入力状態
		bool trgDown;  // 現フレームで入力されたか否か
		bool trgUp;	   // 現フレームで入力が終了したか否か
	};

#pragma region キーボードのメンバ変数
	// キー情報配列
	std::map<int, InputManager::Key> keys_;
	InputManager::Key keyInfoEmpty_;
#pragma endregion


	/*　マウスのメンバ変数　*/

	// マウス情報配列
	std::map<int, InputManager::Mouse> mouses_;
	InputManager::Mouse mouseInfoEmpty_;

	Vector2 mousePos_; // マウスカーソル位置
	int mouseInput_;   // マウスボタン入力状態


	/*　コントローラのメンバ変数　*/

	// パッド情報
	JOYPAD_IN_STATE padInfos_[5];

	// DirectInputの入力状態
	DINPUT_JOYSTATE joyDInState_;

	// コントローラボタンの入力状態
	XINPUT_STATE joyXInState_;



	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	InputManager(void);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	InputManager(const InputManager& other);

	/// <summary>
	/// 通常デストラクタ
	/// </summary>
	~InputManager(void) = default;

	/// <summary>
	/// 配列内から対象のキーの入力を取得
	/// </summary>
	/// <param name="keyType">探索するキーの種類</param>
	/// <returns>キーの情報(定数)</returns>
	const InputManager::Key& FindKey(int keyType) const;

	/// <summary>
	/// 配列内から対象のマウスボタンの入力を取得
	/// </summary>
	/// <param name="keyType">探索するマウスボタンの種類</param>
	/// <returns>マウスボタンの情報(定数)</returns>
	const InputManager::Mouse& FindMouse(int mouseType) const;

	/// <summary>
	/// 判定する入力キーを割り当て
	/// </summary>
	void SetInputKey(void);

	/// <summary>
	/// 情報配列からキー情報を取得
	/// </summary>
	/// <param name="keyType">取得する対象</param>
	/// <returns>キー情報</returns>
	InputManager::Key& FindKey(int keyType);

#pragma region マウスメンバ変数
	/// <summary>
	/// 判定するマウスを割り当て
	/// </summary>
	void SetInputMouse(void);

	/// <summary>
	/// 情報配列からマウス情報を取得
	/// </summary>
	/// <param name="mouseType">取得する対象</param>
	/// <returns>マウス情報</returns>
	InputManager::Mouse& FindMouse(int mouseType);
#pragma endregion

#pragma region コントローラ識別
	/// <summary>
	/// 接続されたコントローラを識別して取得
	/// </summary>
	/// <param name="padNum">判定するコントローラ番号</param>
	/// <returns>コントローラ識別情報</returns>
	JOYPAD_TYPE GetPadType(JOYPAD_NO padNum);

	/// <summary>
	/// DirectInputの入力取得
	/// </summary>
	/// <param name="padNum">判定するコントローラ番号</param>
	/// <returns>DirectInput入力情報</returns>
	DINPUT_JOYSTATE GetPadDInputState(JOYPAD_NO padNum);

	/// <summary>
	/// コントローラボタンの入力取得
	/// </summary>
	/// <param name="padNum">判定するコントローラ番号</param>
	/// <returns>コントローラボタン入力情報</returns>
	XINPUT_STATE GetPadXInputState(JOYPAD_NO padNum);

	/// <summary>
	/// コントローラを識別して取得
	/// </summary>
	/// <param name="jpadNum">判定するコントローラ番号</param>
	/// <returns>コントローラ識別情報</returns>
	void SetPadInState(JOYPAD_NO jpadNum);

	/// <summary>
	/// 接続されたコントローラの識別を取得
	/// </summary>
	/// <param name="jPadNo"></param>
	JOYPAD_TYPE GetJPadType(JOYPAD_NO jPadNo);

#pragma endregion
};