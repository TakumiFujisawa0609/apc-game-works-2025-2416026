#pragma once
#include "../Manager/InputManager.h"
#include "../Common/Vector2.h"

class SceneBase
{
	using PAD_ALGKEY = InputManager::JOYPAD_ALGKEY;
	using PAD_BTN = InputManager::PAD_BTN;
	using PAD_NO = InputManager::PAD_NO;

public:

	// デフォルトコンストラクタ
	SceneBase(void);

	// デフォルトデストラクタ
	virtual ~SceneBase(void) = default;

	// 初回リソース読み込み処理
	virtual void Load(void) = 0;

	// 初期化処理
	virtual void Init(void) = 0;
	
	// 更新処理
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;


	/// <summary>
	/// ロード出来たか否か
	/// </summary>
	bool IsLoad(void);


protected:

	/// @brief 決定時の入力判定
	bool IsCheck(void);

	/// @brief キャンセル時の入力判定
	bool IsCansel(void);

	/// <summary>
	/// 選択の上の判定処理
	/// </summary>
	bool IsSelectUp(void);

	/// <summary>
	/// 選択の下の判定処理
	/// </summary>
	bool IsSelectDown(void);

	/// @brief 状態遷移
	/// @param _state 状態(int)
	/// @param _maxState 状態の最大(int)
	/// @param _max 状態の要素数
	void ChangeState(int& _state, int _maxState, int _max);

	void DrawFontText(Vector2 pos, int size, unsigned int _color, 
					  const char* text, bool _isSelect);
};