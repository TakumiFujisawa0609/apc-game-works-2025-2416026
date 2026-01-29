#pragma once
#include "../Actor/ActorBase.h"
#include <DxLib.h>
#include <map>
class Transform;

class Stage : public ActorBase
{
public:

	using SpawnPosList = std::map<std::string, VECTOR>;
	Stage(void);

	~Stage(void) = default;

	std::map<std::string, VECTOR>& GetSpawnFrames(void) { return spawnPos_; };


protected:


	void LoadPost(void)override;

	void InitPost(void)override;

	void UpdatePost(void)override;

	void DrawPost(void) override;

private:

	SpawnPosList spawnPos_;
};