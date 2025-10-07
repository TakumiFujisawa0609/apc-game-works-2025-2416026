#pragma once

class ConfigManager
{
public:

	enum class TYPE
	{
		INPUT_TYPE, // 入力状態
		VOLUME_BGM, // BGM
		VOLUME_SE,  // SE

		BACK, // 戻る
		MAX,
	};

	static void CreateInstance(void);

	static ConfigManager& GetInstance(void) { return *instance_; }

	static void Destroy(void);

	void Load(void);

	void Init(void);

	void Update(void);

	void DrawDebug(void);


private:

	static ConfigManager* instance_;

	// コンフィグ状態
	TYPE configType_;


	// デフォルトコンストラクタ
	ConfigManager(void);

	// デフォルトデストラクタ
	~ConfigManager(void) = default;


	// 効果音の音量倍率
	float seVolume_;

	// BGMの音量倍率
	float bgmVolume_;

	/// <summary>
	/// コンフィグ変更の変更処理
	/// </summary>
	void ChangeConfigType(void);
};