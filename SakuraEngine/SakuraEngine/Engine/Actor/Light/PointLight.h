#pragma  once 
#include "Core/RangeLight.h"

// 点光源控制		  继承自GLight核心对象
class GPointLight : public GRangeLight
{
	typedef GRangeLight Super;

public:
	GPointLight();

	virtual void Tick(float DeltaTime);//随着帧执行


};