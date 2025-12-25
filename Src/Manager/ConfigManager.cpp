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

	if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_DOWN) ||
		InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_RIGHT))
	{
		--type = ((type <= 0) ? (static_cast<int>(TYPE::MAX) - 1) : type);
	}

	if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_UP) ||
		InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_LEFT))
	{
		++type = ((type >= static_cast<int>(TYPE::MAX)) ? 0 : type);
	}

}