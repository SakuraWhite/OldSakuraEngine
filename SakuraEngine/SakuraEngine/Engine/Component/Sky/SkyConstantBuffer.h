#pragma once
#include "../../EngineMinimal.h"

struct FFogConstantBuffer //��ĳ��������� �ṹ��
{
	FFogConstantBuffer();

	XMFLOAT3 FogColor;//�����ɫ
	float FogStart;//�����ʼ��

	float FogRange;//��ķ�Χ�뾶
	float FogHeight;//��ĸ߶�
	float FogTransparentCoefficient;//��͸��ϵ��
	float xx2;
};