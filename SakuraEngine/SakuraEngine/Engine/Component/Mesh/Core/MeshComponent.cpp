#include "MeshComponent.h"
#include "../../../Mesh/Core/Material/Material.h"

CMeshComponent::CMeshComponent()
{
	//��ʼ��ģ����Ⱦ�㼶����Ϊ��͸��
	MeshRenderLayerType = EMeshRenderLayerType::RENDERLAYER_OPAQUE;
	//ÿ�����󶼻ᴴ��һ������
	//��������
	Materials.push_back(CreateObject<CMaterial>(new CMaterial()));
}

void CMeshComponent::Init()
{
}

void CMeshComponent::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

void CMeshComponent::SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType)
{
	MeshRenderLayerType = InRenderLayerType;//�������Ⱦ�㼶����
}

UINT CMeshComponent::GetMaterialNum() const
{
	return Materials.size();//��������
}

bool CMeshComponent::IsDynamicReflection() const
{
	if (Materials.size() >= 1) //�жϲ��ʵ����  
	{
		return
			Materials[0]->IsDynamicReflection() &&  //��ȡ����̬����
			MeshRenderLayerType == EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR;//�벻͸������ķ���� ���������������ſ����ж�̬����
	}
}
