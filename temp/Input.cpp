#include "Input.h"

Input::Input(void)
{
}

bool Input::PadIsBtnNew(PAD_NO _padNum, PAD_BTN button) const
{
	int num = static_cast<int>(_padNum);

	// パッドが未割当時、false
	if (GetJoypadNum() < num) return false;

	int btnType = static_cast<int>(button);
	bool ret = (padInfos_[num].isNew[btnType]);
	return ret;
}
bool Input::PadIsBtnNew(int _padNum, int button) const
{
	// パッドが未割当時、false
	if (GetJoypadNum() < _padNum) return false;

	bool ret = (padInfos_[_padNum].isNew[button]);
	return ret;
}

bool Input::PadIsBtnTrgDown(PAD_NO _padNum, PAD_BTN button) const
{
	int num = static_cast<int>(_padNum);

	// パッドが未割当時、false
	if (GetJoypadNum() < num) return false;

	int btnType = static_cast<int>(button);
	bool ret = (padInfos_[num].isTrgDown[btnType]);
	return ret;
}
bool Input::PadIsBtnTrgDown(int _padNum, int button) const
{
	// パッドが未割当時、false
	if (GetJoypadNum() < _padNum) return false;

	bool ret = (padInfos_[_padNum].isTrgDown[button]);
	return ret;
}

bool Input::PadIsBtnTrgUp(PAD_NO _padNum, PAD_BTN button) const
{
	int num = static_cast<int>(_padNum);

	// パッドが未割当時、false
	if (GetJoypadNum() < num) return false;

	int btnType = static_cast<int>(button);
	bool ret = (padInfos_[num].isTrgUp[btnType]);
	return ret;
}
bool Input::PadIsBtnTrgUp(int _padNum, int button) const
{
	// パッドが未割当時、false
	if (GetJoypadNum() < _padNum) return false;

	bool ret = (padInfos_[_padNum].isTrgUp[button]);
	return ret;
}

bool Input::PadIsAlgKeyNewAll(PAD_NO _padNum) const
{
	/* いずれかのパッドの */
	bool ret = false;

	const int padNum = static_cast<int>(_padNum);
	const int max = static_cast<int>(JOYPAD_ALGKEY::MAX);

	for (int i = 0; i < max; i++)
	{
		if (padInfos_[padNum].isNewAlgKey[i])
		{
			// 対象のパッドのアナログキーが入力されているときtrue
			ret = true;
			break;
		}
	}
	
	return false;
}

bool Input::PadIsAlgKeyNew(PAD_NO _padNum, JOYPAD_ALGKEY _algKey)const
{
	int num = static_cast<int>(_padNum);

	// パッドが未割当時、false
	if (GetJoypadNum() < static_cast<int>(num)) return false;

	int _keyType = static_cast<int>(_algKey);
	bool ret = (padInfos_[num].isNewAlgKey[_keyType]);
	return ret;
}
bool Input::PadIsAlgKeyNew(int _padNum, int _algKey)const
{
	// パッドが未割当時、false
	if (GetJoypadNum() < _padNum) return false;

	bool ret = (padInfos_[_padNum].isNewAlgKey[_algKey]);
	return ret;
}

bool Input::PadIsAlgKeyTrgDown(PAD_NO _padNum, JOYPAD_ALGKEY _algKey)const
{
	int num = static_cast<int>(_padNum);
	// パッドが未割当時、false
	if (GetJoypadNum() < num) return false;

	int _keyType = static_cast<int>(_algKey);
	bool ret = (padInfos_[num].isTrgDownAlgKey[_keyType]);
	return ret;
}
bool Input::PadIsAlgKeyTrgDown(int _padNum, int _algKey)const
{
	// パッドが未割当時、false
	if (GetJoypadNum() < _padNum) return false;

	bool ret = (padInfos_[_padNum].isTrgDownAlgKey[_algKey]);
	return ret;
}

bool Input::PadIsAlgKeyTrgUp(PAD_NO _padNum, JOYPAD_ALGKEY _algKey)const
{
	int num = static_cast<int>(_padNum);
	// パッドが未割当時、false
	if (GetJoypadNum() < num) return false;

	int _keyType = static_cast<int>(_algKey);
	bool ret = (padInfos_[num].isTrgUpAlgKey[_keyType]);
	return ret;
}
bool Input::PadIsAlgKeyTrgUp(int _padNum, int _algKey)const
{
	// パッドが未割当時、false
	if (GetJoypadNum() < _padNum) return false;

	bool ret = (padInfos_[_padNum].isTrgUpAlgKey[_algKey]);
	return ret;
}