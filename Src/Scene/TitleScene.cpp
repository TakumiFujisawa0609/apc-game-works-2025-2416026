#include "TitleScene.h"
#include <string>
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Common/Font.h"
#include "../Common/Camera.h"
#include "../Application.h"
#include "../Utility/AsoUtility.h"


TitleScene::TitleScene(void) :
	titleImage_(-1),
	state_(TITLE_STATE::START_GAME),
	SceneBase()
{
	Load();
}

void TitleScene::Load(void)
{
	//タイトル画像
	titleImage_ = resMng_.LoadHandleId(ResourceManager::SRC::IMG_TITLE);

	// PV
	pv_ = resMng_.LoadHandleId(ResourceManager::SRC::MOVIE_PV);
}

void TitleScene::Init(void)
{
 	state_ = TITLE_STATE::START_GAME;
	stateWaitTime_ = STATE_WAIT_TIME;

	selectScale_ = 0.35f;
	isPvActive_ = false;
	pvTime_ = 0.0f;
	sceneMng_.GetCamera().Init(Camera::MODE::FIXEX_POINT);
}

void TitleScene::Update(void)
{
	stateWaitTime_ -= sceneMng_.GetDeltaTime();;

	if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		if (isPvActive_)
		{
			isPvActive_ = false;
			return;
		}

		else if (GetIsActiveState())
		{
			SoundManager::GetInstance().Play(SoundManager::SRC::SE_CLICK, false);

			switch (state_)
			{
				case TITLE_STATE::START_GAME:
				{
					SoundManager::GetInstance().Play(SoundManager::SRC::SE_CLICK, false);

					Application::GetInstance().SetIsExitMenu(true);

					sceneMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
					return;
				}
				break;


				case TITLE_STATE::GAME_END:
				{
					// ゲーム終了処理
					Application::GetInstance().SetIsGameEnd();
				}
				break;
			}
		}
	}


	int state = static_cast<int>(state_);

	if (!isPvActive_)
	{
		ChangeState(state, static_cast<int>(TITLE_STATE::GAME_END), static_cast<int>(TITLE_STATE::MAX));
	}

	// 状態割り当て
	state_ = static_cast<TITLE_STATE>(state);

	// PV
	PromotionVideo();
}

void TitleScene::Draw(void)
{
	Font& font = Font::GetInstance();

	// タイトル描画
	const Vector2 TITLE_POS = {Application::SCREEN_HALF_X, 300};
	DrawRotaGraph(TITLE_POS.x, TITLE_POS.y,
				  2.0, 0.0, titleImage_, true);

	//フォントの描画
	DrawFont();

	if (isPvActive_)
	{
		DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
			pv_, false);

		// テキストの長さ
		int strWidth = font.GetDefaultTextWidth("決定でタイトルに戻る");

		int x = (Application::SCREEN_SIZE_X)-425;
		int y = (Application::SCREEN_SIZE_Y)-40;

		// テキスト描画
		font.DrawTextA("GameFont", x, y, "決定でタイトルに戻る",
			0xff5555, 30, Font::FONT_TYPE_ANTIALIASING_EDGE);
	}

#ifdef _DEBUG
	/*
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
	*/
#endif
}

void TitleScene::Release(void)
{

}

void TitleScene::FontSettings(void)
{
}

void TitleScene::DrawFont(void)
{
	int yOffset = 0;
	//unsigned int color;

	// ゲーム開始テキスト
	DrawTitleText(yOffset, "ゲームスタート", TITLE_STATE::START_GAME);
	
	// 操作説明テキスト
	//DrawTitleText(yOffset, "操作説明", TITLE_STATE::INFO);


	// ゲーム終了テキスト
	DrawTitleText(yOffset, "ゲーム終了", TITLE_STATE::GAME_END);
}

void TitleScene::DrawOperation(void)
{
}

void TitleScene::DrawExplanation(void)
{
}

void TitleScene::DrawTitleText(int& _posY, const char* _text, TitleScene::TITLE_STATE _state)
{
	Vector2 pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };
	int size = TEXT_SIZE_DEFAULT;
	unsigned int color = TEXT_COLOR_DEFAULT;

	if (state_ == _state)
	{
		size = TEXT_SIZE_SELECT;
		color = TEXT_COLOR_SELECT;
	}
	pos.y += (TEXT_POS.y + _posY);
	_posY += TEXT_POS_Y_OFFSET;

	// フォントのテキスト描画
	DrawFontText(pos, size, color, _text, (state_ == _state));
}

void TitleScene::PromotionVideo(void)
{
	pvTime_ += sceneMng_.GetDeltaTime();

	if (pvTime_ > 60.0f || CheckHitKey(KEY_INPUT_TAB))
	{
		if (!isPvActive_)
		{
			isPvActive_ = true;

			// 最初から再生
			SeekMovieToGraph(pv_, 0);
			PlayMovieToGraph(pv_);
		}
	}
	if (isPvActive_)
	{
		if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_DECISION) || GetMovieStateToGraph(pv_) == 0)
		{
			// 動画停止
			PauseMovieToGraph(pv_);
			isPvActive_ = false;
			pvTime_ = 0.0f;
		}
	}
}
