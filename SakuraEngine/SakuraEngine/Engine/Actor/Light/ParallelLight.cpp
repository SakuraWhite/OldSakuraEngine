#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
	//���ɵƹ����                                             �����ƹ��������(ƽ�й�)
	SetLightComponent(CreateObject<CParallelLightComponent>(new CParallelLightComponent()));
}

void GParallelLight::Tick(float DeltaTime)
{
	fvector_3d v3 = GetRotation();//��õƹ���ת

	//v3.x -= DeltaTime * 5.f;//����X��
	v3.y -= DeltaTime * 5.f;//����y��
	//v3.z -= DeltaTime * 10.f;//����z��
	SetRotation(v3);//���õƹ���ת
}
