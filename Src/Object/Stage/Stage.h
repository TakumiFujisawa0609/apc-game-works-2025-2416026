#pragma once
#include "../Object.h"
#include <DxLib.h>
#include <vector>
class Transform;

class Stage : public Object
{
public:

	Stage(void);

	~Stage(void) = default;

protected:

	void SetParam(void)override {};
	void UpdateAnim(void)override {};
	void UpdatePost(void)override;
	bool IsUpdateFrame(void)override { return false; };
	void DamageProc(void)override {};
	void InitPost(void)override {};

	void LoadPost(void)override;

	void DrawPost(void) override;

	Transform* roadObj_;

	std::vector<Transform*> object_;
};