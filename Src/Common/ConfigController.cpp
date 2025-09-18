#include "ConfigController.h"
#include "../Manager/SoundManager.h"


ConfigController::ConfigController(void)
{
}

void ConfigController::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ConfigController();
	}
}

void ConfigController::Load(void)
{
	SoundManager& sound = SoundManager::GetInstance();

	sound.GetVolumeMaster();
}

void ConfigController::Init(void)
{
	seVolume_ = 1.0f;
	bgmVolume_ = 1.0f;
}
void ConfigController::Update(void)
{
}
void ConfigController::DrawDebug(void)
{
}