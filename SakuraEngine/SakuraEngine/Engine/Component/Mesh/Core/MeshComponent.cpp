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
