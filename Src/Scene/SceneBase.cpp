#include "SceneBase.h"
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Common/Font.h"
#include "../Common/Vector2.h"


SceneBase::SceneBase(void)
{
}

bool SceneBase::IsLoad(void)
{
	// 非同期読み込み数が０になったら読み込み完了化
	if (GetASyncLoadNum() == 0) return true;

	return false;
}

bool SceneBase::IsCheck(void)
{
	bool ret = false;
	
	if (InputManager::GetInstance().PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::START) ||
		InputManager::GetInstance().PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::RIGHT) ||
		InputManager::GetInstance().PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::DOWN) ||

		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_SPACE) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_RETURN))
	{
		ret = true;
	}
	return ret;
}
bool SceneBase::IsCansel(void)
{
	bool ret = false;
	
	if (InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_ESCAPE) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_X) ||
		InputManager::GetInstance().PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::BACK) ||
		InputManager::GetInstance().PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::RIGHT))
	{
		ret = true;
	}

	return ret;
}

bool SceneBase::IsSelectUp(void)
{
	bool ret = false;
	
	if (InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::LEFT) &&
		InputManager::GetInstance().PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::LEFT) < 0 ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::LEFT) &&
		InputManager::GetInstance().PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::LEFT) < 0 ||

		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) &&
		InputManager::GetInstance().PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) < 0 ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) &&
		InputManager::GetInstance().PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) < 0 ||

		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::D_PAD) &&
		InputManager::GetInstance().PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::D_PAD) < 0 ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::D_PAD) &&
		InputManager::GetInstance().PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::D_PAD) < 0 ||

		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_W) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_A) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_LEFT) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_UP))
	{
		ret = true;
	}

	return ret;
}
bool SceneBase::IsSelectDown(void)
{
	bool ret = false;
	
	if (InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::LEFT) &&
		InputManager::GetInstance().PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::LEFT) > 0 ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::LEFT) &&
		InputManager::GetInstance().PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::LEFT) > 0 ||

		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) &&
		InputManager::GetInstance().PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) > 0 ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) &&
		InputManager::GetInstance().PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::RIGHT) > 0 ||

		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::D_PAD) &&
		InputManager::GetInstance().PadAlgKeyX(PAD_NO::PAD1, PAD_ALGKEY::D_PAD) > 0 ||
		InputManager::GetInstance().PadIsAlgKeyTrgDown(PAD_NO::PAD1, PAD_ALGKEY::D_PAD) &&
		InputManager::GetInstance().PadAlgKeyY(PAD_NO::PAD1, PAD_ALGKEY::D_PAD) > 0 ||

		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_S) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_D) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_RIGHT) ||
		InputManager::GetInstance().KeyIsTrgDown(KEY_INPUT_DOWN))
	{
		ret = true;
	}
	return ret;
}

void SceneBase::ChangeState(int& _state, int _maxState, int _max)
{
	if (IsSelectUp())
	{
		_state--;

		if (_state < 0)
		{
			_state = _maxState;
		}
	}

	if (IsSelectDown())
	{
		_state++;

		if (_state == _max)
		{
			_state = 0;
		}
	}
}

void SceneBase::DrawFontText(Vector2 _pos, int _size, unsigned int _color,
	                         const char* _text, bool _isSelect)
{
	Font& font = Font::GetInstance();

	Vector2 pos = _pos;

	// テキストの長さ
	int strWidth = font.GetDefaultTextWidth(_text);

	pos.x -= strWidth;

	// 状態が選択している時、
	if (_isSelect) { pos.x -= (_size / 2); }

	// テキスト描画
	font.DrawTextA("GameFont", pos.x, pos.y, _text, _color, _size, Font::FONT_TYPE_ANTIALIASING_EDGE);
}