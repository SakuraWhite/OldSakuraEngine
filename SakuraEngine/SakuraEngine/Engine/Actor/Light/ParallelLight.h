#pragma  once 
#include "Core/Light.h"
#include "../../Component/Light/ParallelLightComponent.h"

// 平行光控制		  继承自GLight核心对象
class GParallelLight : public GLight
{
	typedef GLight super;


public:
	GParallelLight();

	virtual void Tick(float DeltaTime);//随着帧执行
};