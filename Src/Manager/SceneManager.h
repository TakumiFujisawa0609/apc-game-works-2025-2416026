#pragma once
class SceneBase;
//class Fader;

#include <chrono> // 時間
#include <DxLib.h>
#include "../Utility/Vector2.h"

/// <summary>
/// シーン管理マネージャ
/// </summary>
class SceneManager
{
public:

	/// <summary>
	/// シーン種類
	/// </summary>
	enum class SCENE_ID
	{
		NONE = 0,
		TITLE,		// タイトル
		GAME,		// ゲーム
	};

	/// <summary>
	/// 演出状態
	/// </summary>
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


	/// <summary>
	/// インスタンス生成処理
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <returns></returns>
	static SceneManager& GetInstance(void) { return *instance_; };

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Load(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Destroy(void);

	/// <summary>
	/// シーン遷移処理
	/// </summary>
	/// <param name="nextScene">遷移後のシーン</param>
	void ChangeScene(SCENE_ID nextScene);

	/// <summary>
	/// シーンID取得
	/// </summary>
	/// <returns>現在シーンID</returns>
	SCENE_ID GetSceneId(void) const { return sceneId_; };

	/// <summary>
	/// 経過時間取得処理
	/// </summary>
	/// <returns>経過時間</returns>
	float GetDeltaTime(void) const { return deltaTime_; };

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


private:

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

	// 静的インスタンス
	static SceneManager* instance_;

	// 現在シーンID
	SCENE_ID sceneId_;

	// 遷移待機しているシーン状態
	SCENE_ID waitSceneId_;

	// 現在のシーン状態
	SceneBase* curScene_;

	//Fader* fader_; // フェーダー


	// 直前のフレームの時間
	std::chrono::system_clock::time_point preTime_;

	// フレームの経過時間
	float deltaTime_;

	// シーン遷移するか否か
	bool isChangeScene_;

	// 演出
	Perform perform_;


	/// <summary>
	/// デフォルトコンストラクタ(private)
	/// </summary>
	SceneManager(void);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	SceneManager(const SceneManager &other) = default;

	/// <summary>
	/// デストラクタ(private)
	/// </summary>
	~SceneManager(void) = default;

	/// <summary>
	/// 3Dの初期化処理
	/// </summary>
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

	/// <summary>
	/// 各フェードの処理
	/// </summary>
	void Fade(void);

	/// <summary>
	/// 経過時間の処理
	/// </summary>
	void DeltaCount(void);

	/// <summary>
	/// 演出処理
	/// </summary>
	/// <returns>停止するか否か</returns>
	bool Performance(void);
};