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

bool CMeshComponent::IsDynamicReflection() const
{
	if (Materials.size() >= 1) //判断材质的序号  
	{
		return
			Materials[0]->IsDynamicReflection() &&  //获取到动态反射
			MeshRenderLayerType == EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR;//与不透明物体的反射层 满足这两个条件才可以有动态反射
	}
}
