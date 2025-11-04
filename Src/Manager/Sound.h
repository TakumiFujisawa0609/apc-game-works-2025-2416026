#pragma once
#include <string>
#include <DxLib.h>

class Sound
{
public:

	enum class TYPE
	{
		NONE = -1,
		SOUND_2D, // 2D音響
		SOUND_3D, // 3D音響
	};

	enum class TIMES
	{
		NONE = -1,
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


	/// @brief 3D座標更新処理
	/// @param pos
	void Update(VECTOR pos);

	/// @brief 読み込み処理
	void Load(void);

	/// @brief 解放処理
	void Release(void);


	/// @brief 2Dサウンドの再生処
	/// @param name times 再生種類を取得
	/// @param name isForce
	/// @returns 音声未割当時、false
	bool Play(TIMES times, bool _isPitch = false, float _pitchRange = 1.0f, bool isForce = false);

	/// @brief 3Dサウンドの再生処
	/// @param times 再生種類を取得
	/// @param pos 音声が出てくる座標
	/// @param radius 音声が聞こえる範囲
	bool Play(TIMES times, VECTOR pos, float radius);

	/// @brief 停止処理
	void Stop(void);

	/// @brief サウンドがあるか判定
	bool CheckSoundHandle(void);

	/// @brief 音声が読み込まれているか否か
	bool IsLoad(void);


	/// @brief 現在の音量を割り当て
	/// @param per 音量
	void SetVolume(float _volume = 1.0f);

	/// @brief 最大音量を割り当て
	/// @param _volume 最大音量
	void SetMaxVolume(float _volume = 1.0f);


	/// @brief 再生を開始したか判定
	bool IsStart(void);

	/// @brief 再生を開始したか判定
	bool IsPlay(void);

	/// @brief 再生が終了したか判定
	bool IsEnd(void);


private:

	SOUND sound_;
};