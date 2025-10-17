#pragma once
#include <unordered_map>
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


	/// @brief デフォルトコンストラクタ
	AnimationController(void);

	/// @brief コンストラクタ
	/// @param modelId キャラモデルID
	AnimationController(int modelId);

	/// @brief デフォルトデストラクタ
	~AnimationController(void) = default;

	/// <summary>
	/// 同じFBX内のアニメーションを準備
	/// </summary>
	/// <param name="_type"></param>
	/// <param name="_speed">再生速度</param>
	/// <param name="_animIndex"></param>
	void AddInFbx(int _type, float _speed, int _animIndex);

	/// <summary>
	/// 別のFBXからアニメーションを準備
	/// </summary>
	/// <param name="_type"></param>
	/// <param name="_speed">再生速度</param>
	/// <param name="path"></param>
	void Add(int _type, float _speed, const std::string path);


	/// @brief アニメーション再生
	/// @param _type アニメーションの種類
	/// @param _isLoop ループするか否か[default=true]
	void Play(int _type, bool _isLoop = true);

	/// @brief 更新処理
	void Update(void);

	/// @brief デバッグ描画処理
	void DrawDebug(void);

	/// @brief メモリ解放処理
	void Release(void);


	/// @brief アニメーションが終了したか判定
	bool IsEnd(void) const;

	/// @brief 再生中のアニメーション番号取得
	int GetPlayType(void) { return playType_; };

	/// @brief アニメーション停止処理
	void Stop(bool isStop = true) { isStop_ = isStop; };

	/// <summary>
	/// 再生位置変更処理
	/// </summary>
	/// <param name="step">再生する位置</param>
	void SetAnimStep(float _step = 0.0f);

	/// @brief 再生中のアニメーションの現在時間を取得
	float GetPlayTime(void);

	/// @brief 再生中のアニメーションの総再生時間を取得
	float GetPlayTimeTotal(void);


private:

	// アニメーションするモデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::unordered_map<int, Animation> animations_;

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
	/// <param name="_type"></param>
	/// <param name="_speed">再生速度</param>
	/// <param name="_animIndex"></param>
	
	/// @brief アニメーション追加処理
	/// @param _type アニメーションの種類
	/// @param _speed 再生速度
	/// @param _animIndex 格納するアニメーションリスト
	void Add(int _type, float _speed, Animation& _animIndex);

	/// <summary>
	/// アニメーションが格納されているか判定
	/// </summary>
	/// <param name="_type">アニメーションの種類</param>
	bool IsFindAnimation(int _type);
};