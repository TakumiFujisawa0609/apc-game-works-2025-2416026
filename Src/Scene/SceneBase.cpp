#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "SceneBase.h"
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Common/Font.h"
#include "../Common/Vector2.h"


SceneBase::SceneBase(void):
	resMng_(ResourceManager::GetInstance()),
	sceneMng_(SceneManager::GetInstance())
{
}

bool SceneBase::IsLoad(void)
{
	// 非同期読み込み数が０になったら読み込み完了化
	if (GetASyncLoadNum() == 0) return true;

	return false;
}

bool SceneBase::IsSelectUp(void)
{
	bool ret = false;
	
	if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_UP) ||
		 InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_LEFT))
	{
		ret = true;
	}

	return ret;
}
bool SceneBase::IsSelectDown(void)
{
	bool ret = false;
	
	if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_DOWN) ||
		InputManager::GetInstance().IsTrgDown(InputManager::TYPE::SELECT_RIGHT))
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