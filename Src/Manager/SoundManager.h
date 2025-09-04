#pragma once
#include <map>
#include <string>
#include "Sound.h"
#include "ResourceManager.h"

class SoundManager
{
public:

	enum class SRC
	{
		BGM_TITLE,
		BGM_TUTORIAL,

		SE_CLICK,  // クリック
		SE_ATTACK, // 攻撃
		SE_THROW,  // 投げる
		SE_CATCH,  // つかむ
		SE_KNOCK,  // 撃破
		SE_RESET,  // 位置リセット
	};

	// タイトルBGM
	static const std::string PATH_BGM_TITLE;

	// チュートリアルBGM
	static const std::string PATH_BGM_TUTORIAL;

	// ゲームシーンBGM
	static const std::string PATH_BGM_GAME;


	// クリックSE
	static const std::string PATH_SE_CLICK;

	// 攻撃SE
	static const std::string PATH_SE_ATTACK;

	// 撃破SE
	static const std::string PATH_SE_KNOCK;

	// つかむSE
	static const std::string PATH_SE_CATCH;

	// 投げるSE
	static const std::string PATH_SE_THROW;

	// 撃破SE
	static const std::string PATH_SE_KNOCKDOWN;

	// リセットSE
	static const std::string PATH_SE_RESET;



	// マスターボリューム
	static constexpr float VOLUME_MASTER_MAX = 255;


	// タイトルBGM音量
	static constexpr float VOLUME_TITLE = 1.0f;

	// チュートリアルBGM音量
	static constexpr float VOLUME_TUTORIAL = 0.5f;

	// ゲームシーンBGM音量
	static constexpr float VOLUME_GAME = 0.5f;


	// クリックSEの音量
	static constexpr float VOLUME_CLICK = 0.9f;

	// 攻撃SEの音量
	static constexpr float VOLUME_ATTACK = 0.9f;

	// つかむSEの音量
	static constexpr float VOLUME_CATCH = 0.9f;

	// なげるSEの音量
	static constexpr float VOLUME_THROW = 1.0f;

	// 撃破SEの音量
	static constexpr float VOLUME_KNOCK = 1.5f;

	// 場外からもとに戻るSEの音量
	static constexpr float VOLUME_RESET = 0.5f;



	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static SoundManager& GetInstance(void);


	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// メモリ解放処理
	/// </summary>
	void Release(void);

	/// <summary>
	/// リソース完全破棄処理
	/// </summary>
	void Destroy(void);

	/// <summary>
	/// 2Dサウンド再生処理
	/// </summary>
	/// <param name="sec">再生するサウンドの種類</param>
	/// <param name="times">再生時間[default:LOOP]</param>
	/// <param name="isForce"></param>
	/// <returns></returns>
	bool Play(SRC sec, Sound::TIMES times = Sound::TIMES::LOOP, bool isForce = false);

	/// <summary>
	/// 3Dサウンド再生処理
	/// </summary>
	/// <param name="sec">再生するサウンドの種類</param>
	/// <param name="times">再生時間</param>
	/// <param name="pos">音量が出てくる座標</param>
	/// <param name="radius">音量の聞こえる半径</param>
	/// <returns></returns>
	bool Play(SRC sec, Sound::TIMES times, VECTOR pos, float radius);

	/// <summary>
	/// サウンドが再生開始しているか判定
	/// </summary>
	/// <param name="src">再生するサウンドの種類</param>
	bool IsSoundStart(SRC src);

	/// <summary>
	/// サウンドが再生しているか判定
	/// </summary>
	/// <param name="src">再生するサウンドの種類</param>
	bool IsSoundPlay(SRC src);

	/// <summary>
	/// サウンドが停止しているか判定
	/// </summary>
	/// <param name="src">再生するサウンドの種類</param>
	bool IsSoundEnd(SRC src);

	/// <summary>
	/// 指定の音声を停止
	/// </summary>
	void Stop(SRC src);

	/// <summary>
	/// 全音声を停止
	/// </summary>
	void StopAll(void);

	/// <summary>
	/// 音量を割り当て
	/// </summary>
	/// <param name="src">割り当てるサウンド</param>
	/// <param name="per">音量の倍率(0.0～1.0)</param>
	void SetVolume(SRC src, float per = 1.0f);

	/// <summary>
	/// 主音量割り当て
	/// </summary>
	void SetVolumeMaster(int master = 255);

	/// <summary>
	/// 主音量取得
	/// </summary>
	int GetVolumeMaster(void);




private:

	// 静的インスタンス
	static SoundManager* instance_;

	std::map<SRC, Sound> sounds_;

	// 主音量
	int volumeMaster_;


	/// <summary>
	/// デフォルトコントラクタ
	/// </summary>
	SoundManager(void) = default;

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~SoundManager(void) = default;

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	SoundManager(const SoundManager& other) = default;


	/// <summary>
	/// SE音声再生処理
	/// </summary>
	void SetSE(void);

	/// <summary>
	/// BGM音声再生処理
	/// </summary>
	void SetBGM(void);
};