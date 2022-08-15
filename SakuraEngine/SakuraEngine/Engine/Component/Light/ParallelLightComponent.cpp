#include "ParallelLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"
#include "../../Component/Mesh/Core/MeshComponent.h"
#include "../../Mesh/Core/Material/Material.h"

CParallelLightComponent::CParallelLightComponent()
	:Super()
{	
	//读取模型资源(从引擎文件夹中读取)
	string MeshPath = "../SakuraEngine/Asset/SunMesh.obj";
	//通过LightMesh信息 创建一个自定义的模型来进行平行光照(自带模型)
	SetLightMesh(GetMeshManage()->CreateMeshComponent(MeshPath));

	//设置太阳光(自带的mesh)为线框模式显示
	if (GetLightMesh())
	{
		
		if (CMaterial *InMaterial =  (*GetLightMesh()->GetMaterials())[0])//获取第0号材质
		{
			InMaterial->SetMaterialType(EMaterialType::BaseColor);//设置材质类型
			InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);//设置线框模式显示状态
			InMaterial->SetBaseColor(fvector_4d(1.0f, 0.6f, 1.0f, 1.0f));//设置颜色
		}
	}
	LightType = ELightType::DirectionalLight;//初始化灯光类型为平行光
}
