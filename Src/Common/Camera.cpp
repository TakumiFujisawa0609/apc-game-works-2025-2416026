#include "Camera.h"
#include "../Manager/InputManager.h"
#include "../Scene/GameScene.h"
#include "../Utility/AsoUtility.h"
#include <EffekseerForDXLib.h>
#include <DxLib.h>

Camera* Camera::instance_ = nullptr;

void Camera::CreateInstance(void)
{
	if (instance_ == nullptr) instance_ = new Camera();
	instance_->Load();
}

Camera::Camera(void)
{
	pos_ = {};
	pos_.target = {};
	targetsPos_.clear();
	rot_ = Quaternion::Identity();

	mode_ = MODE::NONE;

	targetsPos_.clear();

	Load();
}


void Camera::Load(void)
{
	rot_ = Quaternion::Identity();

}

void Camera::Init(MODE mode, const VECTOR& pos, float angleY)
{
	mode_ = mode;

	pos_.cameraPos = pos;

	zoom_.distance = CAMERA_DISTANCE;

	rot_ = Quaternion::Euler(0.0f, AsoUtility::Deg2Rad(angleY), 0.0f);

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
		SetBeforeDraw_FixexPoint();
#endif
	}
	break;

	case Camera::MODE::FLLOW:
	{
		DebugMove();
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


	if (mode_ == MODE::FLLOW)
	{
		//カメラセット
		SetCameraPositionAndTarget_UpVecY(pos_.cameraPos,
			pos_.target);
	}
	else
	{
		SetCameraPositionAndAngle(pos_.cameraPos, rot_.x, rot_.y, 0.0f);
	}

	// 追尾位置
	DrawSphere3D(pos_.target, 10.0f, 10, GetColor(255, 0, 0), GetColor(255, 0, 0), true);

	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();
}

void Camera::DebugMove(void)
{
	InputManager& input = InputManager::GetInstance();
	float move = 2.5f;
	float rot = 1.0f;

	if (input.KeyIsNew(KEY_INPUT_UP))
	{
		if (input.KeyIsNew(KEY_INPUT_LSHIFT)) { pos_.cameraPos.y += move; }

		else if (input.KeyIsNew(KEY_INPUT_RSHIFT)) { rot_.x += AsoUtility::Deg2Rad(rot); }

		else { pos_.cameraPos.z += move; }
	}
	if (input.KeyIsNew(KEY_INPUT_DOWN))
	{
		if (input.KeyIsNew(KEY_INPUT_LSHIFT)) { pos_.cameraPos.y -= move; }

		else if (input.KeyIsNew(KEY_INPUT_LCONTROL)) { rot_.x -= AsoUtility::Deg2Rad(rot); }

		else { pos_.cameraPos.z -= move; }
	}
	if (input.KeyIsNew(KEY_INPUT_RIGHT))
	{
		if (input.KeyIsNew(KEY_INPUT_LCONTROL)) { rot_.y += AsoUtility::Deg2Rad(rot); }

		else { pos_.cameraPos.x += move; }
	}
	if (input.KeyIsNew(KEY_INPUT_LEFT))
	{
		if (input.KeyIsNew(KEY_INPUT_LCONTROL)) { rot_.y -= AsoUtility::Deg2Rad(rot); }

		else { pos_.cameraPos.x -= move; }
	}
}

void Camera::SetBeforeDraw_FixexPoint(void)
{
	// 定点カメラ
	SetCameraPositionAndAngle(
		pos_.cameraPos,
		rot_.ToEuler().x,
		rot_.ToEuler().y,
		rot_.ToEuler().z
	);
}

void Camera::SetBeforeDraw_Follow(void)
{
	// カメラの移動
	// カメラの回転行列を作成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(.x));
	mat = MMult(mat, MGetRotY(angles_.y));
	//mat = MMult(mat, MGetRotZ(angles_.z));

	// 注視点の移動
	VECTOR followPos = follow_->GetPos();
	VECTOR targetLocalRotPos = VTransform(FOLLOW_TARGET_LOCAL_POS, mat);
	targetPos_ = VAdd(followPos, targetLocalRotPos);

	// カメラの移動
	// 相対座標を回転させて、回転後の相対座標を取得する
	VECTOR cameraLocalRotPos = VTransform(FOLLOW_CAMERA_LOCAL_POS, mat);

	// 相対座標からワールド座標に直して、カメラ座標とする
	pos_ = VAdd(followPos, cameraLocalRotPos);

	// カメラの上方向を計算
	VECTOR up = VTransform(AsoUtility::DIR_U, mat);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		up
	);
	
	//pos_.cameraUp = upDir;

	//SetCameraPositionAndTarget_UpVecY(pos_.cameraPos, pos_.target);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_.cameraPos,
		pos_.target,
		upDir
	);
	// 位置制限
	//PosLimit();
}

void Camera::SetBeforeDraw_FollowZoom(void)
{
	//カメラの追従、拡大・縮小処理
	SetCameraTarget();

	//// 注視点から計算した距離分、カメラ座標を離す
	pos_.cameraPos = VAdd(pos_.target, CAMERA_DISTANCE);

	// 位置制限
	PosLimit();

	// ズーム倍率割り当て
	_SetZoomDiff(pos_.target);
}




void Camera::DrawDebug(void)
{
#ifdef _DEBUG
	/*
	DrawFormatString(0, 80, 0xFFFFFF, "camera Pos(%.1f, %.1f, %.1f), Rot(%.1f,%.1f°,%.1f°,%.1f°)"
					 , pos_.x, pos_.y, pos_.z,
					 rot_.w, AsoUtility::Rad2Deg(rot_.x),AsoUtility::Rad2Deg(rot_.y),AsoUtility::Rad2Deg(rot_.z));
	DrawFormatString(0, 96, 0xFFFFFF, "camera targetPos(%.1f, %.1f, %.1f)", targetPos_.x, targetPos_.y, targetPos_.z);
	DrawFormatString(0, 112, 0xFFFFFF, "zoom Pow(%.1f, %.1f, %.1f)", zoomPow.x, zoomPow.y, zoomPow.z);
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

void Camera::Destroy(void)
{
	Release();

	delete instance_;
}

void Camera::UpdatePlayerTransform(VECTOR* pos, Quaternion* rot)
{
	targetsPos_.emplace_back(&pos);
	playerRot_ = rot;
}


void Camera::SetCameraPos(const VECTOR& pos, const VECTOR& angle,
						  MODE cameraMode, float freeTime, const VECTOR& targetPos)
{
	// カメラ状態を遷移
	mode_ = cameraMode;

	// 位置割り当て
	pos_.cameraPos = pos;

	// 角度割り当て
	rot_ = Quaternion::Euler(angle);

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

	pos_.cameraPos = pos;
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
	mode_ = MODE::FLLOW;
}

void Camera::ResetTrackingTarget(void)
{
	// 追尾位置リストを初期化
	targetsPos_.clear();
}

void Camera::ZoomIn(void)
{
	pos_.cameraPos.y -= ZOOM_SPEED;
}

void Camera::ZoomOut(void)
{
	pos_.cameraPos.y += ZOOM_SPEED;
}


void Camera::SetPosLimit(VECTOR min, VECTOR max)
{
	pos_.limitMin = min;
	pos_.limitMax = max;
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

void Camera::PosLimit(void)
{
	if (!AsoUtility::EqualsVZero(pos_.limitMin))
	{
		if (pos_.cameraPos.x < pos_.limitMin.x)
		{
			pos_.cameraPos.x = pos_.limitMin.x;
		}
		if (pos_.cameraPos.y < pos_.limitMin.y)
		{
			pos_.cameraPos.y = pos_.limitMin.y;
		}
		if (pos_.cameraPos.z < pos_.limitMin.z)
		{
			pos_.cameraPos.z = pos_.limitMin.z;
		}
	}

	if (!AsoUtility::EqualsVZero(pos_.limitMax))
	{
		if (pos_.cameraPos.x > pos_.limitMax.x)
		{
			pos_.cameraPos.x = pos_.limitMax.x;
		}
		if (pos_.cameraPos.y > pos_.limitMax.y)
		{
			pos_.cameraPos.y = pos_.limitMax.y;
		}
		if (pos_.cameraPos.z > pos_.limitMax.z)
		{
			pos_.cameraPos.z = pos_.limitMax.z;
		}
	}
}