#include "SpotLight.h"
#include "../../Component/Light/SpotLightComponent.h"

//���Զ�����ֵ
float index_test = 0.f;

GSpotLight::GSpotLight()
	:Super()
{
	//���ɵƹ����										�����ƹ��������(���Դ)
	SetLightComponent(CreateObject<CSpotLightComponent>(new CSpotLightComponent()));
	index_test = 0.f;
}

void GSpotLight::Tick(float DeltaTime)
{
	index_test += DeltaTime;//�ۼ�
	float I = 30.f;//�ڽ�Ȧ
	float O = 50.f;//���Ȧ
	//SetConicalInnerCorner(I * fabsf(cos(index_test)) + 10.f);//�ڽ�Ȧ����cos���ڲ���
	//SetConicalOuterCorner(O * fabsf(cos(index_test)) + 20.f);//���Ȧ����cos���ڲ���

	fvector_3d v3 = GetRotation();//��õƹ���ת
	v3.x += DeltaTime * 10.f;//����X��
	//v3.y += DeltaTime * 10.f;//����y��
	//v3.z += DeltaTime * 10.f;//����z��
	SetRotation(v3);//���õƹ���ת
}

float GSpotLight::GetConicalInnerCorner() const
{
	//ǿתΪ�Լ��ĵƹ����
	if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//��ȡ�۹�Ƶ��ڽ�
		InSpotLightComponent->GetConicalInnerCorner();
	}
	return 0.0f;
}

float GSpotLight::GetConicalOuterCorner() const
{
	if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//��ȡ�۹�Ƶ����
		InSpotLightComponent->GetConicalOuterCorner();
	}
	return 0.0f;
}

void GSpotLight::SetConicalInnerCorner(float InConicalInnerCorner)
{
	if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//���þ۹���ڽ�
		InSpotLightComponent->SetConicalInnerCorner(InConicalInnerCorner);
	}
}

void GSpotLight::SetConicalOuterCorner(float InConicalOuterCorner)
{
	if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//���þ۹�����
		InSpotLightComponent->SetConicalOuterCorner(InConicalOuterCorner);
	}

}


