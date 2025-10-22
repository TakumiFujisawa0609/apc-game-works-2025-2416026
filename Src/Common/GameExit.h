#pragma once
#include <DxLib.h>
#include <string>
#include "../Common/Vector2.h"

class GameExit
{
public:

	
	/// <summary>
	/// 
	/// 終了確認状態
	/// </summary>
	enum class SELECT
	{
		NONE = -1,
		YES, // ゲーム終了
		NO,  // 終了無効

		MAX,
	};
	

	// メニューのテキスト
	const char* TEXT_LABEL_END = "ゲームを終了する？";

	const char* TEXT_LABEL_TITLE = "タイトルに戻る？";


	// 決定のテキスト
	const char* TEXT_YES = "はい";

	// キャンセルのテキスト
	const char* TEXT_NO = "いいえ";


	// テキスト位置
	static constexpr Vector2 MENU_TEXT_OFFSET = { -75, -100 };


	// メニューの横サイズ
	static constexpr int MENU_WIDTH  = 500;

	// メニューの縦サイズ
	static constexpr int MENU_HEIGHT = 250;

	// メニューの色
	static constexpr COLOR_F MENU_COLOR = {125, 125, 125,(255 - 50)};

	// 背景の透明度
	static constexpr int BACK_ALPHA = (255 - 50);


	// メニューのテキストの非選択時の透明度
	static constexpr COLOR_F MENU_TEXT_COLOR = { 255, 0, 0, (255 - 200) };

	// テキストの間隔
	static constexpr Vector2 TEXT_OFFSET = {200, 75};



	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	GameExit(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameExit(void) = default;

	
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// @brief 描画処理
	void Draw(void);

	/// @brief 解放処理
	void Release(void);

	
	/// @brief 終了メニューが有効か判定
	bool GetIsActiveMenu(void)const { return isActiveMenu_; };


private:


	// 終了確認状態
	SELECT select_;


	// 終了確認画面有効化フラグ
	bool isActiveMenu_;

	// ゲーム終了フラグ
	bool isEnd_;

	/// <summary>
	/// メニュー有効判定
	/// </summary>
	bool IsActive(void);


	/// <summary>
	/// 選択判定
	/// </summary>
	bool IsSelect(void);
	
	/// <summary>
	/// 決定判定
	/// </summary>
	bool IsCheck(void);
};
