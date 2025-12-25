#include "InputManager.h" 
#include <DxLib.h>
#include <cassert>
#include <unordered_map>
#ifdef _DEBUG
#include <string>
#endif
#include "../Utility/AsoUtility.h"


InputManager* InputManager::instance_ = nullptr;

void InputManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}
	instance_->Init(); // マネージャ初期化処理
}


InputManager::InputManager(void)
{
	//cursorMode_ = CURSOR_MODE::NONE;
	//keys_.clear();
	//mouses_.clear();
}
/*
InputManager::~InputManager(void)
{
	//keys_.clear();
	//mouses_.clear();
}*/


void InputManager::Init(void)
{
	SetInputKey(); // 入力キー登録

	SetInputMouse(); // マウス入力登録
}

void InputManager::SetInputKey(void)
{
	AddKey(KEY_INPUT_RETURN);
	AddKey(KEY_INPUT_SPACE);
	AddKey(KEY_INPUT_X);

	// プレイヤー１
	AddKey(KEY_INPUT_W);
	AddKey(KEY_INPUT_A);
	AddKey(KEY_INPUT_S);
	AddKey(KEY_INPUT_D);

	// 
	AddKey(KEY_INPUT_UP);
	AddKey(KEY_INPUT_DOWN);
	AddKey(KEY_INPUT_LEFT);
	AddKey(KEY_INPUT_RIGHT);

	AddKey(KEY_INPUT_ESCAPE);
	AddKey(KEY_INPUT_LSHIFT);
	AddKey(KEY_INPUT_RSHIFT);
	AddKey(KEY_INPUT_LCONTROL);
	AddKey(KEY_INPUT_RCONTROL);
	AddKey(KEY_INPUT_TAB);
	AddKey(KEY_INPUT_DELETE);
}

void InputManager::SetInputMouse(void)
{
	// 左クリック
	AddMouse(MOUSE_INPUT_LEFT);

	// 右クリック
	AddMouse(MOUSE_INPUT_RIGHT);

	// 中央クリック
	AddMouse(MOUSE_INPUT_MIDDLE);
}


void InputManager::Update(void)
{
	// キーボード入力判定
	for (auto& [type, key] : keys_)
	{
		key->inputOld = key->inputNew;
		key->inputNew = CheckHitKey(key->type);
		key->trgDown = ( key->inputNew && !key->inputOld);
		key->trgUp   = (!key->inputNew && key->inputOld);
	}

	//マウス座標取得
	GetMousePoint(&mousePos_.x, &mousePos_.y);


	// マウス入力判定
	for (auto&[type, mouse] : mouse_)
	{
		mouse->inputOld = mouse->inputNew;
		mouse->inputNew = ((GetMouseInput() & mouse->type) != 0);
		mouse->trgDown = ( mouse->inputNew && !mouse->inputOld);
		mouse->trgUp   = (!mouse->inputNew && mouse->inputOld);
	}


	// ゲームパッド入力判定更新
	SetPadInState(PAD_NO::PAD1);
	SetPadInState(PAD_NO::PAD2);
	SetPadInState(PAD_NO::PAD3);
	SetPadInState(PAD_NO::PAD4);
}

void InputManager::Destroy(void)
{
	if (instance_ == nullptr) { return; }

	// 入力キー配列 解放
	for (auto& [type, key] : keys_) { delete key; }
	keys_.clear();

	// マウス入力配列 解放
	for (auto& [type, mouse] : mouse_) { delete mouse; }
	mouse_.clear();
	
	// インスタンスポインタを先にクリアしておく
	//InputManager* toDelete = instance_;
	//instance_ = nullptr;
	delete instance_;

}

bool InputManager::IsNew(TYPE _type, Input::PAD_NO _padNum)
{
	for (auto& isNew : funcMapIsNew_[_type])
	{
		if (isNew(_type, _padNum))
		{
			return true;
		}
	}
	return false;
}
bool InputManager::IsTrgDown(TYPE _type, Input::PAD_NO _padNum)
{
	for (auto& isTrgDown : funcMapIsTrgDown_[_type])
	{
		if (isTrgDown(_type, _padNum))
		{
			return true;
		}
	}
	return false;
}
bool InputManager::IsTrgUp(TYPE _type, Input::PAD_NO _padNum)
{
	for (auto& isTrgUp : funcMapIsTrgUp_[_type])
	{
		if (isTrgUp(_type, _padNum))
		{
			return true;
		}
	}
	return false;
}


#pragma region キーボード処理

void InputManager::AddKey(unsigned int type)
{
	/* 判定を行うキーを追加 */

	Input::Trigger* keyInfo = new Input::Trigger;

	keyInfo->type = type;
	keyInfo->inputOld = false;
	keyInfo->inputNew = false;
	keyInfo->trgUp   = false;
	keyInfo->trgDown = false;

	// 入力キー配列に格納
	keys_.emplace(type, keyInfo);
}

bool InputManager::KeyIsNew(unsigned int _keyType) const
{
	return false;
}

bool InputManager::KeyIsNewAll(void) const
{
	/* 登録したキーが入力しているか判定 */

	bool isInput = false;

	for (auto& [type, key] : keys_)
	{
		if (KeyIsNew(key->type))
		{
			// 入力が確認されたらtrueで終了
			isInput = true;
			break;
		}
	}

	return isInput;
}

Input::Trigger& InputManager::FindKey(unsigned int _keyType)const
{
	auto key = keys_.find(_keyType);
	if (key != keys_.end())
	{
		// キーの情報を返す
		return *key->second;
	}

#ifdef _DEBUG
	std::string text = "\nキーの情報がありません。[キーの値：";
	text += std::to_string(_keyType).c_str();
	text += "]";
	OutputDebugString(text.c_str());

#endif

	// 空のキー情報を返す
	return *triggerEmpty_;
}

#pragma endregion

#pragma region マウス処理

void InputManager::AddMouse(unsigned int type)
{
	MouseButton mouse = InputManager::MouseButton();

	mouse.type = type;
	mouse.inputOld = false;
	mouse.inputNew = false;
	mouse.trgDown = false;
	mouse.trgUp   = false;
	// 配列に格納
	mouseButton_.emplace(mouse.type, mouse);
}

const InputManager::MouseButton& InputManager::FindMouse(unsigned int _mouseType) const
{
	auto it = mouseButton_.find(_mouseType);
	if (it != mouseButton_.end())
	{
		// マウスの情報を返す
		return it->second;
	}
#ifdef _DEBUG

	std::string text = "\nマウスの情報がありません。[マウスの値：";
	text += std::to_string(_mouseType).c_str();
	text += "]";
	OutputDebugString(text.c_str());
#endif

	// 空のキー情報を返す
	return mouseInfoEmpty_;
}

#pragma endregion 


#pragma region コントローラ処理

void InputManager::SetPadInState(PAD_NO jPadNum)
{
	/* コントローラを識別して取得 */

	int num = static_cast<int>(jPadNum);
	auto stateNew = GetPadInputState(jPadNum);
	auto& stateNow = padInfos_[num];

	int max = static_cast<int>(PAD_BTN::MAX);
	for (int i = 0; i < max; i++)
	{
		stateNow.buttonOld[i] = stateNow.buttonNew[i];
		stateNow.buttonNew[i] = stateNew.buttonNew[i];

		stateNow.isOld[i] = stateNow.isNew[i];
		stateNow.isNew[i] = (stateNow.buttonNew[i] > 0);

		stateNow.isTrgDown[i] = (stateNow.isNew[i] && !stateNow.isOld[i]);
		stateNow.isTrgUp[i] = (!stateNow.isNew[i] && stateNow.isOld[i]);
	}

	max = static_cast<int>(JOYPAD_ALGKEY::MAX);
	for (int i = 0; i < max; i++)
	{
		stateNow.algKeyX[i] = stateNew.algKeyX[i];
		stateNow.algKeyY[i] = stateNew.algKeyY[i];

		stateNow.isOldAlgKey[i] = stateNow.isNewAlgKey[i];
		stateNow.isNewAlgKey[i] = (stateNow.algKeyX[i] != 0 || stateNow.algKeyY[i] != 0);

		stateNow.isTrgDownAlgKey[i] = (stateNow.isNewAlgKey[i] && !stateNow.isOldAlgKey[i]);
		stateNow.isTrgUpAlgKey[i] = (!stateNow.isNewAlgKey[i] && stateNow.isOldAlgKey[i]);
	}
}

InputManager::Joypad& InputManager::GetPadInputState(PAD_NO _padNum)
{
	/* コントローラ入力取得処理 */

	Joypad ret = InputManager::Joypad();

	auto type = GetPadType(_padNum);

	switch (type)
	{
		/* XBOX入力 */
	case InputManager::JOYPAD_TYPE::XBOX_ONE:
	{
		auto d = GetPadDInputState(_padNum);
		auto x = GetPadXInputState(_padNum);

		int index;

		//  Y
		// X  B
		//  A

		// A
		index = static_cast<int>(PAD_BTN::DOWN);
		ret.buttonNew[index] = d.Buttons[0];

		// B
		index = static_cast<int>(PAD_BTN::RIGHT);
		ret.buttonNew[index] = d.Buttons[1];

		// X
		index = static_cast<int>(PAD_BTN::LEFT);
		ret.buttonNew[index] = d.Buttons[2];

		// Y
		index = static_cast<int>(PAD_BTN::UP);
		ret.buttonNew[index] = d.Buttons[3];

		// L
		index = static_cast<int>(PAD_BTN::L_BUTTON);
		ret.buttonNew[index] = d.Buttons[4];

		// R
		index = static_cast<int>(PAD_BTN::R_BUTTON);
		ret.buttonNew[index] = d.Buttons[5];

		// BACK
		index = static_cast<int>(PAD_BTN::BACK);
		ret.buttonNew[index] = d.Buttons[6];

		// START
		index = static_cast<int>(PAD_BTN::START);
		ret.buttonNew[index] = d.Buttons[7];

		index = static_cast<int>(PAD_BTN::L_STICK);
		ret.buttonNew[index] = d.Buttons[8];

		index = static_cast<int>(PAD_BTN::R_STICK);
		ret.buttonNew[index] = d.Buttons[9];

		// RTrigger
		index = static_cast<int>(PAD_BTN::L_TRIGGER);
		ret.buttonNew[index] = x.LeftTrigger;

		// RTrigger
		index = static_cast<int>(PAD_BTN::R_TRIGGER);
		ret.buttonNew[index] = x.RightTrigger;



		// 左スティック
		index = static_cast<int>(JOYPAD_ALGKEY::LEFT);
		ret.algKeyX[index] = d.X;
		ret.algKeyY[index] = d.Y;

		// 右スティック
		index = static_cast<int>(JOYPAD_ALGKEY::RIGHT);
		ret.algKeyX[index] = d.Rx;
		ret.algKeyY[index] = d.Ry;

		// Dパッド(十字スティック)
		index = static_cast<int>(JOYPAD_ALGKEY::D_PAD);
		int inputX = 0;
		int inputY = 0;
		unsigned int left = 27000;
		unsigned int right = 9000;
		unsigned int up = 0;
		unsigned int down = 18000;

		if (*d.POV == left)  inputX = -1;
		if (*d.POV == right) inputX = 1;
		if (*d.POV == up)    inputY = -1;
		if (*d.POV == down)  inputY = 1;

		ret.algKeyX[index] = inputX;
		ret.algKeyY[index] = inputY;
	}
	break;

	/* PlayStationコントローラ入力 */
	case InputManager::JOYPAD_TYPE::DUAL_SENSE:
	{
		/*
		auto d = GetPadDInputState(_padNum);

		int index;

		// 　△
		// □　○
		// 　×

		//// △
		index = static_cast<int>(JOYPAD_BTN::TOP);
		ret.ButtonNew[index] = d.Buttons[3];

		//// ○
		index = static_cast<int>(JOYPAD_BTN::RIGHT);
		ret.ButtonNew[index] = d.Buttons[2];

		//// ×
		index = static_cast<int>(JOYPAD_BTN::DOWN);
		ret.ButtonNew[index] = d.Buttons[1];

		//// □
		index = static_cast<int>(JOYPAD_BTN::LEFT);
		ret.ButtonNew[index] = d.Buttons[0];


		// 左スティック
		index = static_cast<int>(JOYPAD_ALGKEY::LEFT);
		ret.AlgKeyX[index] = d.X;
		ret.AlgKeyY[index] = d.Y;

		// 右スティック
		index = static_cast<int>(JOYPAD_ALGKEY::RIGHT);
		ret.AlgKeyX[index] = d.Rx;
		ret.AlgKeyY[index] = d.Ry;
		*/
	}
	break;


	/* その他入力 */
	case InputManager::JOYPAD_TYPE::XBOX_360:
	case InputManager::JOYPAD_TYPE::DUAL_SHOCK_4:
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_L:
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_R:
	case InputManager::JOYPAD_TYPE::SWITCH_PRO_CTRL:
	case InputManager::JOYPAD_TYPE::OTHER:
	case InputManager::JOYPAD_TYPE::MAX:
	{
		// 入力なし
	}
	break;
	}

	return ret;
}

InputManager::JOYPAD_TYPE InputManager::GetJPadType(PAD_NO jPadNo)
{
	int num = GetJoypadType(static_cast<int>(jPadNo));
	InputManager::JOYPAD_TYPE type = static_cast<InputManager::JOYPAD_TYPE>(num);
	return type;
}


DINPUT_JOYSTATE InputManager::GetPadDInputState(InputManager::PAD_NO _padNum)
{
	// DirectInput情報取得
	GetJoypadDirectInputState(static_cast<int>(_padNum), &joyDInState_);
	return joyDInState_;
}

XINPUT_STATE InputManager::GetPadXInputState(InputManager::PAD_NO _padNum)
{
	// ボタン入力情報取得
	GetJoypadXInputState(static_cast<int>(_padNum), &joyXInState_);
	return joyXInState_;
}


InputManager::JOYPAD_TYPE InputManager::GetPadType(PAD_NO padNo)
{
	return static_cast<InputManager::JOYPAD_TYPE>(GetJoypadType(static_cast<int>(padNo)));
}

int InputManager::GetMouseWheelRot(void)
{
	return 0;
}



int InputManager::PadAlgKeyX(PAD_NO _padNum, JOYPAD_ALGKEY _algKey)const
{
	int num = static_cast<int>(_padNum);

	// パッドが未割当時、false
	if (GetJoypadNum() < num) return false;

	int key = static_cast<int>(_algKey);
	return padInfos_[num].algKeyX[key];
}
int InputManager::PadAlgKeyX(int _padNum, int _algKey)const
{
	// パッドが未割当時、false
	if (GetJoypadNum() < _padNum) return false;

	return padInfos_[_padNum].algKeyX[_algKey];
}

int InputManager::PadAlgKeyY(PAD_NO _padNum, JOYPAD_ALGKEY _algKey)const
{
	int num = static_cast<int>(_padNum);

	// パッドが未割当時、false
	if (GetJoypadNum() < num) return false;

	int key = static_cast<int>(_algKey);
	return padInfos_[num].algKeyY[key];
}
int InputManager::PadAlgKeyY(int _padNum, int _algKey)const
{
	// パッドが未割当時、false
	if (GetJoypadNum() < _padNum) return false;

	return padInfos_[_padNum].algKeyY[_algKey];
}

const VECTOR& InputManager::GetAlgKeyDirXZ(PAD_NO _padNum, JOYPAD_ALGKEY _algKey) const
{
	VECTOR ret = {};

	int num = static_cast<int>(_padNum);
	int algType = static_cast<int>(_algKey);

	// スティックの個々の入力値は、
	// -1000.0f ～ 1000.0f の範囲で返ってくるが、
	// X:1000.0f、Y:1000.0fになることは無い(1000と500くらいが最大)
	// スティックの入力値を -1.0 ～ 1.0 に正規化
	float dirX = (static_cast<float>(padInfos_[num].algKeyX[algType]) / ALGKEY_VAL_MAX);
	float dirZ = (static_cast<float>(padInfos_[num].algKeyY[algType]) / ALGKEY_VAL_MAX);

	// 平方根により、おおよその最大値が1.0となる
	float len = sqrtf((dirX * dirX) + (dirZ * dirZ));
	if (len < ALGKEY_THRESHOLD)
	{
		// 0でベクトルを返す
		return ret;
	}

	// デッドゾーン境界から再スケーリング(可変デッドゾーン)
	float scale = (len - ALGKEY_THRESHOLD) / (1.0f - ALGKEY_THRESHOLD);
	dirX = (dirX / len) * scale;
	dirZ = (dirZ / len) * scale;

	// Zは前方向を正に反転
	ret = VNorm({ dirX, 0.0f, -dirZ });

	return ret;
}

#pragma endregion