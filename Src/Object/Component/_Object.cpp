#include "_Object.h"
#include <DxLib.h>
#include <string>
#include <memory>
#include <map>
#include <cassert>
#include "./Component/ComponentBase.h"
#include "./AnimationController.h"
#include "../Utility/Vector2.h"


_Object::_Object(void)
{
	
}


void _Object::Init(void)
{

}

void _Object::Update(void) 
{
	for (auto& component : components_)
	{
		component.second->Update();
	}
}

void _Object::Draw(void)
{
	
	for (auto& component : components_) 
	{
		component.second->Draw();
	}
}

void _Object::Release(void)
{
	

}


void _Object::UpdateComponent(void)
{
	if (components_.empty()) return;

	for (auto&& compo : components_)
	{
		if (compo.second == nullptr) continue;

		// 各コンポーネント更新処理
		compo.second->Update();
	}
}
