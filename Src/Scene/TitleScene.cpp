#include "TitleScene.h"
#include <string>
#include <DxLib.h>

#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Common/Font.h"
#include "../Application.h"
#include "../Common/Font.h"
#include "../Utility/AsoUtility.h"


using PAD_ALGKEY = InputManager::JOYPAD_ALGKEY;
using PAD_BTN = InputManager::PAD_BTN;
using PAD_NO = InputManager::JOYPAD_NO;

TitleScene::TitleScene(void)
{
	titleImage_ = 0;		//タイトル画像

	state_ = TITLE_STATE::START_GAME;

	info_ = INFO_TYPE::PLAY_PAD;
}

void TitleScene::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();

	/*
	pv_ = res.LoadHandleId(ResourceManager::SRC::MOVIE_PV);

	//タイトル画像
	titleImage_ = res.LoadHandleId(ResourceManager::SRC::IMAGE_TITLE);

	// パッド画像
	padImage_ = res.LoadHandleId(ResourceManager::SRC::IMAGE_PAD);

	// キーボード画像
	keyImage_ = res.LoadHandleId(ResourceManager::SRC::IMAGE_KEYBOARD);

	// ボタン画像
	buttonImage_ = res.LoadHandleId(ResourceManager::SRC::IMAGE_TITLE_BUTTON);

	// 矢印画像
	arrowImage_ = res.LoadHandleId(ResourceManager::SRC::IMAGE_ARROW);
	*/
}

void TitleScene::Init(void)
{
	state_ = TITLE_STATE::START_GAME;
	isViewInfo_ = false; // 遊び方表示

	SoundManager::GetInstance().Play(SoundManager::SRC::BGM_TITLE, true);

	selectScale_ = 0.35f;
	isPvActive_ = false;
	pvTime_ = 0.0f;
}

void TitleScene::Update(void)
{
	SoundManager& sound = SoundManager::GetInstance();
	SceneManager& scene = SceneManager::GetInstance();

	float delta = SceneManager::GetInstance().GetDeltaTime();

	arrowPerformTime_ -= delta;
	if (arrowPerformTime_ < 0.0f)
	{
		arrowPerformTime_ = ARROW_PERFORM_TIME;

		selectScale_ = ((selectScale_ != 0.5f) ? 0.5f : 0.35f);

		arrowScale_ = ((arrowScale_ != ARROW_SCALE) ? ARROW_SCALE : 1.0f);
	}

	if (IsCheck() && !isPvActive_)
	{
		switch (state_)
		{
		case TITLE_STATE::START_GAME:
		{
			sound.Play(SoundManager::SRC::SE_CLICK, false);

			Application::GetInstance().SetIsExitMenu(true);

			scene.ChangeScene(SceneManager::SCENE_ID::GAME);
		}
		break;

		case (TITLE_STATE::INFO):
		{
			// 遊び方描画フラグ変更処理
			isViewInfo_ = true;

			Application::GetInstance().SetIsExitMenu(false);

			info_ = INFO_TYPE::PLAY_PAD;

			// 演出時間割り当て
			arrowPerformTime_ = ARROW_PERFORM_TIME;

			// 矢印の大きさ割り当て
			arrowScale_ = ARROW_SCALE;
		}
		break;

		case (TITLE_STATE::GAME_END):
		{
			// ゲーム終了処理
			Application::GetInstance().SetIsGame(false);
		}
		break;
		}
	}


	int state = static_cast<int>(state_);

	if (!isViewInfo_ && !isPvActive_)
	{
		if (IsSelectUp())
		{
			state--;

			if (state < 0)
			{
				state = static_cast<int>(TITLE_STATE::GAME_END);
			}
		}

		if (IsSelectDown())
		{
			state++;

			if (state == static_cast<int>(TITLE_STATE::MAX))
			{
				state = 0;
			}
		}
	}
	else
	{
		UpdateInfo();
	}

	// 状態割り当て
	state_ = static_cast<TITLE_STATE>(state);


	pvTime_ += delta;
	if (pvTime_ > 60.0f || CheckHitKey(KEY_INPUT_TAB))
	{
		if (!isPvActive_)
		{
			isPvActive_ = true;

			// 最初から再生
			SeekMovieToGraph(pv_, 0);
			PlayMovieToGraph(pv_);
			sound.Stop(SoundManager::SRC::BGM_TITLE);
		}
	}
	if (isPvActive_)
	{
		if (IsCheck() || GetMovieStateToGraph(pv_) == 0)
		{
			// 動画停止
			PauseMovieToGraph(pv_);
			isPvActive_ = false;
			pvTime_ = 0.0f;
			sound.Play(SoundManager::SRC::BGM_TITLE, true);
		}
	}
}

void TitleScene::Draw(void)
{
	Font& font = Font::GetInstance();

	// タイトルロゴ描画
	DrawGraph(0, 0, titleImage_, true);

	//フォントの描画
	DrawFont();

	if (isViewInfo_)
	{
		DrawInfo();
	}

	if (isPvActive_)
	{
		DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
			pv_, false);

		// テキストの長さ
		int strWidth = font.GetDefaultTextWidth("決定でタイトルに戻る");

		int x = (Application::SCREEN_SIZE_X)-425;
		int y = (Application::SCREEN_SIZE_Y)-64;

		// テキスト描画
		font.DrawTextA("GameFont", x, y, "決定でタイトルに戻る",
			0xff5555, 40, Font::FONT_TYPE_ANTIALIASING_EDGE);
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


	// ゲーム開始テキスト
	DrawTitieText(yOffset, "ゲームスタート", TITLE_STATE::START_GAME);
	yOffset += TEXT_POS_Y_OFFSET;

	// 操作説明テキスト
	DrawTitieText(yOffset, "操作説明", TITLE_STATE::INFO);
	yOffset += TEXT_POS_Y_OFFSET;

	// ゲーム終了テキスト
	DrawTitieText(yOffset, "ゲーム終了", TITLE_STATE::GAME_END);

	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_INVSRC, 200);

	float y = Application::SCREEN_HALF_Y + TEXT_POS.y + 10;
	DrawRotaGraph(Application::SCREEN_HALF_X,
		(y - (95 / 4) - 5 + (TEXT_POS_Y_OFFSET * static_cast<int>(state_))),
		selectScale_, AsoUtility::Deg2Rad(-90.0), arrowImage_, true);

	DrawRotaGraph(Application::SCREEN_HALF_X,
		(y + (95 / 2) + (TEXT_POS_Y_OFFSET * static_cast<int>(state_))),
		selectScale_, AsoUtility::Deg2Rad(90.0), arrowImage_, true);

	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::DrawOperation(void)
{
}

void TitleScene::DrawExplanation(void)
{
}

void TitleScene::DrawTitieText(int posY, const char* text, TitleScene::TITLE_STATE state)
{
	Font& font = Font::GetInstance();

	int size = TEXT_SIZE_DEFAULT;
	unsigned int color = TEXT_COLOR_DEFAULT;

	// テキストの長さ
	int strWidth = font.GetDefaultTextWidth(text);

	int x = (Application::SCREEN_HALF_X - strWidth);
	int y = (Application::SCREEN_HALF_Y + TEXT_POS.y + posY);

	// 状態が選択している時、
	if (state_ == state)
	{
		// テキストサイズ変更
		size = TEXT_SIZE_SELECT;

		// テキスト色変更
		color = TEXT_COLOR_SELECT;

		x -= TEXT_SIZE_SELECT / 2;
	}

	// テキスト描画
	font.DrawTextA("GameFont", x, y, text, color, size, Font::FONT_TYPE_ANTIALIASING_EDGE);
}

void TitleScene::UpdateInfo(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();
	int info = static_cast<int>(info_);

	// 選択処理
	if (IsSelectUp())
	{
		info--;

		if (info < 0)
		{
			info = static_cast<int>(INFO_TYPE::MAX) - 1;
		}
	}
	if (IsSelectDown())
	{
		info++;

		if (info == static_cast<int>(INFO_TYPE::MAX))
		{
			info = 0;
		}
	}
	// 状態割り当て
	info_ = static_cast<INFO_TYPE>(info);


	if (IsCansel())
	{
		// 終了メニュー有効化
		Application::GetInstance().SetIsExitMenu(true);

		// 遊び方非表示
		isViewInfo_ = false;
	}
}

void TitleScene::DrawInfo(void)
{
	/*　遊び方描画　*/
	Font& font = Font::GetInstance();

	int x = 0;
	int y = 0;
	// 中心座標
	int midX = Application::SCREEN_HALF_X;
	int midY = Application::SCREEN_HALF_Y;

	// 背景 半透明描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, INFO_COLOR.a);

	// 背景
	DrawBox(INFO_SIZE.x, INFO_SIZE.y,
		(midX * 2) - INFO_SIZE.x,
		(midY * 2) - INFO_SIZE.y,
		GetColor(INFO_COLOR.r, INFO_COLOR.g, INFO_COLOR.b), true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 矢印
	DrawRotaGraph(midX - ARROW_WIDTH, midY,
		arrowScale_, 0.0f, arrowImage_, true, true);
	DrawRotaGraph(midX + ARROW_WIDTH, midY,
		arrowScale_, 0.0f, arrowImage_, true, false);

	if (info_ == INFO_TYPE::PLAY_PAD)
	{
		// パッド画像
		DrawRotaGraph(midX, midY, 1.0, 0.0, padImage_, true);
	}
	else if (info_ == INFO_TYPE::PLAY_KEY)
	{
		// キーボード画像
		DrawRotaGraph(midX, midY, 1.0, 0.0, keyImage_, true);
	}


	x = (midX - INFO_PARAM.x);
	y = (((midY * 2) - INFO_SIZE.y) - INFO_PARAM.y);

	// テキスト描画
	int now = static_cast<int>(info_) + 1;
	int max = static_cast<int>(INFO_TYPE::MAX);
	std::string text = (std::to_string(now) + " / " + std::to_string(max));
	font.DrawTextA(Font::FONT_GAME, x, y, text.c_str(),
		0xFFFFFF, -1, Font::FONT_TYPE_ANTIALIASING);


	x = (INFO_SIZE.x + INFO_OFFSET.x);
	y = (INFO_SIZE.y + INFO_OFFSET.y);

	// テキスト描画
	font.DrawTextA(Font::FONT_GAME, x, y, "※ ※ ※ ※　 このゲームはコントローラ操作を推奨しています。　※ ※ ※ ※",
		0xFF0000, -1, Font::FONT_TYPE_ANTIALIASING);
}


bool TitleScene::IsSelectUp(void)
{
	bool ret = false;
	InputManager& input = InputManager::GetInstance();

	if (input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::LEFT) &&
		input.PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::LEFT) < 0 ||
		input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::LEFT) &&
		input.PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::LEFT) < 0 ||
		input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) &&
		input.PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) < 0 ||
		input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) &&
		input.PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) < 0 ||

		input.KeyIsTrgDown(KEY_INPUT_W) ||
		input.KeyIsTrgDown(KEY_INPUT_A) ||
		input.KeyIsTrgDown(KEY_INPUT_LEFT) ||
		input.KeyIsTrgDown(KEY_INPUT_UP))
	{
		ret = true;
	}

	return ret;
}
bool TitleScene::IsSelectDown(void)
{
	bool ret = false;
	InputManager& input = InputManager::GetInstance();

	if (input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::LEFT) &&
		input.PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::LEFT) > 0 ||
		input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::LEFT) &&
		input.PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::LEFT) > 0 ||
		input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) &&
		input.PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) > 0 ||
		input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) &&
		input.PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) > 0 ||

		input.KeyIsTrgDown(KEY_INPUT_S) ||
		input.KeyIsTrgDown(KEY_INPUT_D) ||
		input.KeyIsTrgDown(KEY_INPUT_RIGHT) ||
		input.KeyIsTrgDown(KEY_INPUT_DOWN))
	{
		ret = true;
	}
	return ret;
}
bool TitleScene::IsCheck(void)
{
	bool ret = false;
	InputManager& input = InputManager::GetInstance();

	if (input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::START) ||
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::RIGHT) ||
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::DOWN) ||

		input.KeyIsTrgDown(KEY_INPUT_SPACE) ||
		input.KeyIsTrgDown(KEY_INPUT_RETURN))
	{
		ret = true;
	}
	return ret;
}
bool TitleScene::IsCansel(void)
{
	bool ret = false;
	InputManager& input = InputManager::GetInstance();

	if (input.KeyIsTrgDown(KEY_INPUT_ESCAPE) ||
		input.KeyIsTrgDown(KEY_INPUT_X) ||
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::BACK) ||
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::RIGHT))
	{
		ret = true;
	}

	return ret;
}