#include "Camera.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Manager/InputManager.h"
#include "../Scene/GameScene.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Player/Player.h"


Camera::Camera(void) :
	pos_(AsoUtility::VECTOR_ZERO, AsoUtility::VECTOR_ZERO, AsoUtility::VECTOR_ZERO,
		 AsoUtility::VECTOR_ZERO, AsoUtility::VECTOR_ZERO, AsoUtility::VECTOR_ZERO),
	rot_(Quaternion::Identity(), Quaternion::Identity(), nullptr),
	follow_(nullptr),
	mode_(MODE::NONE),
	isActive_(false)
{
	targetsPos_.clear();

	Load();
}


void Camera::Load(void)
{
	
}

void Camera::Init(MODE mode, const VECTOR& pos, float angleY, Player* player)
{
	mode_ = mode;
	isActive_ = true;
	pos_.camera = pos;

	follow_ = player;

	zoom_.distance = LOCAL_CAMERA_POS;

	rot_.camera = rot_.target = Quaternion::Identity();
	rot_.camera = Quaternion::AngleAxis(AsoUtility::Deg2Rad(angleY), AsoUtility::AXIS_Y);

	ResetTrackingTarget();
}

void Camera::SetBeforeDraw(void)
{
	// カメラのクリップ距離を設定
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXEX_POINT:
	{
#ifdef _DEBUG
		/*
		SetBeforeDraw_FixexPoint();
		DebugMove();*/
#endif
	}
	break;

	case Camera::MODE::FOLLOW:
	{
		SetBeforeDraw_Follow();
	}
	break;

	case Camera::MODE::FOLLOW_AUTO_ZOOM:
	{
		SetBeforeDraw_FollowZoom();
	}
	break;

	case Camera::MODE::NONE:
	default:
		break;
	}
	

	if (mode_ != MODE::FOLLOW)
	{
		//カメラセット
		SetCameraPositionAndAngle(pos_.camera, rot_.camera.x, rot_.camera.y, 0.0f);
	}

	
#ifdef _DEBUG
	//DrawFormatString(0, 0, 0xffff00, "mouseDir:(%.2f, %.2f)", InputManager::GetInstance().GetMouseDir().x, InputManager::GetInstance().GetMouseDir().y);
	/*
	if (follow_ != nullptr)
	{
		// カメラ追尾位置描画
		VECTOR forward = VScale(follow_->GetRotation().GetForward(), 2.0f);
		VECTOR followPos = VAdd(follow_->GetPos(), forward);
		followPos = VAdd(followPos, TARGET_LOCAL_POS);
		DrawSphere3D(followPos, 10.0f, 10, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
	}*/

#endif
	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();
}

void Camera::DebugMove(void)
{
	const float move = 2.5f;
	const float rot = 1.0f;

	if (CheckHitKey(KEY_INPUT_UP))
	{
		if (CheckHitKey(KEY_INPUT_LSHIFT)) { pos_.camera.y += move; }

		else if (CheckHitKey(KEY_INPUT_RSHIFT)) { rot_.camera.x += AsoUtility::Deg2Rad(rot); }

		else { pos_.camera.z += move; }
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		if (CheckHitKey(KEY_INPUT_LSHIFT)) { pos_.camera.y -= move; }

		else if (CheckHitKey(KEY_INPUT_LCONTROL)) { rot_.camera.x -= AsoUtility::Deg2Rad(rot); }

		else { pos_.camera.z -= move; }
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		if (CheckHitKey(KEY_INPUT_LCONTROL)) { rot_.camera.y += AsoUtility::Deg2Rad(rot); }

		else { pos_.camera.x += move; }
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		if (CheckHitKey(KEY_INPUT_LCONTROL)) { rot_.camera.y -= AsoUtility::Deg2Rad(rot); }

		else { pos_.camera.x -= move; }
	}
}

void Camera::SetBeforeDraw_FixexPoint(void)
{
	// 定点カメラ
	SetCameraPositionAndAngle(
		pos_.camera,
		rot_.camera.ToEuler().x,
		rot_.camera.ToEuler().y,
		rot_.camera.ToEuler().z
	);
}

void Camera::SetBeforeDraw_Follow(void)
{
	/*　追尾カメラ　*/

	if (isActive_)
	{
		// 回転処理
		_UpdateCameraRot();
		SmoothRotation();
	}


	// カメラの移動
	// カメラの回転行列を作成
	MATRIX mat = rot_.camera.ToMatrix();


	VECTOR local = LOCAL_CAMERA_POS;
	local.z = zoom_.distance.z;

	// 注視点の移動
	VECTOR forward = VScale(follow_->GetRotation().GetForward(), 2.0f);
	VECTOR followPos = VAdd(follow_->GetPos(), forward);
	followPos = VAdd(followPos, TARGET_LOCAL_POS);

	//VECTOR targetLocalRotPos = VTransform(local, mat);
	//pos_.target = targetLocalRotPos

	// カメラの移動
	// 相対座標を回転させて、回転後の相対座標を取得する
	VECTOR cameraLocalRotPos = VTransform(local, mat);

	// 相対座標からワールド座標に直して、カメラ座標とする
	pos_.camera = VAdd(followPos, cameraLocalRotPos);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_.camera,
		followPos,
		rot_.camera.GetUp()
	);
}

void Camera::SmoothRotation(void)
{
	float smoothPow = 0.15f;
	VECTOR pPos = VAdd(follow_->GetPos(), TARGET_LOCAL_POS);
	VECTOR pAngle = follow_->GetRotationEuler();
	Quaternion pRot = follow_->GetRotation();
	

	// カメラの回転を適用してオフセットを回転
	VECTOR rotOffset = rot_.camera.PosAxis(LOCAL_CAMERA_POS);

	// プレイヤー位置に回転したオフセットを加算
	VECTOR idealPos = VAdd(pPos, zoom_.distance);

	// 現在位置を理想位置に滑らかに移動
	//pos_.cameraPos = VAdd(pos_.cameraPos, VScale(VSub(idealPos, pos_.cameraPos), smoothPow));
	pos_.camera = VAdd(pos_.camera, VSub(idealPos, pos_.camera));

	// カメラを滑らかに補間
	//rot_.camera = Quaternion::Slerp(rot_.camera, rot_.target,smoothPow);
	rot_.camera = rot_.target;
}

void Camera::_UpdateCameraRot(void)
{
	float smoothPow = 0.15f;
	float rotPow = (AsoUtility::Deg2Rad(2.5f));
	VECTOR rotInput = {};

	// マウス回転
	//if (InputManager::GetInstance().GetMouseMove().y > 0) { rotInput.x += rotPow; }
	//if (InputManager::GetInstance().GetMouseMove().y < 0) { rotInput.x -= rotPow; }
	if (InputManager::GetInstance().GetMouseMove().x > 0) { rotInput.y += rotPow; }
	if (InputManager::GetInstance().GetMouseMove().x < 0) { rotInput.y -= rotPow; }

	// コントローラ回転
	if (InputManager::GetInstance().GetKnockLStickSize().x != 0)
	{
		Vector2 dir = InputManager::GetInstance().GetKnockLStickSize();
		rotInput.y += (dir.x * rotPow);
		//rotInput.x += (dir.y * rotPow);
	}
	

	if (!AsoUtility::EqualsVZero(rotInput))
	{
		// 相対的な回転を適用
		Quaternion deltaRot = Quaternion::Euler(rotInput);
		rot_.target = rot_.target.Mult(deltaRot);
	}
}

void Camera::SetBeforeDraw_FollowZoom(void)
{
	//カメラの追従、拡大・縮小処理
	SetCameraTarget();

	// 注視点から計算した距離分、カメラ座標を離す
	pos_.camera = VAdd(pos_.target, LOCAL_CAMERA_POS);

	// 位置制限
	pos_.camera = AsoUtility::Clamp(pos_.camera, pos_.limitMin, pos_.limitMax);

	// ズーム倍率割り当て
	_SetZoomDiff(pos_.target);
}



void Camera::DrawDebug(void)
{
#ifdef _DEBUG
	/*
	DrawFormatString(0, 16, 0xFFFFFF, "camera Pos(%.1f, %.1f, %.1f), QuaRot(%.1f, %.1f ,%.1f ,%.1f)"
					 , pos_.cameraPos.x, pos_.cameraPos.y, pos_.cameraPos.z,
					 rot_.camera.w, AsoUtility::Rad2Deg(rot_.camera.x),AsoUtility::Rad2Deg(rot_.camera.y),AsoUtility::Rad2Deg(rot_.camera.z));

	DrawFormatString(0, 32, 0xFFFFFF, "target Pos(%.1f, %.1f, %.1f), QuaRot(%.1f, %.1f ,%.1f ,%.1f)", pos_.target.x, pos_.target.y, pos_.target.z,
					 rot_.target.w, AsoUtility::Rad2Deg(rot_.target.x), AsoUtility::Rad2Deg(rot_.target.y), AsoUtility::Rad2Deg(rot_.target.z));

	//DrawFormatString(0, 112, 0xFFFFFF, "zoom Pow(%.1f, %.1f, %.1f)", zoomPow.x, zoomPow.y, zoomPow.z);
	*/
#endif
}

void Camera::Release(void)
{
	if (!targetsPos_.empty())
	{
		// 追尾座標リスト解放
		targetsPos_.clear();
	}
}

void Camera::UpdatePlayerTransform(VECTOR* pos, Quaternion* rot)
{
	targetsPos_.push_back(pos);
	rot_.player = rot;
}


void Camera::SetCameraPos(const VECTOR& pos, const VECTOR& angle,
						  MODE cameraMode, float freeTime, const VECTOR& targetPos)
{
	// カメラ状態を遷移
	mode_ = cameraMode;

	// 位置割り当て
	pos_.camera = pos;

	// 角度割り当て
	rot_.camera = Quaternion::Euler(angle);

	if (!AsoUtility::EqualsVZero(targetPos))
	{
		// ベクトルが割り当てられている時、追尾位置割り当て
		pos_.target = targetPos;
	}

	// カメラ固定時、
	if (mode_ == MODE::FIXEX_POINT)
	{
		// 追尾しない時間割り当て(負の値は正の値にする)
		freeTime_ = ((freeTime > 0.0f) ? freeTime : -freeTime);
	}
}

void Camera::SetCameraMode(MODE mode, const VECTOR& pos)
{
	// モード割り当て
	mode_ = mode;

	pos_.camera = pos;
}

void Camera::SetTrackingTarget(VECTOR* target)
{
	int check = 0;

	// リストに格納
	targetsPos_.emplace_back(target);

	for (auto& targetPos : targetsPos_)
	{
		VECTOR pos = *targetPos;

		// 確認対象が要素外の時、処理終了
		if (check > targetsPos_.size()) break;

		if (check > 0)
		{
			// １つの対象を割り当て
			pos_.target = pos;
		}
		else
		{
			// 中央座標を割り当て
			pos_.target = VSub(targetPos[check], pos_.target);
		}

		check++;
	}
	// 追尾状態にする
	mode_ = MODE::FOLLOW;
}

void Camera::ResetTrackingTarget(void)
{
	// 追尾位置リストを初期化
	targetsPos_.clear();
}



void Camera::SetPosLimit(const VECTOR& _min, const VECTOR& _max)
{
	pos_.limitMin = _min;
	pos_.limitMax = _max;
}

bool Camera::GetIsCameraClip(const VECTOR& _target)
{
	VECTOR targetPos = VGet(_target.x, 0.0f, _target.z);
	VECTOR cameraPos = VGet(pos_.camera.x, 0.0f, pos_.camera.z);

	// カメラの向いている方向
	VECTOR cameraDir = VNorm(rot_.camera.GetForward());

	// カメラから見た対象の方向
	VECTOR diff = VSub(targetPos, cameraPos);
	VECTOR dirTargetFromCamera = VNorm(diff);


	// 内積を使ってベクトルの比較を行う
	// (+1.0)：２つのベクトルは同じ方向
	// (0.0)：２つのベクトルは直交
	// (-1.0)：２つのベクトルは逆方向
	float dot = VDot(cameraDir, dirTargetFromCamera);
	float angle = acosf(dot);


	const float viewAngle = 67.5f;
	const float viewRad = AsoUtility::Deg2Rad(viewAngle);

	// 追尾対象との距離
	VECTOR vec = VSub(targetPos, cameraPos);
	float distance = ((vec.x * vec.x) + (vec.z * vec.z)) / 2;
	distance *= ((distance < 0) ? -1.0f : 1.0);
	const float DISTANCE = 2000.0f;

	// 視野左右内に入っている
	return (distance <= (DISTANCE * DISTANCE)
			&& (angle <= viewRad));
}


void Camera::SetCameraTarget(void)
{
	// リストが無いとき、処理終了
	if (!targetsPos_.empty())
	{
#ifdef _DEBUG
		OutputDebugString("\nカメラの追尾対象が未割当のため、定点状態に変更しました。\n");
		mode_ = MODE::FIXEX_POINT;
#endif
		return;
	}


	for (const VECTOR* vec : targetsPos_)
	{
		if (AsoUtility::EqualsVZero(*vec))
		{
			pos_.target = VAdd(pos_.target, *vec);
		}
		else
		{
			VECTOR diff = {};

			// ２つの座標間のベクトル割り当て
			diff = VSub(pos_.target, *vec);

			// ２つの座標間のベクトルの半分
			VECTOR harfDiff = VScale(diff, 0.5f);

			// ２つの座標間の中心座標
			pos_.target = VAdd(*vec, harfDiff);
		}
	}

	 // 追尾対象が2つ以上ある場合、中間点を計算
	if (targetsPos_.size() >= 2)
	{
		const VECTOR* p1 = targetsPos_[0];
		const VECTOR* p2 = targetsPos_[1];

		pos_.target.x = (p1->x + p2->x) * 0.5f;
		pos_.target.y = (p1->y + p2->y) * 0.5f;
		pos_.target.z = (p1->z + p2->z) * 0.5f;
	}
	// 1人だけならその座標
	else if (targetsPos_.size() == 1)
	{
		pos_.target = *targetsPos_[0];
	}
}


void Camera::_SetZoomDiff(const VECTOR& vecDiff)
{
	// ベクトルがゼロのとき、処理終了
	if (AsoUtility::EqualsVZero(vecDiff)) return;

	// X軸10度の方向(注視点からカメラ座標に向いた方向)
	VECTOR dir = {};
	dir.y = sinf(AsoUtility::Deg2Rad(30.0f));
	dir.z = -cosf(AsoUtility::Deg2Rad(30.0f));


	// ２つの座標間のベクトルの長さ
	float len = VSize(vecDiff);

	// 注視点からX軸10度方向に２つ座標間のベクトルの長さによって、
	// 離す距離を調整
	zoom_.distance = VScale(dir, len);
	zoom_.distance = VScale(dir, len);

	//Y軸倍率 : 最大・最小
	if (zoom_.distance.y < ZOOM_IN_MAX.y) { zoom_.distance.y = ZOOM_IN_MAX.y; };
	if (zoom_.distance.y > ZOOM_OUT_MAX.y  ) { zoom_.distance.y = ZOOM_OUT_MAX.y; };

	//Z軸倍率: 最大・最小
	if (zoom_.distance.z > -ZOOM_IN_MAX.z) { zoom_.distance.z = -ZOOM_IN_MAX.z; };
	if (zoom_.distance.z < -ZOOM_OUT_MAX.z) { zoom_.distance.z = -ZOOM_OUT_MAX.z; };
}