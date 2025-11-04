#include "GameClearScene.h"
#include "./SceneBase.h"
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
	if (IsCheck())
	{
		switch (state_)
		{
			case STATE_CLEAR::TITLE:
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::SE_CLICK, false);

				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
			break;

			case STATE_CLEAR::RESTART:
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::SE_CLICK, false);

				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			}
			break;

			case STATE_CLEAR::GAME_END:
			{
				SoundManager::GetInstance().Play(SoundManager::SRC::SE_CLICK, false);

				// ゲーム終了処理
				Application::GetInstance().SetIsGameEnd();
			}
			break;
		}
	}

	// 状態遷移処理
	int state = static_cast<int>(state_);
	ChangeState(state, static_cast<int>(STATE_CLEAR::GAME_END), static_cast<int>(STATE_CLEAR::MAX));
	state_ = static_cast<STATE_CLEAR>(state);
}

void GameClearScene::Draw(void)
{
	Font& font = Font::GetInstance();

	// 背景描画
	DrawGraph(0, 0, backImage_, true);

	
	//フォントの描画
	DrawFontText({ Application::SCREEN_HALF_X - 110, Application::SCREEN_HALF_Y }, 60, 0xffffff, "ゲームクリア", false);

	DrawFontText({ Application::SCREEN_HALF_X - 50, Application::SCREEN_HALF_Y +  86}, 30, 0xffffff, "決定でタイトルに戻る", false);


	/*
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

	// タイトル状態
	DrawString(0, 64, text.c_str(), 0xFF0000);

#endif
*/
}

void GameClearScene::Release(void)
{
}
