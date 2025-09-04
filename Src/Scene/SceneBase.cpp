#include <DxLib.h>
#include "SceneBase.h"

SceneBase::SceneBase(void){}


void SceneBase::Load(void){}

void SceneBase::Init(void){}

void SceneBase::Update(void){}

void SceneBase::Draw(void){}

void SceneBase::Release(void){}


bool SceneBase::IsLoad(void)
{
	// ”ñ“¯Šú“Ç‚İ‚İ”‚ª‚O‚É‚È‚Á‚½‚ç“Ç‚İ‚İŠ®—¹‰»
	if (GetASyncLoadNum() == 0) return true;

	return false;
}