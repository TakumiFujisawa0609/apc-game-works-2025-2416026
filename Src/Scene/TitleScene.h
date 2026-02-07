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
		GAME_END,   //ゲーム終了

		MAX, //要素数
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

	// タイトル状態
	TITLE_STATE state_;

	static constexpr float PV_ACTIVE_TIME = ((60.0f * 1) + 0.0f);
	int pv_;
	float pvTime_;
	float isPvActive_;

	int titleImage_;

	// コントローラ画像
	int padImage_;

	// キーボード画像
	int keyImage_;

	float selectScale_;


	/// @brief テキスト割り当て
	/// @param _posY テキスト縦位置
	/// @param _text テキストの内容
	/// @param _state 選択状態
	void DrawTitleText(int& _posY, const char* _text, TITLE_STATE _state);

	void PromotionVideo(void);
};