#include "LightConstantBuffer.h"
#include "LightType.h"

FLightConstantBuffer::FLightConstantBuffer()
{
	//���г�ʼ��
	memset(&SceneLights, 0, sizeof(SceneLights));

}

FLight::FLight()
	:StartAttenuation(1.f) //��ʼ���ƹ�˥�����
	,EndAttenuation(10.f) //��ʼ���ƹ�˥��������
	,LightType((int)ELightType::DirectionalLight)//��ʼ���ƹ�����(ƽ�й�)
{
}
