#include "SoundManager.h"
#include <DxLib.h>
#include <string>
#include <cassert>
#include "./ResourceManager.h"
#include "./Resource.h"

SoundManager* SoundManager::instance_ = nullptr;

const std::string SoundManager::PATH_BGM_TITLE = ResourceManager::PATH_BGM + "BGMTitle.mp3";
const std::string SoundManager::PATH_BGM_GAME  = ResourceManager::PATH_BGM + "GameBGM.mp3";

const std::string SoundManager::PATH_SE_CLICK  = ResourceManager::PATH_SE + "Click.mp3";
const std::string SoundManager::PATH_SE_ATTACK = ResourceManager::PATH_SE + "Attack.mp3";
const std::string SoundManager::PATH_SE_KNOCK  = ResourceManager::PATH_SE + "KnockDown.mp3";

void SoundManager::CreateInstance(void)
{
	/*　インスタンス初期化処理　*/
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}

	// 初期化処理
	instance_->Load();
}


void SoundManager::Load(void)
{
	// 主音量割り当て
	volumeMaster_ = VOLUME_MASTER_MAX;

	// リストクリア
	sounds_.clear();

	// 各音声割り当て
	SetSounds();
}


void SoundManager::SetSounds(void)
{
	// SE

	SetSound(SRC::SE_CLICK, PATH_SE_CLICK, Sound::TYPE::SOUND_2D, VOLUME_CLICK);
	SetSound(SRC::SE_ATTACK, PATH_SE_ATTACK, Sound::TYPE::SOUND_2D, VOLUME_ATTACK);
	SetSound(SRC::SE_KNOCK, PATH_SE_KNOCK, Sound::TYPE::SOUND_2D, VOLUME_KNOCK);


	//　BGM

	SetSound(SRC::BGM_TITLE, PATH_BGM_TITLE, Sound::TYPE::SOUND_2D, VOLUME_TITLE);
	SetSound(SRC::BGM_GAME, PATH_BGM_GAME, Sound::TYPE::SOUND_2D, VOLUME_GAME);

}
void SoundManager::SetSound(SoundManager::SRC src, const std::string& path, Sound::TYPE type, float maxVolume)
{
	Sound res;

	res = Sound(type, path);
	res.SetMaxVolume(maxVolume);
	sounds_.emplace(src, res);
}


void SoundManager::Release(void)
{
	if (sounds_.empty()) return;

	for (auto& sound : sounds_)
	{
		// 各サウンドのメモリ解放
		sound.second.Release();
	}

	// リストをクリア
	sounds_.clear();
}

void SoundManager::Destroy(void)
{
	// メモリ解放
	Release();

	// リストクリア
	sounds_.clear();

	// インスタンス
	delete instance_;
}

bool SoundManager::Play(SRC src, Sound::TIMES times, bool isForce)
{
	const auto& lPair = sounds_.find(src);

	if (lPair == sounds_.end())
	{
#ifdef _DEBUG
		assert("\n2D再生する音声が割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}


	if (!lPair->second.IsLoad())
	{
		// 読み込み処理
		lPair->second.Load();
	}

	// 2Dサウンド再生処理
	return lPair->second.Play(times, isForce);
}
bool SoundManager::Play(SRC src, Sound::TIMES times, VECTOR pos, float radius)
{
	const auto& lPair = sounds_.find(src);

	if (lPair == sounds_.end())
	{
#ifdef _DEBUG
		assert("\n3D再生する音声が割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}


	if (!lPair->second.IsLoad())
	{
		// 読み込み処理
		lPair->second.Load();
	}

	// 3Dサウンド再生処理
	return lPair->second.Play(times, pos, radius);
}

void SoundManager::Stop(SRC src)
{
	const auto& listPair = sounds_.find(src);

	// 判定対象
	if (listPair == sounds_.end())
	{
#ifdef _DEBUG
		OutputDebugString("\n停止する音声が割り当てられていません\n(；_；)\n");
		assert(false); // 例外スロー
#endif
		return;
	}

	// 指定の音声を停止
	return listPair->second.Stop();
}

void SoundManager::StopAll(void)
{
	if (sounds_.empty()) return;

	for (auto& sound : sounds_)
	{
		if (!sound.second.IsLoad())
		{
			// 未読み込み時、スキップ
			continue;
		}

		// リスト内のサウンドを全て停止
		sound.second.Stop();
	}
}

void SoundManager::SetVolume(SRC src, float per)
{
	const auto& lPair = sounds_.find(src);

	if (lPair != sounds_.end())
	{
		// 音声があるとき、音量を割り当て
		return lPair->second.SetVolume(per);
	}

#ifdef _DEBUG
	OutputDebugString("\nサウンドが割り当てられていません\n(；_；)\n");
	assert(false); // 例外スロー
#endif
}

bool SoundManager::IsSoundStart(SRC src)
{
	const auto& soundlist = sounds_.find(src);

	if (soundlist == sounds_.end())
	{
#ifdef _DEBUG
		assert("\n停止するサウンドが割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}

	// 音声があるとき、再生か判定
	return soundlist->second.IsStart();
}

bool SoundManager::IsSoundPlay(SRC src)
{
	const auto& soundlist = sounds_.find(src);

	if (soundlist == sounds_.end())
	{
#ifdef _DEBUG
		assert("\n停止するサウンドが割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}

	// 音声があるとき、再生か判定
	return soundlist->second.IsPlay();
}

bool SoundManager::IsSoundEnd(SRC src)
{
	const auto& soundlist = sounds_.find(src);

	if (soundlist == sounds_.end())
	{
#ifdef _DEBUG
		assert("\n停止するサウンドが割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}

	// 音声があるとき、再生か判定

	return soundlist->second.IsEnd();;
}

void SoundManager::SetVolumeMaster(float volume)
{
	float vol = volume;
	
	// 音量が０未満の時、音量を０にする
	vol = ((vol < 0.0f) ? 0.0f : vol);

	// 音量が最大値を超えた時、音量を最大値にする
	vol = ((volume > VOLUME_MASTER_MAX) ? VOLUME_MASTER_MAX : vol);

	vol = static_cast<int>(volume * VOLUME_MASTER_MAX);

	// 音量割り当て
	volumeMaster_ = vol;
}