#include "_Player.h"
#include "./Component/ComponentBase.h"
#include "./Component/ComponentMove.h"

_Player::_Player(void)
{

}

void _Player::Load(void)
{
	VECTOR temp = { 0,0,0 };
	AddComponent<ComponentMove>(temp);
}

void _Player::Init(void)
{
}
