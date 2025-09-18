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
	targetPos_ = {};
	targetsPos_.clear();
	trackingTarget_ = nullptr;
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

	pos_ = pos;

	rot_ = Quaternion::Euler(0.0f, AsoUtility::Deg2Rad(angleY), 0.0f);

	#ifdef _DEBUG
	rot_.Euler(0.0f, 45.0f, 0.0f);
	#endif

	ResetTrackingTarget();
}

void Camera::Update(void)
{
#ifdef _DEBUG
	/*
	mode_ = MODE::FIXEX_POINT;
	float move = 2.5f;
	float rot  = 1.0f;
	if (CheckHitKey(KEY_INPUT_UP))
	{
		if (CheckHitKey(KEY_INPUT_LSHIFT))
		{
			pos_.y += move;
		}
		else if (CheckHitKey(KEY_INPUT_LCONTROL))
		{
			rot_.x += AsoUtility::Deg2Rad(rot);
		}
		else
		{
			pos_.z += move;
		}
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		if (CheckHitKey(KEY_INPUT_LSHIFT))
		{
			pos_.y -= move;
		}
		else if (CheckHitKey(KEY_INPUT_LCONTROL))
		{
			rot_.x -= AsoUtility::Deg2Rad(rot);
		}
		else
		{
			pos_.z -= move;
		}
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		if (CheckHitKey(KEY_INPUT_LCONTROL))
		{
			rot_.y += AsoUtility::Deg2Rad(rot);
		}
		else
		{
			pos_.x += move;
		}
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		if (CheckHitKey(KEY_INPUT_LCONTROL))
		{
			rot_.y -= AsoUtility::Deg2Rad(rot);
		}
		else
		{
			pos_.x -= move;
		}
	}*/

	
#endif

	if (mode_ == MODE::FLLOW)
	{
		//カメラの追従、拡大・縮小処理
		SetCameraTarget();

		//// 注視点から計算した距離分、カメラ座標を離す
		pos_ = VAdd(targetPos_, zoomPow);

		// 位置制限
		PosLimit();

		// ズーム倍率割り当て
		_SetZoomDiff(targetPos_);
	}
	else if (mode_ == MODE::FIXEX_POINT)
	{

	}
}

void Camera::Draw(void)
{
	if (mode_ == MODE::FLLOW)
	{
		//カメラセット
		SetCameraPositionAndTarget_UpVecY(
			pos_,
			targetPos_
		);
	}
	else
	{
		SetCameraPositionAndAngle(pos_, rot_.x, rot_.y, 0.0f);
	}

	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();

	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);
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

VECTOR Camera::GetPos(void) const
{
	return pos_;
}


void Camera::SetCameraPos(const VECTOR& pos, const VECTOR& angle,
						  MODE cameraMode, float freeTime, const VECTOR& targetPos)
{
	// カメラ状態を遷移
	mode_ = cameraMode;

	// 位置割り当て
	pos_ = pos;

	// 角度割り当て
	rot_ = Quaternion::Euler(angle);

	if (!AsoUtility::EqualsVZero(targetPos))
	{
		// ベクトルが割り当てられている時、追尾位置割り当て
		targetPos_ = targetPos;
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
	pos_ = ((!AsoUtility::EqualsVZero(pos)) ? pos : pos_);
}

void Camera::SetTrackingTarget(VECTOR* target)
{
	int check = 0;

	// リストに格納
	targetsPos_.emplace_back(target);

	for (auto& targetPos : targetsPos_)
	{
		VECTOR pos = *targetPos;

		if (check > 0)
		{
			// １つの対象を割り当て
			targetPos_ = pos;
		}
		else
		{
			// 中央座標を割り当て
			// targetPos_ = 中央座標

			//MATRIX mat = AsoUtility::GetMatrixRotateXYZ(rot_.);
		}

		check++;
	}
	// 追尾状態にする
	mode_ = MODE::FLLOW;
}

void Camera::ResetTrackingTarget(void)
{
	// 追尾位置を初期化
	trackingTarget_ = nullptr;

	// 追尾位置リストを初期化
	targetsPos_.clear();
}

void Camera::ZoomIn(void)
{
	pos_.y -= ZOOM_SPEED;
}

void Camera::ZoomOut(void)
{
	pos_.y += ZOOM_SPEED;
}


void Camera::SetPosLimit(VECTOR min, VECTOR max)
{
	posLimitMin_ = min;
	posLimitMax_ = max;
}


void Camera::SetCameraTarget(void)
{
	// リストが無いとき、処理終了
	if (!targetsPos_.empty())
	{
#ifdef _DEBUG
		OutputDebugString("\nカメラの追尾対象がありません\ns");
#endif
		return;
	}


	for (const VECTOR* vec : targetsPos_)
	{
		if (AsoUtility::EqualsVZero(*vec))
		{
			targetPos_ = VAdd(targetPos_, *vec);
		}
		else
		{
			VECTOR diff = {};

			// ２つの座標間のベクトル割り当て
			diff = VSub(targetPos_, *vec);

			// ２つの座標間のベクトルの半分
			VECTOR harfDiff = VScale(diff, 0.5f);

			// ２つの座標間の中心座標
			targetPos_ = VAdd(*vec, harfDiff);
		}
	}

	 // 追尾対象が2つ以上ある場合、中間点を計算
	if (targetsPos_.size() >= 2) {
		const VECTOR* p1 = targetsPos_[0];
		const VECTOR* p2 = targetsPos_[1];
		targetPos_.x = (p1->x + p2->x) * 0.5f;
		targetPos_.y = (p1->y + p2->y) * 0.5f;
		targetPos_.z = (p1->z + p2->z) * 0.5f;
	}
	// 1人だけならその座標
	else if (targetsPos_.size() == 1) {
		targetPos_ = *targetsPos_[0];
	}
}


void Camera::_SetZoomDiff(const VECTOR& vecDiff)
{
	// ベクトルがゼロのとき、処理終了
	if (AsoUtility::EqualsVZero(vecDiff))
	{
		return;
	}

	// X軸10度の方向(注視点からカメラ座標に向いた方向)
	VECTOR dir = {};
	dir.y = sinf(AsoUtility::Deg2Rad(30.0f));
	dir.z = -cosf(AsoUtility::Deg2Rad(30.0f));


	// ２つの座標間のベクトルの長さ
	float len = VSize(vecDiff);

	// 注視点からX軸10度方向に２つ座標間のベクトルの長さによって、
	// 離す距離を調整
	zoomPow = VScale(dir, len);
	zoomPow = VScale(dir, len);

	//Y軸倍率 : 最大・最小
	if (zoomPow.y < ZOOM_IN_MAX_Y)zoomPow.y = ZOOM_IN_MAX_Y;
	if (zoomPow.y > ZOOM_OUT_MAX_Y)zoomPow.y = ZOOM_OUT_MAX_Y;

	//Z軸倍率: 最大・最小
	if (zoomPow.z > -ZOOM_IN_MAX_Z)zoomPow.z = -ZOOM_IN_MAX_Z;
	if (zoomPow.z < -ZOOM_OUT_MAX_Z)zoomPow.z = -ZOOM_OUT_MAX_Z;
}

void Camera::PosLimit(void)
{
	if (!AsoUtility::EqualsVZero(posLimitMin_))
	{
		if (pos_.x < posLimitMin_.x)
		{
			pos_.x = posLimitMin_.x;
		}
		if (pos_.y < posLimitMin_.y)
		{
			pos_.y = posLimitMin_.y;
		}
		if (pos_.z < posLimitMin_.z)
		{
			pos_.z = posLimitMin_.z;
		}
	}

	if (!AsoUtility::EqualsVZero(posLimitMax_))
	{
		if (pos_.x > posLimitMax_.x)
		{
			pos_.x = posLimitMax_.x;
		}
		if (pos_.y > posLimitMax_.y)
		{
			pos_.y = posLimitMax_.y;
		}
		if (pos_.z > posLimitMax_.z)
		{
			pos_.z = posLimitMax_.z;
		}
	}
}
