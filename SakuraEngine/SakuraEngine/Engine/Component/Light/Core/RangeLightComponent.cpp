#include "RangeLightComponent.h"

CRangeLightComponent::CRangeLightComponent()
	:Super() //执行父类
	, StartAttenuation(1.f) //初始化点光源衰减起点
	, EndAttenuation(10.f)  //初始化点光源衰减终点
{
}
