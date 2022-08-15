#include "MeshComponent.h"
#include "../../../Mesh/Core/Material/Material.h"

CMeshComponent::CMeshComponent()
{
	//初始化模型渲染层级类型为不透明
	MeshRenderLayerType = EMeshRenderLayerType::RENDERLAYER_OPAQUE;
	//每个对象都会创建一个材质
	//创建材质
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
	MeshRenderLayerType = InRenderLayerType;//输入的渲染层级类型
}

UINT CMeshComponent::GetMaterialNum() const
{
	return Materials.size();//材质数量
}
