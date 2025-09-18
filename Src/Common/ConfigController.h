#pragma once

class ConfigController
{
public:

	void CreateInstance(void);

	ConfigController& GetInstance(void) { return *instance_; }


	void Load(void);

	void Init(void);

	void Update(void);

	void DrawDebug(void);


private:

	ConfigController* instance_;

	// デフォルトコンストラクタ
	ConfigController(void);

	// デフォルトデストラクタ
	~ConfigController(void) = default;


	// 効果音の音量倍率
	float seVolume_;

	// BGMの音量倍率
	float bgmVolume_;


};