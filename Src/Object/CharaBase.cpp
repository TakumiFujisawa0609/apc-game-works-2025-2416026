
#include "../Utility/Vector2.h"
#include "CharaBase.h"
#include <DxLib.h>
#include <string>


/// <summary>
/// コンストラクタ処理
/// </summary>
CharaBase::CharaBase(void)
{
	
}


void CharaBase::Init(void)
{

}

void CharaBase::Update(void) 
{

}

void CharaBase::Draw(void)
{

}

void CharaBase::Release(void)
{


}

void CharaBase::SetParam(void)
{
	param_.pos = { 0.0f, 0.0f ,0.0f };
	param_.hp = 0;
	param_.state = STATE::NONE;
	param_.moveSpeed = 0.0f;
}