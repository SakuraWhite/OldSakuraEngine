#include "SkyConstantBuffer.h"

FFogConstantBuffer::FFogConstantBuffer()
	:FogColor(1.f, 1.f, 1.f) //��ʼ������ɫ
	, FogStart(10.f) //��ʼ������ʼ��
	, FogRange(100.f)//��ʼ����Χ�뾶
	, FogHeight(100.f)//��ʼ����߶�
	, FogTransparentCoefficient(0.f)//��ʼ�����͸��ϵ��
{
}
