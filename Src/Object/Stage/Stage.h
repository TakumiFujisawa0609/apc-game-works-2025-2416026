#pragma once
#include "../Actor/ActorBase.h"
#include <DxLib.h>
#include <vector>
class Transform;

class Stage : public ActorBase
{
public:

	using SpawnPosList = std::vector<VECTOR>;
	Stage(void);

	~Stage(void) = default;

	std::vector<VECTOR>& GetSpawnFrames(void);


protected:


	void LoadPost(void)override;

	void InitPost(void)override;

	void UpdatePost(void)override;

	void DrawPost(void) override;

private:

	SpawnPosList spawnPos_;
};