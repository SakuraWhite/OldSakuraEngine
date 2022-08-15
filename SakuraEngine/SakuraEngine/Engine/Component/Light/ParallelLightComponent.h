#pragma once
#include "core/LightComponent.h"

//平行光								继承自灯光控制组件
class CParallelLightComponent :public CLightComponent   
{
	typedef CLightComponent Super;
public:
	CParallelLightComponent();
};