#pragma once
#include <memory>
class Object;

class ComponentBase
{
public:

	virtual ~ComponentBase(void) = default;

	
	virtual void Load(void) {};

	virtual void Start(void) {};

	virtual void Update(void) {};

	virtual void Draw(void) {};

	virtual void Release(void) {};


private:

	std::weak_ptr<Object> actor_;
};