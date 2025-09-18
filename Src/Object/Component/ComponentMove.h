#pragma once
#include "./ComponentBase.h"
#include <DxLib.h>

class ComponentMove : public ComponentBase
{
public:

	VECTOR pos_;
	ComponentMove(void);

	ComponentMove(const VECTOR& _pos)
		: pos_(_pos) {};

	~ComponentMove(void) = default;


	void Move(VECTOR& _acc);
private:
};