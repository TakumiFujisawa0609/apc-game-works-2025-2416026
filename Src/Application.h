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
	static constexpr float GRAVITY_MAX = 9.81f;

	// 重力増加値
	static constexpr float GRAVITY_ACC = 0.25f;

	// 重力増加値の軽さでの減少値
	static constexpr float GRAVITY_DEC = 0.125f;


	/// <summary>
	/// 明示的にインスタンス生成処理
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// 静的インスタンス取得処理
	/// </summary>
	/// <returns>アプリケーションマネージャ</returns>
	static Application& GetInstance(void) { return *instance_; };

	/// <summary>
	/// 実行処理
	/// </summary>
	void Run(void);

	/// <summary>
	/// インスタンス削除処理
	/// </summary>
	void Destroy(void);


	/// <summary>
	/// ゲーム継続判定フラグ割り当て
	/// </summary>
	void SetIsGame(bool flag) { isGame_ = flag; };

	/// <summary>
	/// ゲーム継続判定フラグ取得
	/// </summary>
	bool GetIsGame(void) const { return isGame_; };

	/// <summary>
	/// 終了メニューの処理を行うか否か
	/// </summary>
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


	// デフォルトコンストラクタ
	Application(void);

	// デフォルトデストラクタ
	~Application(void) = default;

	// デフォルトコピーコンストラクタ
	Application(const Application&) = default;


	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// エフェクシアの初期化処理
	/// </summary>
	void InitEffecseer(void);

	/// <summary>
	/// マネージャーを生成
	/// </summary>
	void CreateManagers(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);
};