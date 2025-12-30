#include "GameClearScene.h"
#include "./SceneBase.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Font.h"
#include "../Application.h"


GameClearScene::GameClearScene(void):
	SceneBase::SceneBase()
{
	Load();
}

void GameClearScene::Load(void)
{
	clearHandle_ = resMng_.LoadHandleId(ResourceManager::SRC::IMG_CLEAR);
}

void GameClearScene::Init(void)
{
	state_ = STATE_CLEAR::TITLE;

}

void GameClearScene::Update(void)
{	
	if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_DECISION))
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
	const Vector2 CLEAR_POS = { Application::SCREEN_HALF_X, 350 };
	DrawRotaGraph(CLEAR_POS.x, CLEAR_POS.y,
				  2.0, 0.0, clearHandle_, true);

	//フォントの描画
	const Vector2 TEXT_POS = { Application::SCREEN_HALF_X , Application::SCREEN_HALF_Y + 225 };
	DrawFontText(TEXT_POS, 30, 0xffffff, "決定でタイトルに戻る", false);


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
