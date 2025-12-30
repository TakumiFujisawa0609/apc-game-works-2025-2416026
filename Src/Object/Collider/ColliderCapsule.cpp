#include "ColliderCapsule.h"
#include <DxLib.h>
#include "../Common/Transform.h"


ColliderCapsule::ColliderCapsule(TAG tag, const Transform* follow, const VECTOR& localPosTop, const VECTOR& localPosDown, float radius):
	ColliderBase(SHAPE::CAPSULE, tag, follow),
	localPosTop_(localPosTop),
	localPosDown_(localPosDown),
	radius_(radius)
{
}

VECTOR ColliderCapsule::GetPosTop(void) const
{
	return GetRotPos(localPosTop_);
}
VECTOR ColliderCapsule::GetPosDown(void) const
{
	return GetRotPos(localPosDown_);
}

VECTOR ColliderCapsule::GetCenter(void) const
{
	VECTOR top = GetPosTop();
	VECTOR down = GetPosDown();
	VECTOR diff = VSub(top, down);

	return VAdd(down, VScale(diff, 0.5f));
}

void ColliderCapsule::DrawDebug(int color)
{
	// 上の球体
	VECTOR pos1 = GetPosTop();
	DrawSphere3D(pos1, radius_, 5, color, color, false);

	// 下の球体
	VECTOR pos2 = GetPosDown();
	DrawSphere3D(pos2, radius_, 5, color, color, false);
	
	VECTOR dir;
	VECTOR s, e;

	// 球体を繋ぐ線(X+)
	dir = follow_->GetRight();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);

	// 球体を繋ぐ線(X-)
	dir = follow_->GetLeft();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);

	// 球体を繋ぐ線(Z+)
	dir = follow_->GetForward();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);

	// 球体を繋ぐ線(Z-)
	dir = follow_->GetBack();
	s = VAdd(pos1, VScale(dir, radius_));
	e = VAdd(pos2, VScale(dir, radius_));
	DrawLine3D(s, e, color);

	// カプセルの中心
	DrawSphere3D(GetCenter(), radius_, 10, color, color, true);
}

VECTOR ColliderCapsule::GetPosPushBackAlongNormal(const MV1_COLL_RESULT_POLY& hitColPoly,
												  int maxTryCnt, float pushDistance) const
{
	// コピー生成
	Transform tempTransform = *follow_;
	ColliderCapsule tmpCapsule = *this;
	tmpCapsule.SetFollow(&tempTransform);

	// 衝突補正処理
	int tryCnt = 0;
	while (tryCnt < maxTryCnt)
	{
		// カプセルと三角形の当たり判定
		if (!HitCheck_Capsule_Triangle(
			tmpCapsule.GetPosTop(), tmpCapsule.GetPosDown(),
			tmpCapsule.GetRadius(),
			hitColPoly.Position[0], hitColPoly.Position[1],
			hitColPoly.Position[2]))
		{
			break;
		}
		// 衝突していたら法線方向に押し戻し
		tempTransform.pos = VAdd(tempTransform.pos, VScale(hitColPoly.Normal, pushDistance));
		tryCnt++;
	}
	return tempTransform.pos;
}
