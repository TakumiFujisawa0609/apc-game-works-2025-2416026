#pragma once
class SceneBase;
//class Fader;

#include <chrono> // 時間
#include <DxLib.h>

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


	// 開始シーン
	static constexpr SCENE_ID START_SCENE = SCENE_ID::TITLE;

	// ゲーム内のライトの向き
	static constexpr VECTOR LIGHT_DIR = { 0.0f, -1.0f, 1.0f };


	/// <summary>
	/// インスタンス生成処理
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <returns></returns>
	static SceneManager& GetInstance(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

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
	void Release(void);

	/// <summary>
	/// シーン遷移処理
	/// </summary>
	/// <param name="nextScene">遷移後のシーン</param>
	void ChangeScene(SCENE_ID nextScene);

	/// <summary>
	/// シーンID取得
	/// </summary>
	/// <returns>現在シーンID</returns>
	SCENE_ID GetSceneId(void) const;

	/// <summary>
	/// 経過時間取得処理
	/// </summary>
	/// <returns>経過時間</returns>
	float GetDeltaTime(void) const;


private:


	static SceneManager* manager_; // 静的インスタンス

	SCENE_ID sceneId_;	   // 現在シーンID
	SCENE_ID waitSceneId_; // 遷移待機しているシーン状態

	SceneBase* curScene_; // 現在のシーン状態

	//Fader* fader_; // フェーダー

	// 直前のフレームの時間
	std::chrono::system_clock::time_point preTime_;

	// フレームの経過時間
	float deltaTime_;

	bool isChangeScene_; // シーン遷移するか否か


	/// <summary>
	/// デフォルトコンストラクタ(private)
	/// </summary>
	SceneManager(void);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	SceneManager(const SceneManager &other);

	/// <summary>
	/// デストラクタ(private)
	/// </summary>
	~SceneManager(void) = default;

	/// <summary>
	/// 3Dの初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init3D(void);

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
};