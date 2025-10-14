#include "GameExit.h"
#include "./Font.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include <DxLib.h>

using PAD_ALGKEY = InputManager::JOYPAD_ALGKEY;
using PAD_BTN = InputManager::PAD_BTN;
using PAD_NO = InputManager::PAD_NO;


GameExit::GameExit(void)
{
	// 確認状態
	select_ = SELECT::NONE;

	// ゲーム終了 有効化フラグ
	isEnd_ = false;

	// 終了選択 有効化フラグ
	isActiveMenu_ = false;

	Init();
}


void GameExit::Init(void)
{
	/*　初期化処理　*/

	// 終了無効 状態化
	select_ = SELECT::NO;

	// 終了無効化
	isEnd_ = false;

	// メニュー 無効化
	isActiveMenu_ = false;
}

void GameExit::Update(void)
{
	/*　更新処理　*/
	SoundManager& sound = SoundManager::GetInstance();
	SceneManager& scene = SceneManager::GetInstance();

	if (isActiveMenu_)
	{
		/*　各選択キー 更新処理　*/

		/*　選択状態 遷移処理　*/
		if (IsSelect())
		{
			// YES状態とNO状態を切り替える
			select_ = ((select_ == SELECT::NO) ? SELECT::YES : SELECT::NO);
		}

		if (IsCheck())
		{
			// ゲーム終了処理
			if (select_ == SELECT::YES)
			{
				isActiveMenu_ = false;

				switch (scene.GetSceneId())
				{
					case SceneManager::SCENE_ID::TITLE:
					{
						// ゲーム終了
						Application::GetInstance().SetIsGameEnd();
					}
					break;

					case SceneManager::SCENE_ID::GAME:
					{
						// タイトルに戻る
						scene.ChangeScene(SceneManager::SCENE_ID::TITLE);
					}
					break;
				}
			}
			else
			{
				// メニュー非表示
				isActiveMenu_ = false;
			}
		}
	}

	if (IsActive())
	{
		if (isActiveMenu_)
		{
			// 終了確認画面 無効化
			isActiveMenu_ = false;

			// 音量を通常に戻す
			sound.SetVolumeMaster(SoundManager::VOLUME_MASTER_MAX);
		}
		else
		{
			// メニュー画面 有効化
			isActiveMenu_ = true;

			// 終了無効 状態化
			select_ = SELECT::NO;

			// 音量を半減
			sound.SetVolumeMaster(sound.GetVolumeMaster() / 2);
		}
	}
}

void GameExit::Draw(void)
{
	// メニュー未有効時、処理終了
	if (!isActiveMenu_) return;

	Font& font = Font::GetInstance();
	SceneManager& scene = SceneManager::GetInstance();

	// 画面の中央座標
	int midX = Application::SCREEN_HALF_X;
	int midY = Application::SCREEN_HALF_Y;

	// 決定テキスト位置
	int yesX = ((midX - TEXT_OFFSET.x) - font.GetDefaultTextWidth(TEXT_YES));
	int yesY = (midY + TEXT_OFFSET.y);

	// キャンセルテキスト位置
	int noX = ((midX + TEXT_OFFSET.x) - font.GetDefaultTextWidth(TEXT_NO));
	int noY = (midY + TEXT_OFFSET.y);



	// 背景 半透明描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MENU_COLOR.a);
	DrawBox(0, Application::SCREEN_SIZE_X,
			0, Application::SCREEN_SIZE_Y,
			GetColor(MENU_COLOR.r, MENU_COLOR.g, MENU_COLOR.b), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// メニュー画面 半透明描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MENU_COLOR.a);
	DrawBox((midX - MENU_WIDTH), (midY - MENU_HEIGHT),
			(midX + MENU_WIDTH), (midY + MENU_HEIGHT),
			GetColor(MENU_COLOR.r, MENU_COLOR.g, MENU_COLOR.b), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	// メニューテキスト表示
	const char* text = TEXT_LABEL_END;

	if (scene.GetSceneId() != SceneManager::SCENE_ID::TITLE)
	{
		// タイトル遷移テキスト
		text = TEXT_LABEL_TITLE;
	}

	int x = midX - font.GetDefaultTextWidth(text) + MENU_TEXT_OFFSET.x;
	int y = (midY + MENU_TEXT_OFFSET.y);
	font.DrawTextA("GameFont", x, y, text,
				   GetColor(MENU_TEXT_COLOR.r, MENU_TEXT_COLOR.g, MENU_TEXT_COLOR.b),
				   50, Font::FONT_TYPE_ANTIALIASING_EDGE);


	if (select_ == SELECT::YES)
	{
		// "YES"選択 通常描画
		font.DrawTextA("GameFont", yesX, yesY, TEXT_YES,
					   GetColor(MENU_TEXT_COLOR.r, MENU_TEXT_COLOR.g, MENU_TEXT_COLOR.b),
					   -1, Font::FONT_TYPE_ANTIALIASING_EDGE);

		// "NO"選択 半透明描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MENU_TEXT_COLOR.a);

		font.DrawTextA("GameFont", noX, noY, TEXT_NO,
					   GetColor(MENU_TEXT_COLOR.r, MENU_TEXT_COLOR.g, MENU_TEXT_COLOR.b),
					   -1, Font::FONT_TYPE_ANTIALIASING_EDGE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	else if (select_ == SELECT::NO)
	{
		// "NO"選択 通常描画
		font.DrawTextA("GameFont", noX, noY, TEXT_NO,
					   GetColor(MENU_TEXT_COLOR.r, MENU_TEXT_COLOR.g, MENU_TEXT_COLOR.b),
					   -1, Font::FONT_TYPE_ANTIALIASING_EDGE);

		// "YES"選択 半透明描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, MENU_TEXT_COLOR.a);
		font.DrawTextA("GameFont", yesX, yesY, TEXT_YES,
					   GetColor(MENU_TEXT_COLOR.r, MENU_TEXT_COLOR.g, MENU_TEXT_COLOR.b),
					   -1, Font::FONT_TYPE_ANTIALIASING_EDGE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void GameExit::Release(void)
{
	/*　解放処理　*/
}


bool GameExit::GetIsActiveMenu(void)const
{
	// 終了確認画面有効判定 取得

	return isActiveMenu_;
}


bool GameExit::IsActive(void)
{
	bool ret = false;
	InputManager& input = InputManager::GetInstance();

	if (input.KeyIsTrgDown(KEY_INPUT_ESCAPE) ||
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::BACK))
	{
		ret = true;
	}

	return ret;
}

bool GameExit::IsSelect(void)
{
	bool ret = false;
	InputManager& input = InputManager::GetInstance();

	if (input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::LEFT)  ||
		input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::D_PAD) ||
		input.PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) ||

		input.KeyIsTrgDown(KEY_INPUT_W) || input.KeyIsTrgDown(KEY_INPUT_UP) || input.KeyIsTrgDown(KEY_INPUT_O) ||
		input.KeyIsTrgDown(KEY_INPUT_S) || input.KeyIsTrgDown(KEY_INPUT_DOWN) || input.KeyIsTrgDown(KEY_INPUT_L) ||
		input.KeyIsTrgDown(KEY_INPUT_A) || input.KeyIsTrgDown(KEY_INPUT_LEFT) || input.KeyIsTrgDown(KEY_INPUT_K) ||
		input.KeyIsTrgDown(KEY_INPUT_D) || input.KeyIsTrgDown(KEY_INPUT_RIGHT) || input.KeyIsTrgDown(KEY_INPUT_SEMICOLON))
	{
		ret = true;
	}

	return ret;
}

bool GameExit::IsCheck(void)
{
	bool ret = false;
	InputManager& input = InputManager::GetInstance();

	if (input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::START) ||
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::BACK)  ||
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::RIGHT) ||
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::DOWN)  ||

		input.KeyIsTrgDown(KEY_INPUT_SPACE) ||
		input.KeyIsTrgDown(KEY_INPUT_RETURN))
	{
		ret = true;
	}
	return ret;
}