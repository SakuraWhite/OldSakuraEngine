#include "LightComponent.h"
#include "../../../Manage/LightManage.h"
#include "../../Mesh/Core/MeshComponent.h"

CLightComponent::CLightComponent()
	:LightIntensity(1.f, 1.f, 1.f)//��ʼ���ƹ�ǿ��
{
	//�ռ��ƹ���Ϣ
	GetLightManage()->AddLight(this);
}

CLightComponent::~CLightComponent()
{
	//֮����д�Ƴ�����
}

void CLightComponent::SetLightIntensity(const fvector_3d& InNewLightIntensity)
{
	//��ֵ�µĵƹ�ǿ��
	LightIntensity = InNewLightIntensity;
}

void CLightComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	//���õƹ�λ��
	Super::SetPosition(InNewPosition);
	if (LightMesh)
	{

		//�ƹ��ģ��ģ��λ�ø��ű�����
		LightMesh->SetPosition(InNewPosition);
	}
}

void CLightComponent::SetRotation(const fvector_3d& InNewRotation)
{	
	//���õƹ���ת
	Super::SetRotation(InNewRotation);
	if (LightMesh)
	{

		//�ƹ��ģ��ģ����ת�Ƕȸ��ű仯
		LightMesh->SetRotation(InNewRotation * (-1.f));//����-1 ԭ���ǵ����ģ�͸�д�Ĵ����Ƿ���
	}

}

void CLightComponent::SetScale(const fvector_3d& InNewScale)
{
	//���õƹ�����
	Super::SetScale(InNewScale);
	if (LightMesh)
	{

		//�ƹ��ģ��ģ�����Ŵ�С���ű仯
		LightMesh->SetScale(InNewScale);
	}

}

void CLightComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	//���õƹ�������
	Super::SetForwardVector(InForwardVector);
	if (LightMesh)
	{

		//�ƹ��ģ��ģ�͹�������
		LightMesh->SetForwardVector(InForwardVector);
	}

}

void CLightComponent::SetRightVector(const XMFLOAT3& InRightVevtor)
{
	//���õƹ�������
	Super::SetRightVector(InRightVevtor);
	if (LightMesh)
	{
		//�ƹ��ģ��ģ�͹�������
		LightMesh->SetRightVector(InRightVevtor);
	}
}

void CLightComponent::SetUPVector(const XMFLOAT3& InUPVevtor)
{
	//���õƹ�������
	Super::SetUPVector(InUPVevtor);
	if (LightMesh) 
	{
		//�ƹ��ģ��ģ�͹�������
		LightMesh->SetUPVector(InUPVevtor );
	}
	
}

void CLightComponent::SetLightMesh(CMeshComponent* InLightMesh)
{
	//���õƹ�ƹ�ģ��
	LightMesh = InLightMesh;

}
