/* ---------------------
　アプリケーションヘッダ
 ----------------------- */
#pragma once
class FrameRate;


class Application
{
public:

	/* --- <<< 定数定義 >>> --- */

	static constexpr int SCREEN_SIZE_X = 640; // 画面横サイズ
	static constexpr int SCREEN_SIZE_Y = 480; // 画面縦サイズ


	/* --- <<<< 関数プロトタイプ宣言 >>> --- */

	/// <summary>
	/// 明示的にインスタンス生成処理
	/// </summary>
	static void CreateManager(void);

	/// <summary>
	/// 静的インスタンス取得処理
	/// </summary>
	/// <returns>アプリケーションマネージャ</returns>
	static Application& GetInstance(void);

	/// <summary>
	/// 実行処理
	/// </summary>
	void Run(void);

	/// <summary>
	/// インスタンス削除処理
	/// </summary>
	void Destroy(void);


private:

	static Application* manager_; // インスタンス



	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	Application(void);

	/// <summary>
	/// デフォルトのデストラクタ
	/// </summary>
	~Application(void) = default;

	/// <summary>
	/// デフォルトのコピーコンストラクタ
	/// </summary>
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
	/// 描画処理
	/// </summary>
	void Draw(void);
};