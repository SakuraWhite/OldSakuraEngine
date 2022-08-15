#pragma once
#include "Core/RangeLightComponent.h"

//平行光								继承自灯光范围控制组件
class CPointLightComponent :public CRangeLightComponent
{
	typedef CRangeLightComponent Super;

public:
	CPointLightComponent();


};