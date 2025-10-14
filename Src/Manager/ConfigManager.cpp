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
	InputManager& input = InputManager::GetInstance();
	int type = static_cast<int>(configType_);

	if (input.KeyIsTrgDown(KEY_INPUT_S) ||
		input.KeyIsTrgDown(KEY_INPUT_DOWN) ||
		input.PadIsAlgKeyTrgDown(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::LEFT) ||
		input.PadIsAlgKeyTrgDown(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::RIGHT))
	{
		type--;

		if (type <= 0) { type = (static_cast<int>(TYPE::MAX) - 1); }
	}

	if (input.KeyIsTrgDown(KEY_INPUT_W) ||
		input.KeyIsTrgDown(KEY_INPUT_UP) ||
		input.PadIsAlgKeyTrgDown(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::LEFT) ||
		input.PadIsAlgKeyTrgDown(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::RIGHT))
	{
		type--;

		if (type <= 0) { type = (static_cast<int>(TYPE::MAX) - 1); }
	}

}