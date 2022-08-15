#include "RangeLight.h"
#include "../../../Component/Light/Core/RangeLightComponent.h"


float GRangeLight::GetStartAttenuation() const
{
	//强转为自己的灯光组件
	if (const CRangeLightComponent* InRangeLightComponent = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
	{
		//获取点光源衰减起始点
		return InRangeLightComponent->GetStartAttenuation();
	}

	return 0.f;
}

float GRangeLight::GetEndAttenuation() const
{
	//强转为自己的灯光组件
	if (const CRangeLightComponent* InRangeLightComponent = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
	{
		//获取点光源衰减终点
		return InRangeLightComponent->GetEndAttenuation();
	}

	return 0.f;
}


void GRangeLight::SetStartAttenuation(float InNewStartAttenuation)
{
	if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//设置点光源衰减起始点
		InRangeLightComponent->SetStartAttenuation(InNewStartAttenuation);
	}
}

void GRangeLight::SetEndAttenuation(float InNewEndAttenuation)
{

	if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//设置点光源衰减终点
		InRangeLightComponent->SetEndAttenuation(InNewEndAttenuation);
	}
}