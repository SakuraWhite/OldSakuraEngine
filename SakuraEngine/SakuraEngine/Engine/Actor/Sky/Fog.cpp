#include "Fog.h"
#include "../../Component/Sky/FogComponent.h"

GFog::GFog()
{
	//�������ʵ��
	FogComponent = CreateObject<CFogComponent>(new CFogComponent());
}

void GFog::SetFogColor(const fvector_color& InNewColor)
{
	//ͨ�����������������ɫ
	FogComponent->SetFogColor(InNewColor);
}

void GFog::SetFogStart(float InNewFogStart)
{
	//ͨ�����������������ʼ��
	FogComponent->SetFogStart(InNewFogStart);

}

void GFog::SetFogRange(float InNewFogRange)
{
	//ͨ���������������Χ
	FogComponent->SetFogRange(InNewFogRange);

}

void GFog::SetFogHeight(float InNewFogHeight)
{
	//ͨ���������������߶�
	FogComponent->SetFogHeight(InNewFogHeight);
}

void GFog::SetFogTransparentCoefficient(float InNewFogTransparentCoefficient)
{
	//ͨ���������������͸��ϵ��
	FogComponent->SetFogTransparentCoefficient(InNewFogTransparentCoefficient);
}
