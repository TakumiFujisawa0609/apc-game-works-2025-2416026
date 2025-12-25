#include "Stage.h"
#include <DxLib.h>
#include <vector>
#include "../Object.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"

Stage::Stage(void):
	Object::Object()
{
	Load();
}

void Stage::LoadPost(void)
{
	const float SCALE = 1.5f;

	roadObj_ = new Transform();
	roadObj_->pos = {0.0f, -15.0f, -00.0f};
	roadObj_->modelId = ResourceManager::GetInstance().LoadHandleId(ResourceManager::SRC::MODEL_ROAD);
	roadObj_->InitTransform(SCALE,
							Quaternion::Identity(),
							Quaternion::AngleAxis(AsoUtility::Deg2Rad(-90.0f), AsoUtility::AXIS_Y));

	VECTOR pos = { 0.0f, -15.0f, -250.0f };
	for (int i = 0; i < 12; i++)
	{
		Transform* obj = new Transform();
		obj->pos = pos;
		obj->modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE);
		obj->InitTransform(SCALE,
						   Quaternion::Identity(),
						   Quaternion::AngleAxis(AsoUtility::Deg2Rad(-90.0f), AsoUtility::AXIS_Y));
		object_.emplace_back(obj);

		pos.z += ((i % 3 == 0) ? 1600.0f : 1500.0f);
	}
}

void Stage::UpdatePost(void)
{
	roadObj_->Update();
	for (auto& obj : object_)
	{
		obj->Update();
	}
}

void Stage::DrawPost(void)
{
	MV1DrawModel(roadObj_->modelId);

	for (auto& obj : object_)
	{
		MV1DrawModel(obj->modelId);
	}
}
