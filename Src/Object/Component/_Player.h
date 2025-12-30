#pragma once
#include "./Object.h"

class _Player : public Object
{
public:

	_Player(void);

	~_Player(void) = default;


	void Load(void)override;

	void Init(void)override;

	
private:


};