#include "ConfigManager.h"
#include "./SoundManager.h"
#include "./InputManager.h"

ConfigManager* ConfigManager::instance_;

ConfigManager::ConfigManager(void)
{
}

void ConfigManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ConfigManager();
	}
}

void ConfigManager::Load(void)
{
	SoundManager& sound = SoundManager::GetInstance();

	sound.GetVolumeMaster();
}

void ConfigManager::Destroy(void)
{
	delete instance_;
}


void ConfigManager::Init(void)
{
	seVolume_ = 1.0f;
	bgmVolume_ = 1.0f;
}

void ConfigManager::Update(void)
{
	switch (configType_)
	{
		case TYPE::BACK:
		{

		}
		break;
	}

	ChangeConfigType();
}
void ConfigManager::DrawDebug(void)
{
}
void ConfigManager::ChangeConfigType(void)
{
	int type = static_cast<int>(configType_);

	if (InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_S) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_DOWN) ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::LEFT) ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::RIGHT))
	{
		type--;

		if (type <= 0) { type = (static_cast<int>(TYPE::MAX) - 1); }
	}

	if (InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_W) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_UP) ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::LEFT) ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::RIGHT))
	{
		type--;

		if (type <= 0) { type = (static_cast<int>(TYPE::MAX) - 1); }
	}

}