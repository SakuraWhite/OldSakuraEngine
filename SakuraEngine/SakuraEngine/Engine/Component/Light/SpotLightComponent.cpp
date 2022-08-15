#include "SpotLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"
#include "../../Component/Mesh/Core/MeshComponent.h"
#include "../../Mesh/Core/Material/Material.h"

CSpotLightComponent::CSpotLightComponent()
	:Super() //执行父类
	, ConicalInnerCorner(30.f) //初始化聚光灯内角
	, ConicalOuterCorner(60.f) //初始化聚光灯外角
{	
	//读取模型资源(从引擎文件夹中读取)
	string MeshPath = "../SakuraEngine/Asset/SpotMesh.obj";
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

	LightType = ELightType::SpotLight;//初始化灯光类型为聚光灯
}

void CSpotLightComponent::SetConicalInnerCorner(float InConicalInnerCorner)
{
	//判断聚光灯内角是否小雨外角
	if (ConicalOuterCorner < InConicalInnerCorner)//如果外角小于内角
	{
		ConicalOuterCorner = InConicalInnerCorner;//外角赋值内角 外角的角度给内角
		ConicalInnerCorner = InConicalInnerCorner;//内角赋值内角 内角跟随外角变化
	}
	else if (ConicalOuterCorner > InConicalInnerCorner)//如果外角大于内角
	{
		ConicalInnerCorner = InConicalInnerCorner;//则内角赋值内角 内角不动
	}
}

void CSpotLightComponent::SetConicalOuterCorner(float InConicalOuterCorner)
{

	if (ConicalInnerCorner > InConicalOuterCorner)//如果内角大于外角
	{
		ConicalOuterCorner = InConicalOuterCorner;//外角赋值外角 外角跟随内角变化
		ConicalInnerCorner = InConicalOuterCorner;//内角赋值外角 内角的值给外角
	}
	else if (ConicalInnerCorner < InConicalOuterCorner)//如果内角小于外角
	{
		ConicalOuterCorner = InConicalOuterCorner;//则外角赋值外角 外角不动
	}
}
