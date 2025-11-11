#include "Player.h"
#include <DxLib.h>
#include <cassert>
#include <string>
#include <map>
#include <unordered_map>

#include "../Object.h"
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../Status/StatusData.h"
#include "../Status/StatusPlayer.h"
#include "../../Utility/AsoUtility.h"
#include "../../Common/Quaternion.h"
#include "../../Common/Camera.h"
#include "../../Common/Perform.h"
#include "../Common/AnimationController.h"


Player::Player(void):
	Object::Object(),
	status_(StatusData::GetInstance().GetPlayerStatus()),
	motionType_(-1)
{	
	inputKey_.clear();

	Load();
}

void Player::LoadPost(void)
{
	paramChara_.handle = ResourceManager::GetInstance().LoadHandleId(ResourceManager::SRC::MODEL_PLAYER);
}

void Player::SetParam(void)
{
	/*　パラメータ割り当て　*/
	
	// 行動状態
	paramPlayer_.actionState = ACTION_STATE::IDLE;

	paramChara_.posLocal = LOCAL_POS;

	// 角度初期化
	paramChara_.quaRot = Quaternion::Identity();
	paramChara_.quaRotLocal = Quaternion::Identity(); // ローカル回転初期化
	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
											   Quaternion::AngleAxis(LOCAL_ANGLE_Y, AsoUtility::AXIS_Y));

	VECTOR rotLocal = VAdd(AsoUtility::AXIS_X, AsoUtility::AXIS_Z);
	/*
	paramChara_.quaRotLocal = Quaternion::Mult(paramChara_.quaRotLocal,
											   Quaternion::AngleAxis(0.0f, rotLocal));
*/
	// 着地判定
	paramChara_.isGround = false;

	float scale = status_.GetScale();
	paramChara_.scale = { scale ,scale, scale };
	paramChara_.radius = status_.GetRadius();

	paramChara_.hp    = status_.GetHp();
	paramChara_.power = status_.GetPower();
	paramChara_.speed = status_.GetSpeed();
	paramChara_.speedAcc = status_.GetSpeedAcc();
	paramPlayer_.dashMult = status_.GetDashMult();
	paramPlayer_.jubCnt = 0;

	paramPlayer_.weaponId = status_.GetWeaponId();
	paramPlayer_.luck = status_.GetLuck();

	paramChara_.isActive = true;
	paramChara_.velocity = AsoUtility::VECTOR_ZERO;
	Rotation(true);
}
void Player::InitAnim(void)
{
	int max = static_cast<int>(ANIM_STATE::MAX);

	// 内部アニメーション読み込み
	paramPlayer_.animSpeed.emplace(ANIM_STATE::IDLE, status_.GetAnimSpeedIdle());
	paramPlayer_.animSpeed.emplace(ANIM_STATE::WALK, status_.GetAnimSpeedWalk());
	paramPlayer_.animSpeed.emplace(ANIM_STATE::DASH, status_.GetAnimSpeedDash());
	paramPlayer_.animSpeed.emplace(ANIM_STATE::SWORD_SLASH, status_.GetMotionSpeed(MOTION_TYPE::JUB_1));
	for (auto& [type, speed] : paramPlayer_.animSpeed)
	{
		// アニメーション番号
		int typeNum = static_cast<int>(type);

		speed = paramPlayer_.animSpeed[type];

		// 内部アニメーション以外ではスキップ
		if (typeNum >= static_cast<int>(ANIM_STATE::MAX)) { continue; }

		// 内部アニメーション割り当て
		anim_->AddInternal(static_cast<int>(type), speed);
	}

	// 外部アニメーション読み込み
	int res = -1;

	// 弱攻撃1
	res = ResourceManager::GetInstance().LoadHandleId(RES_SRC::ANIM_JUB_1_SWORD);
	SetExternalAnim(ANIM_STATE::JUB_1, MOTION_TYPE::JUB_1, res);

	// 弱攻撃2
	res = ResourceManager::GetInstance().LoadHandleId(RES_SRC::ANIM_JUB_2_SWORD);
	SetExternalAnim(ANIM_STATE::JUB_2, MOTION_TYPE::JUB_2, res);

	// 弱攻撃3
	res = ResourceManager::GetInstance().LoadHandleId(RES_SRC::ANIM_JUB_3_SWORD);
	SetExternalAnim(ANIM_STATE::JUB_END, MOTION_TYPE::JUB_END, res);

	// 必殺技
	res = ResourceManager::GetInstance().LoadHandleId(RES_SRC::ANIM_SPECIAL_SWORD);
	SetExternalAnim(ANIM_STATE::SPECIAL, MOTION_TYPE::SPECIAL , res);

	// 強攻撃1
	res = ResourceManager::GetInstance().LoadHandleId(RES_SRC::ANIM_STRONG_1_SWORD);
	SetExternalAnim(ANIM_STATE::STRONG_1, MOTION_TYPE::STRONG_1, res);

	// 強攻撃2
	res = ResourceManager::GetInstance().LoadHandleId(RES_SRC::ANIM_STRONG_2_SWORD);
	SetExternalAnim(ANIM_STATE::STRONG_2, MOTION_TYPE::STRONG_2, res);

	// 強攻撃3
	res = ResourceManager::GetInstance().LoadHandleId(RES_SRC::ANIM_STRONG_3_SWORD);
	SetExternalAnim(ANIM_STATE::STRONG_3, MOTION_TYPE::STRONG_3, res);


	// 待機アニメーション再生
	anim_->Play(static_cast<int>(ANIM_STATE::IDLE));
}
void Player::SetExternalAnim(ANIM_STATE _state, MOTION_TYPE _motion, int _res)
{
	/* 外部アニメーション割り当て処理 */
	// アニメーション速度格納
	float animSpeed = 0.0f;
	paramPlayer_.animSpeed.emplace(_state, status_.GetMotionSpeed(_motion));

	// アニメーションに格納
	animSpeed = paramPlayer_.animSpeed.at(_state);
	anim_->AddExternal(static_cast<int>(_state), animSpeed, _res);
}

void Player::InitModelFrame(void)
{
	Object::InitModelFrame();

	int index;
	index = FindFrameNum("Torso");
	paramChara_.colList.emplace(COLLISION_TYPE::BODY, &paramChara_.frames[index]);

	index = FindFrameNum("Head");
	paramChara_.colList.emplace(COLLISION_TYPE::HEAD, &paramChara_.frames[index]);

	index = FindFrameNum("Bone");
	paramChara_.colList.emplace(COLLISION_TYPE::BOTTOM, &paramChara_.frames[index]);

	index = FindFrameNum("Fist.L");
	paramChara_.colList.emplace(COLLISION_TYPE::HAND_L, &paramChara_.frames[index]);

	index = FindFrameNum("Fist.R");
	paramChara_.colList.emplace(COLLISION_TYPE::HAND_R, &paramChara_.frames[index]);
}
void Player::InitPost(void)
{
	inputKey_.emplace(INPUT_TYPE::MOVE_BACK, KEY_INPUT_W);
	inputKey_.emplace(INPUT_TYPE::MOVE_FRONT, KEY_INPUT_S);
	inputKey_.emplace(INPUT_TYPE::MOVE_LEFT, KEY_INPUT_A);
	inputKey_.emplace(INPUT_TYPE::MOVE_RIGHT, KEY_INPUT_D);
	inputKey_.emplace(INPUT_TYPE::DASH, KEY_INPUT_LSHIFT);
	inputKey_.emplace(INPUT_TYPE::ATTACK_JUB, MOUSE_INPUT_LEFT);
	inputKey_.emplace(INPUT_TYPE::ATTACK_STRONG, MOUSE_INPUT_RIGHT);

	//jumpPower_ = START_JUMP_POWER;

	// 当たり判定割り当て
	//collision_ = new PlayerCollision;
	//collision_->Init(*this);

	Update();
}


void Player::UpdateAnim(void)
{
	// アニメーション更新
	anim_->Update();
}
void Player::UpdatePost(void)
{
	/*　プレイヤー独自処理　*/
	// 反転回転フラグ
	bool isRevert = false;

	// 状態更新処理
	UpdateActionState();

	// 回転処理
	Rotation(isRevert);

	// 行列更新
	SetMatrixModel();

	// 当たり判定更新
	//collision_->Update();
}


void Player::DrawPost(void)
{
	unsigned int color = ((paramChara_.atkMotion.GetIsActionAttack()) ? 0xff0000 : 0xcccccc);
	if (SceneManager::GetInstance().GetIsDebugMode())
	{
		// 正面座標
		if (IsActionAttack())
		{
			float forwardRad = status_.GetMotionRadius(motionType_);
			DrawSphere3D(paramChara_.posForward, forwardRad,
				16, color, 0xffffff, false);
		}
	}
}

void Player::DrawDebug(void)
{
	DrawFormatString(Application::SCREEN_HALF_X-500, 0, 0xffffff, "プレイヤーのHP:%d", paramChara_.hp);

#ifdef _DEBUG
	/*
	Object::DrawDebug();

	VECTOR pos = VAdd(paramChara_.pos, paramChara_.posLocal);

	VECTOR rot = Quaternion::ToEuler(Quaternion::Mult(paramChara_.quaRot, paramChara_.quaRotLocal));
	rot = Quaternion::ToEuler(paramChara_.quaRotLocal);
	rot = AsoUtility::Rad2Deg(rot);

	// パラメータ描画
	DrawFormatString(0, 120, 0xFFFFFF,"player：p(%.1f, %.1f, %.1f), rot(%.1f°, %.1f° ,%.1f°), ac(%.1f°,%.1f°,%.1f°),attack(%d, %.1f秒), type(%d), anim(%d), cnt(%d)"
					 ,pos.x, pos.y, pos.z
					 ,rot.x, rot.y, rot.z
					 ,paramChara_.velocity.x, paramChara_.velocity.y, paramChara_.velocity.z
					 ,paramChara_.atkMotion.GetAttackState(), paramChara_.atkMotion.GetTimeAction()
		             ,paramPlayer_.actionState ,anim_->GetPlayType() ,paramPlayer_.jubCnt);*/
#endif
}

void Player::UpdateActionState(void)
{
	if (paramPlayer_.actionState == ACTION_STATE::IDLE)
	{
		UpdateStateIdle();
	}
	else if (IsActionAttack())
	{
		// 攻撃処理
		UpdateStateAtk();
	}
	else if (paramPlayer_.actionState == ACTION_STATE::GAME_OVER)
	{
		// ゲームオーバー処理
		UpdateStateOver();
	}
}

void Player::UpdateStateIdle(void)
{
	auto& input = InputManager::GetInstance();

	// ジャンプ
	//Jump();

	// 移動処理
	if (input.KeyIsNewAll() ||
		input.PadIsAlgKeyNew(InputManager::PAD_NO::PAD1, PAD_ALGKEY::LEFT))
	{
		Move();
	}

	// 攻撃処理起動
	ANIM_STATE anim = static_cast<ANIM_STATE>(anim_->GetPlayType());

	// 弱攻撃処理
	if (IsInputAtkJub())
	{
		ChangeActionState(ACTION_STATE::ATTACK_JUB);
	}

	// 強攻撃処理
	if (IsInputAtkStrong())
	{
		ChangeActionState(ACTION_STATE::ATTACK_STRONG);
	}

	// アニメーション遷移処理
	AnimStateIdle();
}


void Player::UpdateStateAtk(void)
{
	// 攻撃モーション更新
	UpdateMotion();

	// モーション終了時、通常状態に戻す
	if (anim_->IsEnd() &&
		paramChara_.atkMotion.GetAttackState() == AttackMotion::ATTACK_STATE::NONE)
	{
		ChangeActionState(ACTION_STATE::IDLE);
	}
}


void Player::UpdateStateOver(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	if (paramChara_.timeInv > 0.0f)
	{
		paramChara_.timeInv -= delta;
	}

	// Y軸加算
	paramChara_.pos.y += paramChara_.velocity.y;
}


void Player::ChangeActionState(ACTION_STATE _state)
{
	int stateNum = static_cast<int>(_state);

	// 再生の状態
	int animNum = -1;
	bool isLoop = true;

	float activeTime;
	float inputTime;
	float endTime;
	float atkTime;
	activeTime = inputTime = endTime = atkTime = -1.0f;

	// 待機状態以外のとき、ダッシュ無効化
	if (_state != ACTION_STATE::IDLE &&
		_state != ACTION_STATE::DODGE)
	{
		paramPlayer_.isDash = false;
		// 加速度初期化
		paramChara_.velocity = AsoUtility::VECTOR_ZERO;
	}

	if (_state == ACTION_STATE::IDLE)
	{
		// 待機状態
		animNum = static_cast<int>(ANIM_STATE::IDLE);
		//paramChara_.attackState = ATTACK_STATE::NONE;
		paramPlayer_.jubCnt = 0;
		motionType_ = -1;
	}
	else if (_state == ACTION_STATE::ATTACK_JUB)
	{
		// 弱攻撃回数によって状態を変更
		stateNum = static_cast<int>(ACTION_STATE::ATTACK_JUB) + (paramPlayer_.jubCnt + 1);

		// アニメーション状態割り当て
		animNum = static_cast<int>(ANIM_STATE::JUB_1) + paramPlayer_.jubCnt;
		isLoop = false;

		// モーション割り当て
		motionType_ = static_cast<int>(MOTION_TYPE::JUB_1) + paramPlayer_.jubCnt;
		activeTime = status_.GetMotionStart(motionType_);
		inputTime = status_.GetMotionInput(motionType_);
		endTime = status_.GetMotionEnd(motionType_);
		atkTime = status_.GetMotionAtk(motionType_);
		paramChara_.atkMotion.SetMotion(activeTime, endTime, inputTime, atkTime, false);

		// 弱攻撃回数増加
		paramPlayer_.jubCnt++;
	}
	else if (_state == ACTION_STATE::ATTACK_STRONG)
	{
		// 弱攻撃回数によって状態を変更
		stateNum = static_cast<int>(ACTION_STATE::ATTACK_STRONG) + paramPlayer_.jubCnt + 1;

		// アニメーション状態割り当て
		animNum = static_cast<int>(ANIM_STATE::STRONG_1) + (paramPlayer_.jubCnt - 1);
		isLoop = false;

		// モーション割り当て
		motionType_ = static_cast<int>(MOTION_TYPE::STRONG_1) + (paramPlayer_.jubCnt - 1);
		activeTime = status_.GetMotionStart(motionType_);
		inputTime = status_.GetMotionInput(motionType_);
		endTime = status_.GetMotionEnd(motionType_);
		atkTime = status_.GetMotionAtk(motionType_);
		paramChara_.atkMotion.SetMotion(activeTime, endTime, inputTime, atkTime, false);
	}
	else if (_state == ACTION_STATE::ATTACK_SPECIAL)
	{

	}

	// アニメーション再生
	anim_->Play(animNum, isLoop);

	// 行動状態割り当て
	paramPlayer_.actionState = static_cast<ACTION_STATE>(stateNum);
}


void Player::Move(void)
{
	/*　移動処理　*/

	InputManager& input = InputManager::GetInstance();
	VECTOR inputDir = AsoUtility::VECTOR_ZERO;
	
	// 移動速度
	float speedAcc = paramChara_.speedAcc;
	float speedMax = paramChara_.speed;


	if (IsActiveAction())
	{
		if (input.PadIsAlgKeyNew(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::LEFT))
		{
			inputDir = input.GetAlgKeyDirXZ(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::LEFT);
		}

		if (AsoUtility::EqualsVZero(inputDir))
		{
			bool inputY = (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_BACK]) &&
				!input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_FRONT]) ||
				!input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_BACK]) &&
				input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_FRONT]));

			bool inputX = (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_LEFT]) &&
				!input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_RIGHT]) ||
				!input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_LEFT]) &&
				input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_RIGHT]));


			if (inputY)
			{
				if (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_BACK]))
				{
					// 奥移動処理
					inputDir.z = 1.0f;
				}

				if (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_FRONT]))
				{
					// 前移動処理
					inputDir.z = -1.0f;
				}
			}

			if (inputX)
			{
				if (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_LEFT]))
				{
					// 左移動処理
					inputDir.x = -1.0f;
				}

				if (input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_RIGHT]))
				{
					// 右移動処理
					inputDir.x = 1.0f;
				}
			}
		}
	}
	




	if (!AsoUtility::EqualsVZero(inputDir))
	{
		// カメラの角度
		Camera& camera = SceneManager::GetInstance().GetCamera();
		float cameraEulerY = camera.GetCameraRot().ToEuler().y;
		Quaternion cameraYRot = Quaternion::Euler(0.0f, cameraEulerY, 0.0f);

		// 入力方向をカメラの向きに合わせる
		VECTOR worldDir = cameraYRot.PosAxis(inputDir);
		worldDir.y = 0.0f;
		worldDir = VNorm(worldDir);

		// 移動方向を取得
		paramChara_.dir = worldDir;


		// ワールド座標を加速量に反映
		if (paramPlayer_.isDash)
		{
			// ダッシュ時の加速度割り当て
			speedAcc = (paramChara_.speedAcc * paramPlayer_.dashMult);
			speedMax = (paramChara_.speed * paramPlayer_.dashMult);
		}

		VECTOR acc = VScale(worldDir, speedAcc);
		paramChara_.velocity = VAdd(paramChara_.velocity, acc);

		// 加速ベクトルの長さを取得
		VECTOR veloXZ = { paramChara_.velocity.x, 0.0f, paramChara_.velocity.z };
		float curVelocity = VSize(veloXZ);
		if (curVelocity > speedMax)
		{
			// 加速ベクトルが上限を超えた時、スカラー倍する
			float scale = (speedMax / curVelocity);
			paramChara_.velocity.x *= scale;
			paramChara_.velocity.z *= scale;
		}
	}
	else
	{
		paramChara_.velocity.x = DecVelocityXZ(&paramChara_.velocity.x);
		paramChara_.velocity.z = DecVelocityXZ(&paramChara_.velocity.z);
	}

	// 位置に反映する
	// 移動処理
	paramChara_.pos = VAdd(paramChara_.pos, paramChara_.velocity);
}

void Player::DamageProc(void)
{

}


void Player::AnimStateIdle(void)
{
	ANIM_STATE state = static_cast<ANIM_STATE>(anim_->GetPlayType());
	InputManager& input = InputManager::GetInstance();

	// 待機アニメーション状態
	if (state == ANIM_STATE::IDLE)
	{
		// アニメーションを遷移しない
		if (AsoUtility::EqualsVZero(paramChara_.velocity)) return;

		// ダッシュ
		if (IsInputDash() && IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::DASH)); }

		// 歩行
		else if (IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::WALK)); }
	}

	else if (state == ANIM_STATE::WALK)
	{
		// ダッシュ
		if (IsInputDash() && IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::DASH)); }

		// 待機
		else if (!IsInputMove()) { anim_->Play(static_cast<int>(ANIM_STATE::IDLE)); }

		// 待機状態に戻す
		if (AsoUtility::EqualsVZero(paramChara_.velocity)) { anim_->Play(static_cast<int>(ANIM_STATE::IDLE)); }
	}
	else if (state == ANIM_STATE::DASH)
	{
		// 歩行
		if (IsInputMove() && !IsInputDash()) { anim_->Play(static_cast<int>(ANIM_STATE::WALK)); }

		// 待機
		else if (!IsInputMove()){ anim_->Play(static_cast<int>(ANIM_STATE::IDLE)); }

		// 待機状態に戻す
		if (AsoUtility::EqualsVZero(paramChara_.velocity)) { anim_->Play(static_cast<int>(ANIM_STATE::IDLE)); }
	}
}


void Player::UpdateMotion(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	// モーション更新
	paramChara_.atkMotion.Update();

	// 攻撃しているときに攻撃時間減少
	if (paramChara_.atkMotion.GetIsActionAttack())
	{
		SoundManager::SRC src = static_cast<SoundManager::SRC>(GetSoundSrc());
		
		// 効果音再生
		SoundManager::GetInstance().Play(src, false, true, 3);

		SceneManager::GetInstance().GetPerform().SetHitStop(0.005f);
		// 待機状態に戻す
		if (paramChara_.atkMotion.GetAttackState() == AttackMotion::ATTACK_STATE::NONE &&
			paramChara_.atkMotion.GetTimeAttack() < 0.0f)
		{
			ChangeActionState(ACTION_STATE::IDLE);
		}
	}

	// モーション状態の遷移不可のときは処理終了
	if (!paramChara_.atkMotion.GetIsChangeMotion()) { return; }

	// 攻撃終了時に再攻撃処理
	if (paramPlayer_.actionState != ACTION_STATE::ATTACK_JUB_END &&
		paramPlayer_.actionState != ACTION_STATE::ATTACK_SPECIAL &&
		paramPlayer_.actionState != ACTION_STATE::ATTACK_STRONG_1 &&
		paramPlayer_.actionState != ACTION_STATE::ATTACK_STRONG_2 &&
		paramPlayer_.actionState != ACTION_STATE::ATTACK_STRONG_3)
	{
		// 弱攻撃
		if (IsInputAtkJub())
		{
			const float KNOCK_Y = 1.25f;
			KnockBack(paramChara_.dir, KNOCK_Y, 0.05f);

			// 行動状態遷移
			ChangeActionState(ACTION_STATE::ATTACK_JUB);
		}

		// 強攻撃
		else if (IsInputAtkStrong())
		{
			// 行動状態遷移
			ChangeActionState(ACTION_STATE::ATTACK_STRONG);
		}
	}
}

void Player::SetPosForward(void)
{
	int forwardNum = static_cast<int>(motionType_) + 1;
	float frameRad = status_.GetMotionRadius(forwardNum);

	VECTOR forward = VScale(paramChara_.dir, frameRad);
	VECTOR pos = VAdd(paramChara_.colList[COLLISION_TYPE::BODY]->pos, forward);

	paramChara_.posForward = {pos.x, pos.y, pos.z};
}

bool Player::IsUpdateFrame(void)
{
	/*　フレーム更新の条件　*/
	bool ret = false;

	// 待機時の移動中・攻撃中
	if (paramPlayer_.actionState == ACTION_STATE::IDLE &&
		!AsoUtility::EqualsVZero(paramChara_.velocity) ||
		IsActionAttack())
	{
		ret = true;
	}
	return ret;
}

bool Player::IsActiveAction(void)const
{
	bool ret = true;

	// 攻撃中・ゲームオーバー時はfalse
	if (paramChara_.atkMotion.GetAttackState() != AttackMotion::ATTACK_STATE::NONE ||
		paramPlayer_.actionState == ACTION_STATE::GAME_OVER)
	{
		return false;
	}

	return ret;
}

bool Player::IsActionAttack(void)
{
	bool result = false;
	
	if (IsActionJub() || IsActionStrong() ||
		paramPlayer_.actionState == ACTION_STATE::ATTACK_SPECIAL)
	{
		result = true;
	}
	return result;
}

bool Player::IsActionJub(void)
{
	bool ret = false;
	ACTION_STATE state = paramPlayer_.actionState;

	if (state == ACTION_STATE::ATTACK_JUB_1 ||
		state == ACTION_STATE::ATTACK_JUB_2 ||
		state == ACTION_STATE::ATTACK_JUB_END)
	{
		ret = true;
	}

	return ret;
}

bool Player::IsActionStrong(void)
{
	bool ret = false;
	ACTION_STATE state = paramPlayer_.actionState;

	if (state == ACTION_STATE::ATTACK_STRONG_1 ||
		state == ACTION_STATE::ATTACK_STRONG_2)
	{
		ret = true;
	}

	return ret;
}

bool Player::IsInputMove(void)
{
	InputManager& input = InputManager::GetInstance();

	bool ret = false;

	// 左スティックが入力されている時
	if (input.PadIsAlgKeyNew(PAD_NO::PAD1, PAD_ALGKEY::LEFT) ||

		input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_FRONT]) ||
		input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_BACK]) ||
		input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_LEFT]) ||
		input.KeyIsNew(inputKey_[INPUT_TYPE::MOVE_RIGHT]))
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
	if (input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::LEFT) ||
		input.MouseIsTrgDown(inputKey_[INPUT_TYPE::ATTACK_JUB]))
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
	if (input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::UP) ||
		input.MouseIsTrgDown(inputKey_[INPUT_TYPE::ATTACK_STRONG]))
	{
		// 移動入力時、true
		ret = true;
	}

	return ret;
}

bool Player::IsInputDash(void)
{
	/* ダッシュ入力判定 */
	InputManager& input = InputManager::GetInstance();

	if (input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::L_STICK) ||
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::DOWN) ||
		input.KeyIsTrgDown(inputKey_[INPUT_TYPE::DASH]))
	{
		// ダッシュ切替処理
		paramPlayer_.isDash = ((!paramPlayer_.isDash) ? true : false);
	}

	return paramPlayer_.isDash;
}

int Player::GetSoundSrc(void)
{
	SoundManager::SRC sound = SoundManager::SRC::NONE;

	// 弱攻撃時
	sound = (IsActionJub() ? SoundManager::SRC::SE_SWORD_JUB : sound);

	// 強攻撃
	sound = (IsActionStrong() ? SoundManager::SRC::SE_SWORD_STRONG : sound);

	return static_cast<int>(sound);
}
