#include "FogComponent.h"

CFogComponent::CFogComponent()
	:FogColor(1.f, 1.f, 1.f, 1.f)//��ʼ������ɫ
	, FogStart(5.f) //��ʼ������ʼ��
	, FogRange(100.f)//��ʼ����Χ�뾶
	, FogHeight(9000.f)//��ʼ����߶�
	, FogTransparentCoefficient(0.f)//��ʼ��͸��ϵ��
	, bDirty(false)//��ʼ���ж����Ƿ񱻸��Ĺ�
{
}

void CFogComponent::SetFogColor(const fvector_color& InNewColor)
{
	//��������ɫ
	FogColor = InNewColor;

	SetDirty(true);//�ж��Ƿ���Ĺ�
}

void CFogComponent::SetFogStart(float InNewFogStart)
{
	//��������ʼ��
	FogStart = InNewFogStart;

	SetDirty(true);
}

void CFogComponent::SetFogRange(float InNewFogRange)
{
	//������Χ
	FogRange = InNewFogRange;

	SetDirty(true);
}

void CFogComponent::SetFogHeight(float InNewFogHeight)
{
	//������ĸ߶�
	FogHeight = InNewFogHeight;

	SetDirty(true);
}

void CFogComponent::SetFogTransparentCoefficient(float InNewFogTransparentCoefficient)
{
	//�������͸��ϵ��
	FogTransparentCoefficient = InNewFogTransparentCoefficient;
	//��
	SetDirty(true);
}

void CFogComponent::SetDirty(float InNewDirty)
{
	//�����ж�
	bDirty = InNewDirty;
}
