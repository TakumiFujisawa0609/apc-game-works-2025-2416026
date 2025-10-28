#include "Player.h"
#include <DxLib.h>
#include <cassert>
#include <string>
#include <map>
#include <unordered_map>

#include "./Object.h"
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "./Status/StatusData.h"
#include "./Status/StatusPlayer.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Quaternion.h"
#include "../Common/Camera.h"
#include "./AnimationController.h"


Player::Player(void):
	status_(StatusData::GetInstance().GetPlayerStatus())
{
	paramChara_ = {};

	jumpPower_ = 0.0f;

	inputKey_.clear();

	inputType_ = INPUT_TYPE::NONE;

	paramPlayer_.isDash = false;
}

void Player::Load(void)
{
	paramChara_.handle = ResourceManager::GetInstance().LoadHandleId(ResourceManager::SRC::MODEL_PLAYER);

	Object::Load();
}

void Player::Init(const VECTOR& pos, float angleY)
{
	// 位置割り当て
	paramChara_.pos = paramChara_.prePos = pos;
	float rotY = AsoUtility::Deg2Rad(angleY);
	paramChara_.quaRot = Quaternion::Euler({ 0.0f, rotY, 0.0f });

	// 初期化処理
	Object::Init();

	/*　初期化処理　*/
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
}

void Player::SetParam(void)
{
	/*　パラメータ割り当て　*/
	
	// 行動状態
	paramPlayer_.actionState = ACTION_STATE::IDLE;

	// 攻撃状態初期化
	paramChara_.attackState = ATTACK_STATE::NONE;

	paramChara_.posLocal = {};

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
}

void Player::SetDamage(int _damage)
{
	Object::SetDamage(_damage);
}

void Player::InitAnim(void)
{
	int type;
	int max = static_cast<int>(ANIM_STATE::MAX);
	float speed = 0.0f;

	paramPlayer_.animSpeed.emplace(ANIM_STATE::IDLE, status_.GetAnimSpeedIdle());
	paramPlayer_.animSpeed.emplace(ANIM_STATE::WALK, status_.GetAnimSpeedWalk());
	paramPlayer_.animSpeed.emplace(ANIM_STATE::DASH, status_.GetAnimSpeedDash());
	paramPlayer_.animSpeed.emplace(ANIM_STATE::SWORD_SLASH, status_.GetMortionSpeed(MORTION_TYPE::JUB_1));

	for (auto& anim : paramPlayer_.animSpeed)
	{
		type = static_cast<int>(anim.first);

		speed = paramPlayer_.animSpeed[anim.first];

		// アニメーション割り当て
		anim_->AddInternal(type, speed, type);
	}

	// 待機アニメーション再生
	anim_->Play(static_cast<int>(ANIM_STATE::IDLE));
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



void Player::Update(void)
{
	/*　更新処理　*/

	Object::Update();

	// 反転回転フラグ
	bool isRevert = false;

	// 状態更新処理
	UpdateActionState();

	// 回転処理
	Rotation(isRevert);

	// アニメーション更新
	UpdateAnim();

	// 行列更新
	SetMatrixModel();

	// 当たり判定更新
	//collision_->Update();
}

void Player::Draw(void)
{
	Object::Draw();
	if (SceneManager::GetInstance().GetIsDebugMode())
	{
		// 正面座標
		if (IsActionAttack())
		{
			float forwardRad = status_.GetMortionRadius(static_cast<int>(paramPlayer_.actionState) + 1);
			DrawSphere3D(paramChara_.posForward, forwardRad,
				16, 0xffaaaa, 0xffffff, false);
		}
	}
}

void Player::DrawDebug(void)
{
#ifdef _DEBUG
	
	Object::DrawDebug();

	VECTOR pos = VAdd(paramChara_.pos, paramChara_.posLocal);
	// パラメータ描画
	DrawFormatString(0, 120, 0xFFFFFF,"player：p(%.1f, %.1f, %.1f), rot(%.1f°, %.1f° ,%.1f°), ac(%.1f°,%.1f°,%.1f°),attack(%d, %.1f秒), type(%d), anim(%d), cnt(%d)"
					 ,pos.x, pos.y, pos.z
					 ,paramChara_.rot.x, paramChara_.rot.y, paramChara_.rot.z
					 ,paramChara_.velocity.x, paramChara_.velocity.y, paramChara_.velocity.z
					 ,paramChara_.attackState, paramChara_.timeAct
		             ,paramPlayer_.actionState ,anim_->GetPlayType() ,paramPlayer_.jubCnt);
#endif
}

void Player::Release(void)
{
	/*　解放処理　*/
	Object::Release();

	// 当たり判定解放	
	//collision_->Release();
	//delete collision_;
}

void Player::UpdateActionState(void)
{
	switch (paramPlayer_.actionState)
	{
		case ACTION_STATE::IDLE:
		{
			UpdateStateIdle();
		}
		break;

		case ACTION_STATE::ATTACK_JUB_1:
		{
			// 攻撃処理
			UpdateStateAtk();
		}
		break;

		case ACTION_STATE::GAME_OVER:
		{
			// ゲームオーバー処理
			UpdateStateOver();
		}
		break;
	}
}

void Player::UpdateStateIdle(void)
{
	auto& input = InputManager::GetInstance();

	// ジャンプ
	//Jump();

	// 移動処理
	Move();

	// 攻撃処理起動
	ANIM_STATE anim = static_cast<ANIM_STATE>(anim_->GetPlayType());

	// 攻撃入力・攻撃アニメーションではないとき、攻撃有効化
	if (IsInputAtkJub())
	{
		ChangeActionState(ACTION_STATE::ATTACK_JUB_1);

		//paramChara_.timeAct = status_.GetMortionStart(MORTION_TYPE::JUB_1);
	}

	// アニメーション遷移処理
	AnimStateIdle();

	// 移動時、フレーム位置更新
	if (!AsoUtility::EqualsVZero(paramChara_.velocity))
	{
		UpdateModelFrames();
	}
}


void Player::UpdateStateAtk(void)
{
	MORTION_TYPE type = MORTION_TYPE::NONE;

	if (paramPlayer_.actionState == ACTION_STATE::ATTACK_JUB_1)
	{
		type = MORTION_TYPE::JUB_1;
		anim_->Play(static_cast<int>(ANIM_STATE::SWORD_SLASH),false);
	}

	// 攻撃モーション更新
	UpdateMortion(type);

	// フレーム位置更新
	UpdateModelFrames();

	// コンボ終了時、通常状態に戻す
	if (paramPlayer_.actionState == ACTION_STATE::ATTACK_JUB_END ||
		paramPlayer_.actionState == ACTION_STATE::ATTACK_STRONG_1 ||
		paramPlayer_.actionState == ACTION_STATE::ATTACK_STRONG_2 ||
		paramPlayer_.actionState == ACTION_STATE::ATTACK_STRONG_3)
	{
		if (anim_->IsEnd() && paramChara_.attackState == ATTACK_STATE::NONE)
		{
			ChangeActionState(ACTION_STATE::IDLE);
		}
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


void Player::SetIsAttack(bool flag)
{
	if (flag == false)
	{
		// 攻撃無効化時、攻撃時間初期化
		paramChara_.timeAct = 0.0f;
	}
}




void Player::ChangeActionState(ACTION_STATE state)
{
	paramPlayer_.actionState = state;

	if (state == ACTION_STATE::IDLE)
	{
		anim_->Play(static_cast<int>(ANIM_STATE::IDLE));
		paramChara_.attackState = ATTACK_STATE::NONE;

		paramChara_.timeAct = 0.0f;
	}
	else if (state == ACTION_STATE::ATTACK_JUB_1)
	{
		paramPlayer_.actionState = ACTION_STATE::ATTACK_JUB_1;

		anim_->Play(static_cast<int>(ANIM_STATE::SWORD_SLASH), false);
		int mortionNum = static_cast<int>(paramPlayer_.actionState) + 1;
		paramChara_.timeAct = status_.GetMortionStart(mortionNum);
		paramChara_.attackState = ATTACK_STATE::START;

		// 加速度初期化
		paramChara_.velocity = AsoUtility::VECTOR_ZERO;

		paramPlayer_.isDash = false;
	}
	else if (state == ACTION_STATE::ATTACK_STRONG_1)
	{
		int strong = static_cast<int>(ACTION_STATE::ATTACK_STRONG_1) + 1;
		paramPlayer_.actionState = static_cast<ACTION_STATE>(strong);
	}
}

bool Player::GetIsAttack(void) const
{
	bool ret = false;

	// 攻撃状態で攻撃有効時true
	if (paramChara_.attackState == Object::ATTACK_STATE::ACTIVE)
	{
		ret = true;
	}

	return ret;
}


void Player::Move(void)
{
	/*　移動処理　*/

	InputManager& input = InputManager::GetInstance();
	VECTOR inputDir = {};
	
	// ダッシュ処理
	float speedAcc = paramChara_.speedAcc;
	float speedMax = paramChara_.speed;
	DashProc(speedAcc, speedMax);

	if (GetJoypadNum() > 0 && IsActiveAction())
	{
		inputDir = input.GetAlgKeyDirXZ(InputManager::PAD_NO::PAD1, InputManager::JOYPAD_ALGKEY::LEFT);
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

		if (IsActiveAction())
		{
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

void Player::DashProc(float& _acc, float& _max)
{
	if (AsoUtility::EqualsVZero(paramChara_.velocity))
	{
		InputManager& input = InputManager::GetInstance();

		if (!input.PadIsBtnNew(PAD_NO::PAD1, PAD_BTN::L_STICK) &&
			!input.PadIsBtnNew(PAD_NO::PAD1, PAD_BTN::DOWN) ||
			!input.KeyIsNew(inputKey_[INPUT_TYPE::DASH]))
		{
			// ダッシュ入力が無効+加速度が0の時、ダッシュ無効化
			paramPlayer_.isDash = false;
		}
	}

	if (paramPlayer_.isDash)
	{
		// ダッシュ時の加速度割り当て
		_acc = (paramChara_.speedAcc * paramPlayer_.dashMult);
		_max = (paramChara_.speed * paramPlayer_.dashMult);
	}
	else
	{
		bool isChange = false;
		VECTOR velo = { paramChara_.velocity.x, 0.0f, paramChara_.velocity.z };

		// 加速度を絶対値で取得
		velo.x = ((velo.x > 0) ? velo.x : -velo.x);
		velo.z = ((velo.z > 0) ? velo.z : -velo.z);

		// ダッシュ時の加速度を通常の加速度に戻す
		if (velo.x > _max)
		{
			isChange = true;
			velo.x = _max;
		}
		if (velo.z > _max)
		{
			isChange = true;
			velo.z = _max;
		}

		// 必要に応じて加速度を戻す
		paramChara_.velocity = ((isChange) ? velo : paramChara_.velocity);
	}
}


void Player::UpdateAnim(void)
{
	// アニメーション更新
	anim_->Update();
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


void Player::UpdateMortion(MORTION_TYPE _type)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();
	
	float time[static_cast<int>(ATTACK_STATE::MAX) - 1];
	time[static_cast<int>(ATTACK_STATE::ACTIVE) - 1] = status_.GetMortionActive(_type);
	time[static_cast<int>(ATTACK_STATE::END) - 1]    = status_.GetMortionEnd(_type);

	// 攻撃しているときに攻撃時間減少
	paramChara_.timeAct -= delta;

	if (paramChara_.timeAct <= 0.0f)
	{
		// 行動の有効な時間割り当て
		int state = static_cast<int>(paramChara_.attackState);

		// 次の攻撃状態に遷移
		ChangeAttackStateNext(time[state], SoundManager::SRC::SE_SWORD_JUB);

		// 待機状態に戻す
		if (paramChara_.attackState == ATTACK_STATE::NONE)
		{
			ChangeActionState(ACTION_STATE::IDLE);
		}
	}

	// 攻撃終了時に再攻撃処理
	if (paramPlayer_.actionState != ACTION_STATE::ATTACK_JUB_END &&
		paramPlayer_.actionState != ACTION_STATE::ATTACK_STRONG_1 &&
		paramPlayer_.actionState != ACTION_STATE::ATTACK_STRONG_2 &&
		paramPlayer_.actionState != ACTION_STATE::ATTACK_STRONG_3 &&
		paramChara_.attackState == ATTACK_STATE::END)
	{
		// 弱攻撃
		if (IsInputAtkJub())
		{
			// 弱攻撃増加
			paramPlayer_.jubCnt++;

			// 再生位置初期化
			anim_->SetAnimStep();

			// 弱攻撃の状態遷移
			int jubState = static_cast<int>(ACTION_STATE::ATTACK_JUB_1) + paramPlayer_.jubCnt;
			paramPlayer_.actionState = static_cast<ACTION_STATE>(jubState);

			// 再攻撃状態遷移
			ChangeAttackState(ATTACK_STATE::START,
							  time[static_cast<int>(paramChara_.attackState)],
				              SoundManager::SRC::SE_SWORD_JUB);
		}

		// 強攻撃
		else if (IsInputAtkStrong())
		{
			int strong = static_cast<int>(ACTION_STATE::ATTACK_STRONG_1);
			strong = (paramPlayer_.jubCnt + 1);
			paramPlayer_.actionState = static_cast<ACTION_STATE>(strong);
			paramPlayer_.jubCnt = 0;
		}
	}
}

void Player::SetPosForward(void)
{
	int forwardNum = static_cast<int>(paramPlayer_.actionState) + 1;
	float frameRad = status_.GetMortionRadius(forwardNum);

	VECTOR forward = VScale(paramChara_.quaRot.GetForward(), frameRad);
	VECTOR pos = VAdd(paramChara_.colList[COLLISION_TYPE::BODY]->pos, forward);

	paramChara_.posForward = pos;
}


bool Player::IsActiveAction(void)const
{
	bool ret = true;

	// 攻撃中・ゲームオーバー時はfalse
	if (paramChara_.attackState == ATTACK_STATE::START ||
		paramChara_.attackState == ATTACK_STATE::ACTIVE ||
		paramPlayer_.actionState == ACTION_STATE::GAME_OVER)
	{
		return false;
	}

	return ret;
}

bool Player::IsActionAttack(void)
{
	bool result = false;
	ACTION_STATE state = paramPlayer_.actionState;

	if (state == ACTION_STATE::ATTACK_JUB_1 ||
		state == ACTION_STATE::ATTACK_JUB_2 ||
		state == ACTION_STATE::ATTACK_JUB_END ||

		state == ACTION_STATE::ATTACK_SPECIAL ||

		state == ACTION_STATE::ATTACK_STRONG_1 ||
		state == ACTION_STATE::ATTACK_STRONG_2 ||
		state == ACTION_STATE::ATTACK_STRONG_3)
	{
		result = true;
	}
	return result;
}

bool Player::IsInputMove(void)
{
	InputManager& input = InputManager::GetInstance();

	bool ret = false;

	// コントローラ入力時
	if (GetJoypadNum() > 0)
	{
		// 左スティックが入力されている時
		if (input.PadIsAlgKeyNew(PAD_NO::PAD1, PAD_ALGKEY::LEFT))
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

bool Player::IsInputAtkJub(void)
{
	InputManager& input = InputManager::GetInstance();

	bool ret = false;

	// コントローラ入力時
	if (GetJoypadNum() > 0)
	{
		if (input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::LEFT))
		{
			ret = true;
		}
	}

	// キーボード入力時
	else if (input.MouseIsTrgDown(inputKey_[INPUT_TYPE::ATTACK_JUB]))
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
	if (GetJoypadNum() > 0)
	{
		if (input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::UP))
		{
			ret = true;
		}
	}

	// キーボード入力時
	else if (input.MouseIsTrgDown(inputKey_[INPUT_TYPE::ATTACK_STRONG]))
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

	if (GetJoypadNum() > 0 &&
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::L_STICK) ||
		GetJoypadNum() > 0 &&
		input.PadIsBtnTrgDown(PAD_NO::PAD1, PAD_BTN::DOWN) ||

		GetJoypadNum() <= 0 &&
		input.KeyIsTrgDown(inputKey_[INPUT_TYPE::DASH]))
	{
		// ダッシュ切替処理
		paramPlayer_.isDash = ((!paramPlayer_.isDash) ? true : false);
	}

	return paramPlayer_.isDash;
}