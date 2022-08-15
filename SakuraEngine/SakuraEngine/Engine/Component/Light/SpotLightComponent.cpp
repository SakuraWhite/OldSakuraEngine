#include "SpotLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"
#include "../../Component/Mesh/Core/MeshComponent.h"
#include "../../Mesh/Core/Material/Material.h"

CSpotLightComponent::CSpotLightComponent()
	:Super() //ִ�и���
	, ConicalInnerCorner(30.f) //��ʼ���۹���ڽ�
	, ConicalOuterCorner(60.f) //��ʼ���۹�����
{	
	//��ȡģ����Դ(�������ļ����ж�ȡ)
	string MeshPath = "../SakuraEngine/Asset/SpotMesh.obj";
	//ͨ��LightMesh��Ϣ ����һ���Զ����ģ��������ƽ�й���(�Դ�ģ��)
	SetLightMesh(GetMeshManage()->CreateMeshComponent(MeshPath));

	//����̫����(�Դ���mesh)Ϊ�߿�ģʽ��ʾ
	if (GetLightMesh())
	{
		
		if (CMaterial *InMaterial =  (*GetLightMesh()->GetMaterials())[0])//��ȡ��0�Ų���
		{
			InMaterial->SetMaterialType(EMaterialType::BaseColor);//���ò�������
			InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);//�����߿�ģʽ��ʾ״̬
			InMaterial->SetBaseColor(fvector_4d(1.0f, 0.6f, 1.0f, 1.0f));//������ɫ
		}
	}

	LightType = ELightType::SpotLight;//��ʼ���ƹ�����Ϊ�۹��
}

void CSpotLightComponent::SetConicalInnerCorner(float InConicalInnerCorner)
{
	//�жϾ۹���ڽ��Ƿ�С�����
	if (ConicalOuterCorner < InConicalInnerCorner)//������С���ڽ�
	{
		ConicalOuterCorner = InConicalInnerCorner;//��Ǹ�ֵ�ڽ� ��ǵĽǶȸ��ڽ�
		ConicalInnerCorner = InConicalInnerCorner;//�ڽǸ�ֵ�ڽ� �ڽǸ�����Ǳ仯
	}
	else if (ConicalOuterCorner > InConicalInnerCorner)//�����Ǵ����ڽ�
	{
		ConicalInnerCorner = InConicalInnerCorner;//���ڽǸ�ֵ�ڽ� �ڽǲ���
	}
}

void CSpotLightComponent::SetConicalOuterCorner(float InConicalOuterCorner)
{

	if (ConicalInnerCorner > InConicalOuterCorner)//����ڽǴ������
	{
		ConicalOuterCorner = InConicalOuterCorner;//��Ǹ�ֵ��� ��Ǹ����ڽǱ仯
		ConicalInnerCorner = InConicalOuterCorner;//�ڽǸ�ֵ��� �ڽǵ�ֵ�����
	}
	else if (ConicalInnerCorner < InConicalOuterCorner)//����ڽ�С�����
	{
		ConicalOuterCorner = InConicalOuterCorner;//����Ǹ�ֵ��� ��ǲ���
	}
}
