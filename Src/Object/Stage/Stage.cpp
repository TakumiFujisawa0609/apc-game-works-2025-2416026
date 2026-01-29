#include "Stage.h"
#include <DxLib.h>
#include <vector>
#include <string>
#include "../Actor/ActorBase.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"

Stage::Stage(void):
	ActorBase::ActorBase()
{
	Load();
}

void Stage::LoadPost(void)
{
	transform_->modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::MODEL_STAGE);
}

void Stage::InitPost(void)
{
	const float SCALE = 0.5f;
	constexpr VECTOR STAGE_POS = { 0.0f, 0.0f, -250.0f };

	transform_->InitTransform(SCALE,
						      Quaternion::Identity(), Quaternion::Identity(),
							  STAGE_POS, AsoUtility::VECTOR_ZERO);

	for (int i = 0; i < MV1GetFrameNum(transform_->modelId); i++)
	{
		const std::string FRAME_NAME = "Spawn";

		// “G¶¬ˆÊ’u‚ÌƒtƒŒ[ƒ€–¼‚ÌÀ•W‚ðŠi”[
		std::string name = MV1GetFrameName(transform_->modelId, i);
		if (name.find(FRAME_NAME) != std::string::npos)
		{
			VECTOR pos = MV1GetFramePosition(transform_->modelId, i);
			spawnPos_.emplace(name, pos);
		}
	}
}

void Stage::UpdatePost(void)
{
}

void Stage::DrawPost(void)
{

}