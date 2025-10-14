#include "GameClearScene.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Font.h"
#include "../Application.h"


GameClearScene::GameClearScene(void)
{
}

void GameClearScene::Load(void)
{
}

void GameClearScene::Init(void)
{
	state_ = STATE_CLEAR::TITLE;

}

void GameClearScene::Update(void)
{
	SoundManager& sound = SoundManager::GetInstance();
	SceneManager& scene = SceneManager::GetInstance();
	
	if (IsCheck())
	{
		switch (state_)
		{
			case STATE_CLEAR::TITLE:
			{
				sound.Play(SoundManager::SRC::SE_CLICK, false);

				scene.ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
			break;

			case STATE_CLEAR::RESTART:
			{
				sound.Play(SoundManager::SRC::SE_CLICK, false);

				scene.ChangeScene(SceneManager::SCENE_ID::GAME);
			}
			break;

			case STATE_CLEAR::GAME_END:
			{
				sound.Play(SoundManager::SRC::SE_CLICK, false);

				// ƒQ[ƒ€I—¹ˆ—
				Application::GetInstance().SetIsGameEnd();
			}
			break;
		}
	}

	// ó‘Ô‘JˆÚˆ—
	int state = static_cast<int>(state_);
	ChangeState(state, static_cast<int>(STATE_CLEAR::GAME_END), static_cast<int>(STATE_CLEAR::MAX));
	state_ = static_cast<STATE_CLEAR>(state);
}

void GameClearScene::Draw(void)
{
	Font& font = Font::GetInstance();

	// ”wŒi•`‰æ
	DrawGraph(0, 0, backImage_, true);

	/*
	//ƒtƒHƒ“ƒg‚Ì•`‰æ
	DrawFont();

	if (isViewInfo_)
	{
		DrawInfo();
	}

#ifdef _DEBUG
	std::string text = "TitleState:";
	switch (state_)
	{
	case TITLE_STATE::START_GAME:
		text += " GameStart";
		break;

	case TITLE_STATE::INFO:
		text += " Info";
		break;

	case TITLE_STATE::GAME_END:
		text += " GameEnd";
		break;
	}

	// ƒ^ƒCƒgƒ‹ó‘Ô
	DrawString(0, 64, text.c_str(), 0xFF0000);

#endif
*/
}

void GameClearScene::Release(void)
{
}
