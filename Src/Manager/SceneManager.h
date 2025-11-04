#pragma once
#include <chrono>
#include <DxLib.h>
#include "../Common/Vector2.h"
class SceneBase;
//class Fader;
class Camera;

class SceneManager
{
public:

	/// @brief シーン種類
	enum class SCENE_ID
	{
		NONE = 0,
		TITLE,    // タイトル
		GAME,     // ゲーム
		CLEAR,    // クリアシーン
		GAMEOVER, // ゲームオーバー
	};

	/// @brief 演出状態
	enum class PERFORM_TYPE
	{
		NONE = 0, // 無効
		HIT_STOP, // ヒットストップ
		SLOW,	  // 遅延演出
		SHAKE,		// 振動演出
		SLOW_SHAKE, // 遅延+振動
	};


	// 開始シーン
	static constexpr SCENE_ID START_SCENE = SCENE_ID::TITLE;

	// ゲーム内のライトの向き
	static constexpr VECTOR LIGHT_DIR = { 0.0f, -1.0f, 1.0f };

	//振動の最大振れ幅
	static constexpr float SHAKE_MAX_X = 8.0f;
	static constexpr float SHAKE_MAX_Y = 5.0f;


	/// @brief インスタンス生成処理
	static void CreateInstance(void);

	/// @brief インスタンス取得処理
	static SceneManager& GetInstance(void) { return *instance_; };

	/// @brief 初期化処理 
	void Load(void);

	/// @brief 更新処理
	void Update(void);

	/// @brief 描画処理
	void Draw(void);

	/// <summary>
	/// デバッグ描画処理
	/// </summary>
	void DrawDebug(void);

	/// @brief 解放処理
	void Destroy(void);

	/// <summary>
	/// シーン遷移処理
	/// </summary>
	/// <param name="nextScene">遷移後のシーン</param>
	void ChangeScene(SCENE_ID nextScene);

	/// @brief 現在シーンID取得
	SCENE_ID GetSceneId(void) const { return sceneId_; };

	/// @brief 経過時間取得処理
	float GetDeltaTime(void) const { return deltaTime_; };

	/// @brief カメラ取得
	Camera& GetCamera(void) { return *camera_; };

	/// <summary>
	/// 演出時間
	/// </summary>
	/// <param name="type">演出の種類</param>
	/// <param name="time">演出の時間</param>
	/// <param name="slowTerm">遅延の間隔</param>
	/// <param name="shakePowerX">横の振動の強さ</param>
	/// <param name="shakePowerY">縦の振動の強さ</param>
	void SetPerform(PERFORM_TYPE type, float time,
					int slowTerm = 2, float shakePowerX = 0.0f, float shakePowerY = 0.0f);

	/// <summary>
	/// グリッド線描画
	/// </summary>
	void DrawGrid(void);

	/// <summary>
	/// デバッグモード有効判定
	/// </summary>
	bool GetIsDebugMode(void);


private:

	// 背景色
	static constexpr COLOR_F BACK_COLOR = { 128,128,128 };


	// 線の長さ
	static constexpr float GRID_LEN = 3000.0f;

	// 線の長さの半分
	static constexpr float GRID_HLEN = (GRID_LEN / 2.0f);

	// 線の間隔
	static constexpr float GRID_TERM = 100.0f;

	// 線の数
	static const int GRID_NUM = static_cast<int>(GRID_LEN / GRID_TERM);

	// 線の数の半分
	static const int GRID_HNUM = GRID_NUM / 2;


	/// <summary>
	/// 演出
	/// </summary>
	struct Perform
	{
		// 演出状態
		PERFORM_TYPE type;

		// ヒットストップ演出時間
		float time;

		Vector2 shakePos;

		int  slowTerm;

		int tempScreen_;
	};
	// 演出
	Perform perform_;


	// 静的インスタンス
	static SceneManager* instance_;

	// 現在シーンID
	SCENE_ID sceneId_;

	// 遷移待機しているシーン状態
	SCENE_ID waitSceneId_;

	// 現在のシーン状態
	SceneBase* curScene_;

	// カメラ
	Camera* camera_;

	//Fader* fader_; // フェーダー


	// 直前のフレームの時間
	std::chrono::system_clock::time_point preTime_;

	// フレームの経過時間
	float deltaTime_;

	// シーン遷移するか否か
	bool isChangeScene_;

	bool isDebugMode_;


	/// @brief デフォルトコンストラクタ
	SceneManager(void);

	/// @brief コピーコンストラクタ対策
	SceneManager(const SceneManager &other) = default;

	/// @brief デフォルトデストラクタ
	~SceneManager(void) = default;

	/// @brief 3Dの初期化処理
	void Init3D(void);

	/// <summary>
	/// ヒットストップ初期化
	/// </summary>
	void InitPerform(void);


	/// <summary>
	/// 遷移処理
	/// </summary>
	/// <param name="nextScene"></param>
	void DoChangeState(SceneManager::SCENE_ID nextScene);

	/// @brief 各フェードの処理
	void Fade(void);

	/// @brief 経過時間の処理
	void DeltaCount(void);

	/// <summary>
	/// 演出処理
	/// </summary>
	/// <returns>停止するか否か</returns>
	bool Performance(void);
};