#include "Sky.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Component/Mesh/Core/MeshComponentType.h"

GSky::GSky()
	:Super()
{
	SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_BACKGROUND);//将模型设置在背景层

	CreateMesh(2.f, 100, 100, true);//设置模型半径 细分 然后反转法线
	SetScale(fvector_3d(4000.f));//设置天空球的缩放

	if (CMaterial* InMaterial = (*GetMaterials())[0])
	{
		InMaterial->SetBaseColor(fvector_4d(0.f, 0.f, 0.f, 1.f)); //设置颜色
		InMaterial->SetBaseColor("x1_CubeMap");//设置立方体贴图
		InMaterial->SetSpecular(fvector_3d(1.f));//设置高光
		InMaterial->SetMaterialType(EMaterialType::BaseColor);//设置材质类型
	}

}



void GSky::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}
