#include "PointLight.h"
#include "../../Component/Light/PointLightComponent.h"
GPointLight::GPointLight()
	:Super()
{
	//���ɵƹ����											�����ƹ��������(���Դ)
	SetLightComponent(CreateObject<CPointLightComponent>(new CPointLightComponent()));
}

void GPointLight::Tick(float DeltaTime)
{
	//fvector_3d v3 = GetRotation();//��õƹ���ת

	//v3.x += DeltaTime * 10.f;//����X��
	//v3.y += DeltaTime * 10.f;//����y��
	//v3.z += DeltaTime * 10.f;//����z��
	//SetRotation(v3);//���õƹ���ת
}




