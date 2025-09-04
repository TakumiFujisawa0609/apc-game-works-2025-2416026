#pragma once
#include <string>
#include <DxLib.h>

class Sound
{
public:

	enum class TYPE
	{
		NONE,
		SOUND_2D, // 2D音響
		SOUND_3D, // 3D音響
	};

	enum class TIMES
	{
		NONE,
		ONCE, // １回
		LOOP, // 繰り返し
	};


	struct SOUND
	{
		// 再生する種類
		TYPE type;

		// リソース
		std::string path;

		// ハンドルID
		int handle;

		// 最大音声
		int maxVolume;

		// 再生位置(3D用)
		VECTOR pos;

		// 有効範囲(3D用)
		float radius;

		// 再生しているか否か
		bool isPlay;

		// 再生しているか否か
		bool isPlayOld;
	};


	Sound(void);

	Sound(TYPE type, const std::string& path);

	~Sound(void) = default;


	/// <summary>
	/// 3D座標更新処理
	/// </summary>
	/// <param name="pos"></param>
	void Update(VECTOR pos);

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);


	/// <summary>
	/// 2Dサウンドの再生処理
	/// </summary>
	/// <param name="times">再生種類を取得</param>
	/// <param name="isForce"></param>
	bool Play(TIMES times, bool isForce = false);

	/// <summary>
	/// 3Dサウンドの再生処理
	/// </summary>
	/// <param name="times">再生種類を取得</param>
	/// <param name="pos">音声が出てくる座標</param>
	/// <param name="radius">音声が聞こえる範囲</param>
	bool Play(TIMES times, VECTOR pos, float radius);

	/// <summary>
	/// 停止処理
	/// </summary>
	void Stop(void);

	/// <summary>
	/// サウンドがあるか判定
	/// </summary>
	bool CheckSoundHandle(void);

	/// <summary>
	/// 音声が読み込まれているか否か
	/// </summary>
	bool IsLoad(void);


	/// <summary>
	/// 現在の音量を割り当て
	/// </summary>
	/// <param name="per">音量</param>
	void SetVolume(float per = 1.0f);

	/// <summary>
	/// 最大音量を割り当て
	/// </summary>
	/// <param name="per">最大音量</param>
	void SetMaxVolume(float per = 1.0f);


	/// <summary>
	/// 再生を開始したか判定
	/// </summary>
	bool IsStart(void);

	/// <summary>
	/// 再生を開始したか判定
	/// </summary>
	bool IsPlay(void);

	bool IsEnd(void);


private:

	SOUND sound_;
};