#pragma once
#include "./SceneBase.h"
#include "../Common/Vector2.h"
#include "../Utility/UtilityCommon.h"
#include <DxLib.h>


class TitleScene : public SceneBase
{
public:

	enum class TITLE_STATE
	{
		START_GAME, // ゲームシーン遷移状態
		//INFO,       // 遊び方
		GAME_END,   //ゲーム終了

		MAX, //要素数
	};

	enum class INFO_TYPE
	{
		PLAY_PAD,
		PLAY_KEY,
		MAX,
	};


	/// @brief デフォルトコンストラクタ
	TitleScene(void);

	/// @brief デフォルトデストラクタ
	~TitleScene(void) = default;

	/// @brief 初期読み込み
	void Load(void) override;

	/// @brief 初期化処理
	void Init(void) override;

	/// @brief 更新処理
	void Update(void) override;

	/// @brief 描画処理
	void Draw(void) override;

	/// @brief 解放処理
	void Release(void) override;

	/// @brief フォント設定
	void FontSettings(void);

	/// @brief フォント描画
	void DrawFont(void);

	/// @brief 操作説明
	void DrawOperation(void);

	/// @brief 遊び方
	void DrawExplanation(void);


private:

	// デフォルトのテキストサイズ
	static constexpr int TEXT_SIZE_DEFAULT = 33;

	// 選択時のテキストサイズ
	static constexpr int TEXT_SIZE_SELECT = 40;

	// テキスト位置
	static constexpr Vector2 TEXT_POS = { 0, 185 };

	// テキスト縦調整値
	static constexpr int TEXT_POS_Y_OFFSET = 75;

	// デフォルトのテキスト色
	static constexpr unsigned int TEXT_COLOR_DEFAULT = 0x0;

	// 選択時のテキスト色
	static constexpr unsigned int TEXT_COLOR_SELECT = 0xffffff;

	static constexpr Vector2 INFO_TEXT_POS = { 500, 250 };

	static constexpr Vector2 INFO_SIZE = { 100, 100 };
	static constexpr Vector2 INFO_OFFSET = { 175, 16 };

	// 説明背景色
	static constexpr UtilityCommon::Color INFO_COLOR = { 200, 200, 200, 200 };

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


	/// @brief テキスト割り当て
	/// @param _posY テキスト縦位置
	/// @param _text テキストの内容
	/// @param _state 選択状態
	void DrawTitleText(int& _posY, const char* _text, TITLE_STATE _state);

	/// @brief 遊び方更新
	void UpdateInfo(void);

	/// @brief 遊び方描画処理
	void DrawInfo(void);

	void PromotionVideo(void);
};