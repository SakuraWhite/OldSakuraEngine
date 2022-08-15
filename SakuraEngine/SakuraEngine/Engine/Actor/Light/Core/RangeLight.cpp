#include "RangeLight.h"
#include "../../../Component/Light/Core/RangeLightComponent.h"


float GRangeLight::GetStartAttenuation() const
{
	//ǿתΪ�Լ��ĵƹ����
	if (const CRangeLightComponent* InRangeLightComponent = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
	{
		//��ȡ���Դ˥����ʼ��
		return InRangeLightComponent->GetStartAttenuation();
	}

	return 0.f;
}

float GRangeLight::GetEndAttenuation() const
{
	//ǿתΪ�Լ��ĵƹ����
	if (const CRangeLightComponent* InRangeLightComponent = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
	{
		//��ȡ���Դ˥���յ�
		return InRangeLightComponent->GetEndAttenuation();
	}

	return 0.f;
}


void GRangeLight::SetStartAttenuation(float InNewStartAttenuation)
{
	if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//���õ��Դ˥����ʼ��
		InRangeLightComponent->SetStartAttenuation(InNewStartAttenuation);
	}
}

void GRangeLight::SetEndAttenuation(float InNewEndAttenuation)
{

	if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//���õ��Դ˥���յ�
		InRangeLightComponent->SetEndAttenuation(InNewEndAttenuation);
	}
}