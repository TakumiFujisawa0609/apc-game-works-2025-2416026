#include "ComponentMove.h"
#include <DxLib.h>


ComponentMove::ComponentMove(void)
{
}

void ComponentMove::Move(VECTOR& _acc)
{
	pos_ = VAdd(pos_, _acc);
}
