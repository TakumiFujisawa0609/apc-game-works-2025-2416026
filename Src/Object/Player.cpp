#include "Player.h"
#include <DxLib.h>
#include <cassert>
#include <string>

#include "./Object.h"
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "./Status/StatusData.h"
#include "./Status/StatusPlayer.h"
#include "../Utility/AsoUtility.h"
#include "./AnimationController.h"

using PAD_BTN = InputManager::PAD_BTN;
using PAD_ALGKEY = InputManager::JOYPAD_ALGKEY;
using ANIM_STATE = Player::ANIM_STATE;

Player::Player(void):
	status_(StatusData::GetInstance().GetPlayerStatus())
{
	paramChara_ = {};

	jumpPower_ = 0.0f;

	type_ = PLAYER_TYPE::NONE;

	inputKey_.clear();

	inputPad_ = -1;
	inputType_ = INPUT_TYPE::NONE;

	paramPlayer_.isRun = false;

}

void Player::Load(void)
{

}

void Player::Init(const VECTOR& pos, float angleY)
{
	/*　初期化処理　*/
	inputPad_ = static_cast<int>(InputManager::JOYPAD_NO::PAD1);
	inputKey_.emplace(INPUT_TYPE::MOVE_BACK, KEY_INPUT_W);
	inputKey_.emplace(INPUT_TYPE::MOVE_FRONT, KEY_INPUT_S);
	inputKey_.emplace(INPUT_TYPE::MOVE_LEFT, KEY_INPUT_A);
	inputKey_.emplace(INPUT_TYPE::MOVE_RIGHT, KEY_INPUT_D);
	inputKey_.emplace(INPUT_TYPE::RUN, KEY_INPUT_LSHIFT);
	//inputKey_.emplace(INPUT_TYPE::ATTACK_JUB, MOUSE_INPUT_1);
	//inputKey_.emplace(INPUT_TYPE::ATTACK_STRONG, MOUSE_INPUT_2);

	SetParam();

	float rotY = AsoUtility::Deg2Rad(angleY);
	paramChara_.quaRot = Quaternion::Euler({ 0.0f, angleY, 0.0f });

	paramChara_.pos = paramChara_.prePos = pos;

	paramChara_.posChatch = AsoUtility::VECTOR_ZERO;
	paramChara_.posChatch.y += CATCH_OFFSET;


	// モデルスケール調整
	VECTOR size = { MODEL_SIZE, MODEL_SIZE, MODEL_SIZE };
	MV1SetScale(paramChara_.handle, size);

	// モデル位置調整
	VECTOR modelPos = VAdd(paramChara_.pos, MODEL_OFFSET);
	MV1SetPosition(paramChara_.handle, modelPos);

	jumpPower_ = START_JUMP_POWER;

	// 当たり判定割り当て
	//collision_ = new PlayerCollision;
	//collision_->Init(*this);
}

void Player::SetParam(void)
{
	/*　パラメータ割り当て　*/
	
	// 行動状態
	paramPlayer_.actionState = ACTION_STATE::IDLE;

	paramChara_.velocity = AsoUtility::VECTOR_ZERO;
	paramChara_.dir = {};

	paramChara_.modelOffset = MODEL_OFFSET;

	// 角度初期化
	paramChara_.quaRot = Quaternion::Identity();
	paramChara_.quaRotLocal = Quaternion::Identity(); // ローカル回転初期化
	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
											   Quaternion::AngleAxis(LOCAL_ANGLE_Y, AsoUtility::AXIS_Y));

	VECTOR rotLocal = VAdd(AsoUtility::AXIS_X, AsoUtility::AXIS_Z);

	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
											   Quaternion::AngleAxis(0.0f, rotLocal));

	// 着地判定
	paramChara_.isGround = false;

	paramChara_.scale = { MODEL_SIZE, MODEL_SIZE, MODEL_SIZE };

	paramChara_.hp    = status_.GetHp();
	paramChara_.power = status_.GetPower();
	paramChara_.speed = status_.GetSpeed();
	paramChara_.speedAcc = status_.GetSpeedAcc();

	paramPlayer_.weaponId = status_.GetWeaponId();
	paramPlayer_.luck = status_.GetLuck();
	paramChara_.handle = ResourceManager::GetInstance().LoadHandleId(ResourceManager::SRC::MODEL_PLAYER);

	paramPlayer_.animSpeed.clear();
	paramPlayer_.animSpeed.emplace(ANIM_STATE::IDLE, status_.GetAnimSpeedIdle());
	paramPlayer_.animSpeed.emplace(ANIM_STATE::WALK, status_.GetAnimSpeedWalk());
	paramPlayer_.animSpeed.emplace(ANIM_STATE::RUN, status_.GetAnimSpeedRun());
	
	SetAnim();
	SetMatrixModel();
}

void Player::SetAnim(void)
{
	ANIM_STATE type;
	int max = static_cast<int>(ANIM_STATE::MAX);
	float speed = 0.0f;

	// アニメーション初期化
	anim_ = new AnimationController(paramChara_.handle);


	for (auto& anim : paramPlayer_.animSpeed)
	{
		type = anim.first;

		speed = paramPlayer_.animSpeed[type];

		// アニメーション割り当て
		anim_->AddInFbx(static_cast<int>(type), speed, static_cast<int>(type));
	}

	// 待機アニメーション再生
	anim_->Play(static_cast<int>(ANIM_STATE::IDLE));
}



void Player::Update(void)
{
	/*　更新処理　*/

	// 反転回転フラグ
	bool isRevert = false;

	// 現在位置割り当て
	paramChara_.prePos = paramChara_.pos;

	switch (paramPlayer_.actionState)
	{
	case ACTION_STATE::IDLE:
	{
		Update_Idle();
	}
	break;

	case ACTION_STATE::ATTACK_WAIT:
	case ACTION_STATE::ATTACK:
	case ACTION_STATE::ATTACK_END:
	{
		// 攻撃処理
		Update_Attack();
	}
	break;

	case ACTION_STATE::GAME_OVER:
	{
		// ゲームオーバー処理
		Update_GameOver();
	}
	break;
	}


#ifdef _DEBUG
	/*　重量変化　*/
	/*
	if (input.KeyIsTrgDown(KEY_INPUT_UP))
	{
		paramChara_.weight += 0.1f;
		if (paramChara_.weight > 1.0f) paramChara_.weight = 1.0f;
	}
	if (input.KeyIsTrgDown(KEY_INPUT_DOWN))
	{
		paramChara_.weight -= 0.1f;
		if (paramChara_.weight < 0.0f) paramChara_.weight = 0.0f;
	}*/
#endif

	// 移動処理
	Move();

	// 回転処理
	Rotation(isRevert);

	// モデル位置調整
	VECTOR pos = VAdd(paramChara_.pos, MODEL_OFFSET);
	VECTOR size = { MODEL_SIZE, MODEL_SIZE, MODEL_SIZE };
	MV1SetScale(paramChara_.handle, size);

	// アニメーション更新
	Update_Animation();


	// 行列更新
	SetMatrixModel();

	// 当たり判定更新
	//collision_->Update();
}

void Player::Draw(void)
{
	MV1DrawModel(paramChara_.handle);
}

void Player::DrawDebug(void)
{
#ifdef _DEBUG
	
	VECTOR pos = VAdd(paramChara_.pos, MODEL_OFFSET);
	// パラメータ描画
	DrawFormatString(0, 120, 0xFFFFFF,"player：p(%.1f, %.1f, %.1f), rot(%.1f°, %.1f° ,%.1f°), ac(%.1f°,%.1f°,%.1f°),ground(%d), type(%d)"
					 ,pos.x, pos.y, pos.z
					 ,paramChara_.rot.x, paramChara_.rot.y, paramChara_.rot.z
					 ,paramChara_.velocity.x, paramChara_.velocity.y, paramChara_.velocity.z
					 ,paramChara_.isGround, paramPlayer_.actionState);

	// 向き描画
	AsoUtility::DrawLineXYZ(paramChara_.pos, paramChara_.quaRot);
#endif
}

void Player::Release(void)
{
	/*　解放処理　*/

	paramPlayer_.animSpeed.clear();

	// アニメーション解放
	anim_->Release();
	delete anim_;

	// 当たり判定解放	
	//collision_->Release();
	//delete collision_;
}

void Player::Update_Idle(void)
{
	auto& input = InputManager::GetInstance();

	// ジャンプ
	//Jump();

	// 攻撃処理起動
	ANIM_STATE anim = static_cast<ANIM_STATE>(anim_->GetPlayType());

	// 攻撃入力・攻撃アニメーションではないとき、攻撃有効化
	if (IsInputAtkStrong() && anim != ANIM_STATE::SWORD_SLASH)
	{
		SetActionState(ACTION_STATE::ATTACK_WAIT);

		paramChara_.timeAct = ATTACK_TIME_WAIT;
	}
}


void Player::Update_Attack(void)
{
	Action(ACTION_STATE::ATTACK_WAIT, ACTION_STATE::ATTACK, ATTACK_TIME,
		ACTION_STATE::ATTACK_END, ATTACK_TIME_END);

	// 重力
	//Gravity();
}


void Player::Update_GameOver(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	if (paramChara_.timeInv > 0.0f)
	{
		paramChara_.timeInv -= delta;
	}

	// Y軸加算
	paramChara_.pos.y += paramChara_.velocity.y;
}


bool Player::IsInputMove(void)
{
	InputManager& input = InputManager::GetInstance();

	bool ret = false;

	// コントローラ入力時
	if (GetJoypadNum() > 0)
	{
		InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(inputPad_);

		// 左スティックが入力されている時
		if (input.PadIsAlgKeyNew(inputPad_, PAD_ALGKEY::LEFT))
		{
			ret = true;
		}
	}

	// キーボード入力時
	else if (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_FRONT]) ||
		input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_BACK]) ||
		input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_LEFT]) ||
		input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_RIGHT]))
	{
		// 移動入力時、true
		ret = true;
	}

	return ret;
}

bool Player::IsInputAtkStrong(void)
{
	InputManager& input = InputManager::GetInstance();

	bool ret = false;

	// コントローラ入力時
	if (inputPad_ != -1)
	{
		if (input.PadIsBtnTrgDown(inputPad_, PAD_BTN::UP))
		{
			ret = true;
		}
	}

	// キーボード入力時
	else if (input.KeyIsTrgDown(inputKey_[INPUT_TYPE::ATTACK_STRONG]))
	{
		// 移動入力時、true
		ret = true;
	}

	return ret;
}

bool Player::IsInputAtkJub(void)
{
	InputManager& input = InputManager::GetInstance();

	bool ret = false;

	// コントローラ入力時
	if (GetJoypadNum() > 0)
	{
		if (input.PadIsBtnTrgDown(inputPad_, PAD_BTN::LEFT))
		{
			ret = true;
		}
	}

	// キーボード入力時
	else if (input.KeyIsTrgDown(inputKey_[INPUT_TYPE::ATTACK_JUB]))
	{
		// 移動入力時、true
		ret = true;
	}

	return ret;
}

bool Player::IsInputRun(void)
{
	InputManager& input = InputManager::GetInstance();

	// コントローラ入力時
	if (GetJoypadNum() > 0)
	{
		if (input.PadIsBtnTrgDown(inputPad_, PAD_BTN::L_STICK))
		{
			paramPlayer_.isRun = true;
		}
		else if (input.PadIsBtnTrgUp(inputPad_, PAD_BTN::L_STICK))
		{
			paramPlayer_.isRun = false;
		}
	}

	// キーボード入力時
	else
	{
		if (input.KeyIsTrgDown(inputKey_[INPUT_TYPE::RUN]))
		{
			// ダッシュ移動入力時、true
			paramPlayer_.isRun = true;
		}
		else if (input.KeyIsTrgUp(inputKey_[INPUT_TYPE::RUN]))
		{
			// ダッシュ移動入力時、true
			paramPlayer_.isRun = false;
		}
	}

	return paramPlayer_.isRun;
}


void Player::SetIsAttack(bool flag)
{
	if (flag == false)
	{
		// 攻撃無効化時、攻撃時間初期化
		paramChara_.timeAct = 0.0f;
	}
}


bool Player::CheckActiveAttack(void) const
{
	bool ret = false;

	// 攻撃判定が有効中か否か
	if (paramPlayer_.actionState == ACTION_STATE::ATTACK &&
		paramChara_.timeAct > 0.0f)
	{
		// 攻撃有効時、true
		ret = true;
	}

	return ret;
}

void Player::SetActionState(ACTION_STATE state)
{
}


void Player::Move(void)
{
	/*　移動処理　*/

	InputManager& input = InputManager::GetInstance();

	if (GetJoypadNum() > 0)
	{
		InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(inputPad_);


		if (input.PadAlgKeyY(inputPad_, PAD_ALGKEY::LEFT) && IsActiveAction())
		{
			if (input.PadAlgKeyY(inputPad_, PAD_ALGKEY::LEFT) < 0)
			{
				// 奥移動処理
				paramChara_.velocity.z += _Move(&paramChara_.velocity.z, paramChara_.speedAcc, paramChara_.speed);
			}
			else
			{
				// 前移動処理
				paramChara_.velocity.z += _Move(&paramChara_.velocity.z, -paramChara_.speedAcc , -paramChara_.speed);
			}
		}
		else
		{
			// 加速度減少
			paramChara_.velocity.z = DecVelocityXZ(&paramChara_.velocity.z);
		}

		if (input.PadAlgKeyX(inputPad_, PAD_ALGKEY::LEFT) != 0 && IsActiveAction())
		{
			if (input.PadAlgKeyX(inputPad_, PAD_ALGKEY::LEFT) > 0)
			{
				// 右移動処理
				paramChara_.velocity.x += _Move(&paramChara_.velocity.x, paramChara_.speedAcc, paramChara_.speed);
			}
			else
			{
				// 左移動処理
				paramChara_.velocity.x += _Move(&paramChara_.velocity.x, -paramChara_.speedAcc , -paramChara_.speed);
			}
		}
		else
		{
			paramChara_.velocity.x = DecVelocityXZ(&paramChara_.velocity.x);
		}
	}

	// キーボード操作
	else
	{
		bool inputY = (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_BACK]) &&
			!input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_FRONT]) ||
			!input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_BACK]) &&
			input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_FRONT]));

		bool inputX = (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_LEFT]) &&
			!input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_RIGHT]) ||
			!input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_LEFT]) &&
			input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_RIGHT]));

		if (inputY && IsActiveAction())
		{
			if (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_BACK]))
			{
				// 奥移動処理
				paramChara_.velocity.z += _Move(&paramChara_.velocity.z, paramChara_.speedAcc, paramChara_.speed);
			}

			if (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_FRONT]))
			{
				// 前移動処理
				paramChara_.velocity.z += _Move(&paramChara_.velocity.z, -paramChara_.speedAcc, -paramChara_.speed);
			}
		}
		else
		{
			paramChara_.velocity.z = DecVelocityXZ(&paramChara_.velocity.z);
		}

		if (inputX && IsActiveAction())
		{
			if (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_LEFT]))
			{
				// 左移動処理
				paramChara_.velocity.x += _Move(&paramChara_.velocity.x, -paramChara_.speedAcc, -paramChara_.speed);
			}

			if (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_RIGHT]))
			{
				// 右移動処理
				paramChara_.velocity.x += _Move(&paramChara_.velocity.x, paramChara_.speedAcc, paramChara_.speed);
			}
		}
		else
		{
			paramChara_.velocity.x = DecVelocityXZ(&paramChara_.velocity.x);
		}
	}




	if (paramChara_.velocity.x != 0.0f && paramChara_.velocity.z != 0.0f)
	{
		float len = sqrtf((paramChara_.velocity.x * paramChara_.velocity.x) + (paramChara_.velocity.z * paramChara_.velocity.z));

		// 移動ベクトルを正規化
		float length = sqrtf(len);

		float speed = (paramChara_.speed + paramChara_.speedAcc);
	
		if (length > speed)
		{
			// 長さが移動速度を超えた時、スカラー倍

			float scale = (speed / length);

			paramChara_.velocity.x *= scale;
			paramChara_.velocity.z *= scale;
		}
	}

	// 位置に反映する
	// 移動処理
	paramChara_.pos = VAdd(paramChara_.pos, paramChara_.velocity);

}


void Player::Update_Animation(void)
{
	// アニメーション遷移処理
	AnimationState();


	if (paramChara_.timeInv > 0.0f)
	{
		anim_->Play(static_cast<int>(ANIM_STATE::JUMP), false);
		anim_->SetAnimStep(anim_->GetPlayTimeTotal());
		anim_->Stop();
	}

	// アニメーション更新
	anim_->Update();
}

void Player::AnimationState(void)
{
	ANIM_STATE state = static_cast<ANIM_STATE>(anim_->GetPlayType());
	InputManager& input = InputManager::GetInstance();

	// 待機アニメーション状態
	if (state == ANIM_STATE::IDLE)
	{
		// ダッシュ
		if (IsInputRun() && IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::RUN)); }

		// 歩行
		else if (IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::WALK)); }

		// 攻撃
		if (IsInputAtkStrong())
		{
			paramPlayer_.isRun = false;
			anim_->Play(static_cast<int>(ANIM_STATE::SWORD_SLASH), false);
		}
	}

	else if (state == ANIM_STATE::SWORD_SLASH)
	{
		// 攻撃終了時、アニメーション終了している時
		if (anim_->IsEnd() &&
			paramPlayer_.actionState != ACTION_STATE::ATTACK_END &&
			paramPlayer_.actionState != ACTION_STATE::ATTACK &&
			paramChara_.timeAct <= 0.0f)
		{
			if (IsInputRun() && IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::RUN)); }

			// 歩行
			else if (IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::WALK)); }

			// 待機
			else { anim_->Play(static_cast<int>(ANIM_STATE::IDLE)); }
		}
	}

	else if (state == ANIM_STATE::WALK)
	{
		// ダッシュ
		if (IsInputRun() && IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::RUN)); }

		// 待機
		else if (!IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::IDLE)); }

		// 攻撃
		if (IsInputAtkStrong())
		{
			paramPlayer_.isRun = false;
			anim_->Play(static_cast<int>(ANIM_STATE::SWORD_SLASH), false);
		}
	}
	else if (state == ANIM_STATE::RUN)
	{
		// 歩行
		if (IsInputMove() && !IsInputRun()) { anim_->Play(static_cast<int>(ANIM_STATE::WALK)); }

		// 待機
		else if (!IsInputMove()){ anim_->Play(static_cast<int>(ANIM_STATE::IDLE)); }

		// 攻撃
		if (IsInputAtkStrong())
		{
			paramPlayer_.isRun = false;
			anim_->Play(static_cast<int>(ANIM_STATE::SWORD_SLASH), false);
		}
	}
}


void Player::Action(ACTION_STATE waitState, ACTION_STATE activeState, float activeTime,
	ACTION_STATE endState, float endTime)
{
	auto& input = InputManager::GetInstance();
	float delta = SceneManager::GetInstance().GetDeltaTime();


	// 攻撃しているときに攻撃時間減少
	paramChara_.timeAct -= delta;

	if (paramPlayer_.actionState == waitState)
	{
		if (paramChara_.timeAct <= 0.0f)
		{
			// 行動の有効な時間割り当て
			paramChara_.timeAct = activeTime;

			// 有効化
			SetActionState(activeState);
		}
	}

	// 行動時の処理
	if (paramPlayer_.actionState == activeState)
	{
		if (paramChara_.timeAct <= 0.0f)
		{
			// 行動が失敗した時の処理
			paramChara_.timeAct = endTime;

			// 行動終了処理
			SetActionState(endState);
		}
	}

	// 終了時の処理
	if (paramPlayer_.actionState == endState)
	{
		int temp = 0;
		if (paramChara_.timeAct <= 0.0f)
		{
			paramChara_.timeAct = 0.0f;

			// 待機状態に戻す
			SetActionState(ACTION_STATE::IDLE);
		}
	}
}

bool Player::IsActiveAction(void)const
{
	bool ret = true;

	if (paramPlayer_.actionState == ACTION_STATE::ATTACK ||
		paramPlayer_.actionState == ACTION_STATE::ATTACK_END ||
		paramPlayer_.actionState == ACTION_STATE::GAME_OVER)
	{
		return false;
	}

	return ret;
}
