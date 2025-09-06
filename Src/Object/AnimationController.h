#pragma once
#include <map>
#include <string>

class AnimationController
{
public:

	// アニメーションデータ
	struct Animation
	{
		int model		= -1;
		int attachNo	= -1;
		int animIndex	= 0;
		float speed		= 0.0f; // 再生速度
		float totalTime = 0.0f; // 最大再生時間
		float step		= 0.0f; // 現在再生時間
	};


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	AnimationController(void);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelId">キャラモデルID</param>
	AnimationController(int modelId);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~AnimationController(void) = default;

	/// <summary>
	/// 同じFBX内のアニメーションを準備
	/// </summary>
	/// <param name="type"></param>
	/// <param name="speed">再生速度</param>
	/// <param name="animIndex"></param>
	void AddInFbx(int type, float speed, int animIndex);

	/// <summary>
	/// 別のFBXからアニメーションを準備
	/// </summary>
	/// <param name="type"></param>
	/// <param name="speed">再生速度</param>
	/// <param name="path"></param>
	void Add(int type, float speed, const std::string path);


	/// <summary>
	/// アニメーション再生
	/// </summary>
	/// <param name="type">アニメーションの種類</param>
	/// <param name="isLoop">ループするか否か(default=true)</param>
	void Play(int type, bool isLoop = true);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// デバッグ描画処理
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// メモリ解放処理
	/// </summary>
	void Release(void);


	/// <summary>
	/// アニメーションが終了したか判定
	/// </summary>
	bool IsEnd(void) const;

	/// <summary>
	/// 再生中のアニメーション番号取得
	/// </summary>
	int GetPlayType(void);

	/// <summary>
	/// アニメーション停止処理
	/// </summary>
	/// <param name="isStop"></param>
	void Stop(bool isStop = true);

	/// <summary>
	/// 再生位置変更処理
	/// </summary>
	/// <param name="step">再生する位置</param>
	void SetAnimStep(float step = 0.0f);

	/// <summary>
	/// 再生中のアニメーションの現在時間を取得
	/// </summary>
	float GetPlayTime(void);

	/// <summary>
	/// 再生中のアニメーションの総再生時間を取得
	/// </summary>
	float GetPlayTimeTotal(void);


private:

	// アニメーションするモデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation> animations_;

	// 再生中のアニメーション
	int playType_;

	// 再生中のアニメーションデータ
	Animation playAnim_;

	// ループするか否かの判定
	bool isLoop_;

	// アニメーションを停止するか否か
	bool isStop_;
	

	/// <summary>
	/// アニメーション追加の共通処理
	/// </summary>
	/// <param name="type"></param>
	/// <param name="speed">再生速度</param>
	/// <param name="animIndex"></param>
	void Add(int type, float speed, Animation& animIndex);
};