#pragma once
#include "../Scene/SceneBase.h"
#include "../Utility/Vector2.h"
#include <DxLib.h>

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public SceneBase
{
public:

	enum class TITLE_STATE
	{
		START_GAME,     // ゲームシーン遷移状態
		INFO,           // 遊び方
		GAME_END,       //ゲーム終了

		MAX, //要素最大数
	};

	enum class INFO_TYPE
	{
		PLAY_PAD,
		PLAY_KEY,
		MAX,
	};


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	TitleScene(void);

	/// <summary>
	/// 通常のデストラクタ
	/// </summary>
	~TitleScene(void) = default; // defaultさん やっておしまいなさい

	/// <summary>
	/// 初期読み込み
	/// </summary>
	void Load(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void) override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void) override;

	//フォント設定
	void FontSettings(void);

	//フォント描画
	void DrawFont(void);

	//操作説明
	void DrawOperation(void);

	//遊び方
	void DrawExplanation(void);


private:

	// デフォルトのテキストサイズ
	static constexpr int TEXT_SIZE_DEFAULT = 33;

	// 選択時のテキストサイズ
	static constexpr int TEXT_SIZE_SELECT = 40;

	// テキスト位置
	static constexpr Vector2 TEXT_POS = { 0, 150 };

	// テキスト縦調整値
	static constexpr int TEXT_POS_Y_OFFSET = 75;

	// デフォルトのテキスト色
	static constexpr unsigned int TEXT_COLOR_DEFAULT = 0xFFFA00;

	// 選択時のテキスト色
	static constexpr unsigned int TEXT_COLOR_SELECT = 0xFF5500;

	static constexpr Vector2 INFO_TEXT_POS = { 500, 250 };

	static constexpr Vector2 INFO_SIZE = { 100, 100 };
	static constexpr Vector2 INFO_OFFSET = { 175, 16 };

	// 説明背景色
	static constexpr COLOR_F INFO_COLOR = { 200, 200, 200, 200 };

	// 遊び方の状態番号テキスト位置
	static constexpr Vector2 INFO_PARAM = { 45, 32 };

	static constexpr int ARROW_WIDTH = 650;
	static constexpr float ARROW_SCALE = 0.75f;
	static constexpr float ARROW_PERFORM_TIME = 1.0f;


	// タイトル状態
	TITLE_STATE state_;

	// 遊び方状態
	INFO_TYPE info_;

	// 遊び方を表示するか否か
	bool isViewInfo_;


	int pv_;
	float pvTime_;
	float isPvActive_;

	// タイトル
	int titleImage_;

	// コントローラ画像
	int padImage_;

	// キーボード画像
	int keyImage_;

	// ボタン画像
	int buttonImage_;

	// 矢印画像
	int arrowImage_;

	float arrowScale_;
	float arrowPerformTime_;

	float selectScale_;


	/// <summary>
	/// テキスト割り当て
	/// </summary>
	/// <param name="posY">テキスト縦位置</param>
	/// <param name="text">テキストの内容</param>
	/// <param name="state">選択状態</param>
	void DrawTitieText(int posY, const char* text, TITLE_STATE state);

	/// <summary>
	/// 選択の上の判定処理
	/// </summary>
	bool IsSelectUp(void);

	/// <summary>
	/// 選択の下の判定処理
	/// </summary>
	bool IsSelectDown(void);

	/// <summary>
	/// 決定の判定処理
	/// </summary>
	bool IsCheck(void);

	/// <summary>
	/// 決定の判定処理
	/// </summary>
	bool IsCansel(void);

	/// <summary>
	/// 遊び方更新
	/// </summary>
	void UpdateInfo(void);

	/// <summary>
	/// 遊び方描画処理
	/// </summary>
	void DrawInfo(void);
};