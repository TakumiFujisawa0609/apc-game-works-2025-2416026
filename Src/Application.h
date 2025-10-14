#pragma once
#include <DxLib.h>
#include <memory>
class GameExit;

class Application
{
public:

	// ゲーム名
	const char* GAME_NAME = "無双クロニクル";

	// 画面横サイズ
	static constexpr int SCREEN_SIZE_X = (16 * 100);

	// 画面縦サイズ
	static constexpr int SCREEN_SIZE_Y = (9 * 100);

	// 画面横サイズ半分
	static constexpr int SCREEN_HALF_X = (SCREEN_SIZE_X / 2);

	// 画面縦サイズ半分
	static constexpr int SCREEN_HALF_Y = (SCREEN_SIZE_Y / 2);


	// 重力最大値(default:9.81f)
	static constexpr float GRAVITY_MAX = 9.5f;

	// 重力増加値(default:0.25f)
	static constexpr float GRAVITY_ACC = 0.2f;

	// 重力増加値の軽さでの減少値
	static constexpr float GRAVITY_DEC = 0.125f;


	/// @brief 明示的にインスタンス生成処理
	static void CreateInstance(void);

	/// @brief 静的インスタンス取得処理
	/// @return アプリケーションマネージャ
	static Application& GetInstance(void) { return *instance_; };

	/// @brief 実行処理
	void Run(void);

	/// @brief インスタンス削除処理
	void Destroy(void);


	/// @brief ゲーム終了処理
	void SetIsGameEnd(void) { isGame_ = false; };

	/// @brief ゲーム継続判定フラグ取得
	bool GetIsGame(void) const { return isGame_; };

	/// @brief 終了メニューの処理を行うか否か
	void SetIsExitMenu(bool flag) { isActiveExitMenu_ = flag; };


private:

	// インスタンス
	static Application* instance_;

	// ゲーム終了
	std::unique_ptr<GameExit> exit_;

	// ゲームを継続するか否か
	bool isGame_;

	// 終了メニューを有効にできるか否か
	bool isActiveExitMenu_;


	/// @brief デフォルトコンストラクタ
	Application(void);

	/// @brief デフォルトデストラクタ
	~Application(void) = default;

	/// @brief デフォルトコピーコンストラクタ
	Application(const Application&) = default;


	/// @brief 初期化処理
	void Init(void);

	/// @brief エフェクシアの初期化処理
	void InitEffecseer(void);

	/// @brief 各マネージャー生成処理
	void CreateManagers(void);

	/// @brief 描画処理
	void Draw(void);
};