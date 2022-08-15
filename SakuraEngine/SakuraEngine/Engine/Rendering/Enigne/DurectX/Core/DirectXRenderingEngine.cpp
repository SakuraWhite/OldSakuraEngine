#include "DirectXRenderingEngine.h"

#include "../../../../Debug/EngineDebug.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "../../../../Rendering/Core/Rendering.h"
#include "../../../../Mesh/BoxMesh.h"
#include "../../../../Mesh/SphereMesh.h"
#include "../../../../Mesh/CylinderMesh.h"
#include "../../../../Mesh/ConeMesh.h"
#include "../../../../Mesh/PlaneMesh.h"
#include "../../../../Mesh/CustomMesh.h"
#include "../../../../Core/CoreObject/CoreMinimalObject.h"
#include "../../../../Core/World.h"
#include "../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../Mesh/Core/MeshManage.h"
#include "../../../../Mesh/Core/Material/Material.h"
#include "../../../../Manage/LightManage.h"
#include "../../../../Actor/Light/ParallelLight.h"
#include "../../../../Actor/Light/SpotLight.h"
#include "../../../../Actor/Light/PointLight.h"


#if defined(_WIN32)
#include "../../../../Core/WinMainCommandParameters.h"

//class FVector
//{
//	unsigned char r;//255 ->[0,1]
//	unsigned char g;//255
//	unsigned char b;//255
//	unsigned char a;//255
//};

CDirectXRenderingEngine::CDirectXRenderingEngine()
	:CurrentFenceIndex(0)
	, M4XQualityLevels(0)
	, bMSAA4XEnabled(false)
	, BackBufferFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)//采用八位的RGBA
	, DepthStencilFormat(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
	, CurrentSwapBuffIndex(0)
{
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}

	bTick = false;

	MeshManage = CreateObject<CMeshManage>(new CMeshManage());//创建新的模型信息
	LightManage = CreateObject<CLightManage>(new CLightManage());//创建新的灯光信息
}

CDirectXRenderingEngine::~CDirectXRenderingEngine()
{
	//析构之后删除
	delete MeshManage;
	delete LightManage;

}

int CDirectXRenderingEngine::PreInit(FWinMainCommandParameters InParameters)
{
	Engine_Log("DirectXRenderingEngine pre initialization complete.");
	return 0;
}

int CDirectXRenderingEngine::Init(FWinMainCommandParameters InParameters)
{
	InitDirect3D();//d3d驱动

	PostInitDirect3D();

	MeshManage->Init();


	Engine_Log("DirectXRenderingEngine initialization complete.");

	return 0;
}

int CDirectXRenderingEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");

	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));
	{
		//构建Mesh
		// 自定义  注意顺序
		//	string MeshObjPath = "../../Mesh/EDMesh.obj";
		//	MeshManage->CreateMesh(MeshObjPath);

		//创建点光源
		//if (GPointLight* PointLight = World->CreateActorObject<GPointLight>())
		//{	
		//	//位置 旋转 缩放
		//	PointLight->SetPosition(XMFLOAT3(0.f, 5.f, -10.f));
		//	PointLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));
		//
		//	//强度 衰减范围控制
		//	PointLight->SetLightIntensity(fvector_3d(10.f, 10.f, 10.f));
		//	//点光源衰减起始终点
		//	PointLight->SetEndAttenuation(150.f);
		//}

		//聚灯光生成
		//if (GSpotLight* SpotLight = World->CreateActorObject<GSpotLight>())
		//{
		//	//位置 旋转 缩放
		//	SpotLight->SetPosition(XMFLOAT3(0.f, 10.f, 10.f));
		//	SpotLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));
		//
		//	//强度 衰减范围控制
		//	SpotLight->SetLightIntensity(fvector_3d(1.5f, 1.5f, 1.5f));
		//	//点光源衰减起始点
		//	SpotLight->SetStartAttenuation(1.f);
		//	//点光源衰减终点
		//	SpotLight->SetEndAttenuation(130.f);
		//
		//	//聚光灯内圈半径设置
		//	SpotLight->SetConicalInnerCorner(40.f);
		//	//聚光灯外圈半径设置
		//	SpotLight->SetConicalOuterCorner(60.f);
		//}

		//创建平行光光1  平行光（太阳光）
		if (GParallelLight* ParallelLight = World->CreateActorObject<GParallelLight>())
		{
			ParallelLight->SetPosition(XMFLOAT3(10.f, -10.f, 0.f));
			ParallelLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));

			ParallelLight->SetLightIntensity(fvector_3d(1.1f, 1.1f, 1.1f));
		}
		////创建平行光光2
		//if (GParallelLight* ParallelLight = World->CreateActorObject<GParallelLight>())
		//{
		//	ParallelLight->SetPosition(XMFLOAT3(0.f, -10.f, 10.f));
		//	ParallelLight->SetRotation(fvector_3d(-90.f, 0.f, 0.f));
		//}


		// Plane 面片									
		if (GPlaneMesh* InPlaneMesh = World->CreateActorObject<GPlaneMesh>())
		{
			//设置面片的            XY轴的长度  XY轴的段数
			InPlaneMesh->CreateMesh(4.f, 3.f, 20, 20);

			//对模型位置进行设置
			InPlaneMesh->SetPosition(XMFLOAT3(0.f, -12.f, 0.f));
			//对模型旋转进行设置
			InPlaneMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			//对模型缩放进行设置
			InPlaneMesh->SetScale(fvector_3d(50.f, 50.f, 50.f));

			if (CMaterial* InMaterial = (*InPlaneMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));//设置模型颜色
				InMaterial->SetMaterialType(EMaterialType::Lambert); 

			}
		}

		//box模型
		if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		{
			InBoxMesh->CreateMesh(5.f, 5.f, 5.f);

			InBoxMesh->SetPosition(XMFLOAT3(22.f, -10.f, 20.f));
			InBoxMesh->SetScale(fvector_3d(1));
			if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
			{
				//	InMaterial->SetBaseColor(fvector_4d(0.5f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		//Sphere 球体 
		// 兰伯特	Lambert	
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>()) //获取球体 
		{
			//设置球体的           半径 X轴与Y轴段数 
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 2.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(0.5f, 0.5f, 0.6f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::Lambert);//设置模型套用的材质类型
			}
		}

		//半兰伯特 HalfLambert
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>()) //获取球体 
		{
			//设置球体的           半径 X轴与Y轴段数
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(3.f, 2.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					221.f / 255.f,
					154.f / 255.f,
					255.f / 255.f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);//设置模型套用的材质类型
			}
		}

		//冯 Phong
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(9.f, 2.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(0.5f, 0.5f, 0.5f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::Phong);//设置模型套用的材质类型
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRoughness(0.8f);
			}
		}

		//布林冯 BinnPhong
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(9.f, 7.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					220.f / 255.f,
					223.f / 255.f,
					227.f / 255.f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::BinnPhong);//设置模型套用的材质类型
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRoughness(0.3f);
			}
		}

		//WrapLight 皮肤
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 7.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(0.5f, 0.5f, 0.5f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::Wrap);//设置模型套用的材质类型 
				InMaterial->SetRoughness(0.95f);//设置粗糙度来控制高光
			}
		}

		//Minnaert Lighting 布料 丝袜
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 7.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(0.5f, 0.5f, 0.5f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::Minnaert);//设置模型套用的材质类型  
				InMaterial->SetRoughness(0.8f);//设置粗糙度来控制高光
			}
		}

		//AnisotropyKajiyaKay   各向异性
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 2.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(0.7f, 0.7f, 1.5f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::AnisotropyKajiyaKay);//设置模型套用的材质类型  
				InMaterial->SetRoughness(0.8f);//设置粗糙度来控制高光
			}
		}

		//   Banded 基础卡通
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 12.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(0.7f, 0.7f, 1.5f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::Banded);//设置模型套用的材质类型  
			}
		}

		//GradualBanded 具有渐变的卡通材质
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 12.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					247.f / 255.f,
					150.f / 255.f,
					85.f / 255.f,
					1.0f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::GradualBanded);//设置模型套用的材质类型  
			}
		}

		//FinalBanded 混合多种效果的卡通材质  (基础反射 渐变颜色 高光等)
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(3.f, 12.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					213.f / 255.f,
					132.f / 255.f,
					234.f / 255.f,
					1.0f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::FinalBanded);//设置模型套用的材质类型  
				InMaterial->SetSpecular(fvector_3d(1.f, 1.f, 1.f));//设置高光颜色
				InMaterial->SetRoughness(0.6f);//设置粗糙度来控制高光
			}
		}

		//BackLight 玉的投射效果  次表面散射
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(9.f, 12.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					2.f / 255.f,
					214.f / 255.f,
					17.f / 255.f,
					1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::BackLight);//设置模型套用的材质类型  

				InMaterial->SetRoughness(0.2f);//设置粗糙度来控制高光
			}
		}

		//OrenNayar GDC 类沙漠粗糙表面效果
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 18.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					0.7,
					0.7,
					1.4,
					1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::OrenNayar);//设置模型套用的材质类型  
				InMaterial->SetRoughness(0.7f);//粗糙度为0时 则为兰伯特
			}
		}



		//菲涅尔
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(3.f, 7.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(0.5f, 0.5f, 0.5f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::Fresnel);//设置模型套用的材质类型 这里是在兰伯特基础上加上冯的高光
				InMaterial->SetRoughness(0.95f);//设置粗糙度来控制高光
			}
		}

		//基础线框模式 模型材质显示
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(9.f, 18.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));//设置模型基本颜色

				InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);//模型显示模式为线框显示
			
				InMaterial->SetMaterialType(EMaterialType::BaseColor);//不受光 只显示基本颜色
			}
		}

		//基础顶点模式 模型材质显示
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 18.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));//设置模型基本颜色

				InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::PointDisplay);//模型显示模式为顶点显示
			
				InMaterial->SetMaterialType(EMaterialType::BaseColor);//不受光 只显示基本颜色
			}
		}


		//只显示局部法线效果
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(3.f, 18.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Normal);//不受光 只显示局部法线
			}
		}

		//只显示世界法线效果
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(3.f, 24.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::WorldNormal);//不受光 只显示世界法线
			}
		}

		//只显示纹理贴图 方法1(直接指定材质ID)
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			//					半径   XY轴细分
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-9.f, -3.f, 0.f));
			//旋转
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("Earth");//设置基本颜色贴图
				InMaterial->SetSpecular(fvector_3d(1.f, 1.f, 1.f));//设置高光颜色
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::BinnPhong);
			}
		}

		//只显示纹理贴图 方法2(使用路径指定贴图资源)
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			//					半径   XY轴细分
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(-3.f, -3.f, 0.f));
			//旋转
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("../SakuraEngine/Asset/Texture/MMOARPG.dds");//设置基本颜色贴图
				InMaterial->SetNormal("../SakuraEngine/Asset/Texture/MMOARPG_NRM.dds");//设置基本颜色贴图
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}

		//只显示纹理贴图 方法3(在路径中遍历文件)
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			//					半径   XY轴细分
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(3.f, -3.f, 0.f));
			//旋转
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("Texture'/Project/Texture/Wood.Wood'");//设置基本颜色贴图
				InMaterial->SetNormal("Texture'/Project/Texture/Wood_NRM.Wood_NRM'");//设置基本颜色贴图
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}

		//只显示法线贴图
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			//					半径   XY轴细分
			SphereMesh->CreateMesh(2.f, 50, 50);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(9.f, -3.f, 0.f));
			//旋转
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("Wood2");//设置颜色
				InMaterial->SetNormal("Wood2_Nor");//设置法线贴图
				InMaterial->SetSpecular("Wood2_SPEC");
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));//设置模型基本颜色
				InMaterial->SetMaterialType(EMaterialType::BinnPhong);

			}
		}

		//透明
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//透明
		{
			//注册渲染层级									设置位透明层
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_TRANSPARENT);

			//					半径   XY轴细分
			SphereMesh->CreateMesh(2.f, 100, 100);
			//对模型位置进行设置
			SphereMesh->SetPosition(XMFLOAT3(15.f, 7, 0.f));
			//旋转
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("TransparentContent");//设置颜色贴图
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}


		//环境光贴图
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			//									 法线反转是否开启
			SphereMesh->CreateMesh(2.f, 100, 100, true);
			//设置位置
			SphereMesh->SetPosition(XMFLOAT3(0.f, 0.f, 0.f));
			//SphereMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
			//设置缩放
			SphereMesh->SetScale(fvector_3d(4000.f));
			//对模型材质进行设置
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("EpicQuadPanorama_CC");//设置颜色贴图
				InMaterial->SetSpecular(fvector_3d(1.f));//设置高光
				InMaterial->SetMaterialType(EMaterialType::BaseColor);
			}
		}

	}


	//渲染模型
	MeshManage->BuildMesh();
	//提交录入初始化
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);
	//CPU等待GPU初始化
	WaitGPUCommandQueueComplete();

	return 0;
}

void CDirectXRenderingEngine::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//执行数学的方法
	MeshManage->UpdateCalculations(DeltaTime, ViewportInfo);
}

void CDirectXRenderingEngine::Tick(float DeltaTime)
{
	//重置录制相关的内存，为下一帧做准备 
	ANALYSIS_HRESULT(CommandAllocator->Reset());

	//Mesh的预初始化
	MeshManage->PreDraw(DeltaTime);

	//指向哪个资源 转换其状态
	//D3D12_RESOURCE_STATE_RENDER_TARGET：我们要渲染的目标里面写入的状态
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuff(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);

	//需要每帧执行
	//绑定矩形框
	GraphicsCommandList->RSSetViewports(1, &ViewprotInfo);//1代表着绑定的数量，之后是绑定视口信息
	GraphicsCommandList->RSSetScissorRects(1, &ViewprotRect);//1代表绑定数量，之后是绑定视口矩形大小

	//清除画布
	GraphicsCommandList->ClearRenderTargetView(//我们可以使用ClearRenderTargetView来吧画布清楚为各种颜色，比如红色白色蓝色等等
		GetCurrentSwapBufferView(),//我们需要获取的视图
		DirectX::Colors::Black,		//把画布给清除为各种颜色的API
		0, nullptr);//0与nullptr是与视口相关

	//清除深度模板缓冲区
	//将当前的一个或多个的渲染目标与深度模板缓冲区绑定在输出的合并阶段
	GraphicsCommandList->ClearDepthStencilView(
		GetCurrentDepthStencilView(), //快速获取深度位置
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,//清除深度与模板 位运算
		1.f,//代表着我们用1来清除深度缓冲区
		0, //代表着我们用0来清除模板缓冲区
		0, NULL);//最后两个参数保持与DX12的API一致即可

	//指定渲染缓冲区 输出合并阶段
	D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();//指定渲染目标
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();//指定深度目标
	GraphicsCommandList->OMSetRenderTargets(
		1,//指定绑定的数量
		&SwapBufferView, //指定渲染目标
		true,//代表传入的句柄指向一个连续的描述符指针
		&DepthStencilView);//指定深度目标

	
	MeshManage->Draw(DeltaTime);//渲染的结构框架
	MeshManage->PostDraw(DeltaTime);//渲染提交

	//设置新的状态准备渲染
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuff(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 
		D3D12_RESOURCE_STATE_PRESENT);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

	//录入完成
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	//提交命令
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//交换两个buff缓冲区
	ANALYSIS_HRESULT(SwapChain->Present(0, 0));//参数里第一个0是垂直同步，第二个是inflag
	CurrentSwapBuffIndex = !(bool)CurrentSwapBuffIndex;

	//CPU等GPU
	WaitGPUCommandQueueComplete();
}

int CDirectXRenderingEngine::PreExit()
{


	Engine_Log("Engine post exit complete.");
	return 0;
}

int CDirectXRenderingEngine::Exit()
{

	Engine_Log("Engine exit complete.");
	return 0;
}

int CDirectXRenderingEngine::PostExit()
{
	

	Engine_Log("Engine post exit complete.");
	return 0;
}

ID3D12Resource* CDirectXRenderingEngine::GetCurrentSwapBuff() const //获取BUFF交换链  BUFF交换链属于渲染器的内容
{
	return SwapChainBuffer[CurrentSwapBuffIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),//获取CPU当前描述
		CurrentSwapBuffIndex, //获取内存里的当前的SwapBuffindex
		RTVDescriptorSize);//获取RTV的描述 大小是多少
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();//获取DSV深度的CPU描述
}

UINT CDirectXRenderingEngine::GetDXGISampleCount() const
{
	return bMSAA4XEnabled ? 4 : 1;;//判断采样是否开启，如果开启就输入4，否则就输入1
}

UINT CDirectXRenderingEngine::GetDXGISampleQuality() const
{
	return bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;//判断采样是否开启，如果开启那么输入采样质量的默认值减1，否则就是0
}

//GPU与CPU的一个同步
void CDirectXRenderingEngine::WaitGPUCommandQueueComplete()
{
	CurrentFenceIndex++;

	//向GPU设置新的围栏（隔离点） 等待GPU处理完成信号
	ANALYSIS_HRESULT(CommandQueue->Signal(		//新的隔离点 GPU完成后的通知
		Fence.Get(),							//之前创建好的围栏（隔离点）
		CurrentFenceIndex));					//当前的围栏（隔离点）

	//通过当前的Fence隔离点判断GPU处理完成信号
	if (Fence->GetCompletedValue() < CurrentFenceIndex)	//通过fence获取到完成的值，利用这个值与当前的值进行判断
	{
		//创建或打开一个事件内核对象，并返回该内核对象的句柄。
		//第一个参数代表这里我们不需要被子进程的来继承，所以这里返回Null    SECURITY_ATTRIBUTES
		//第二个参数NULL代表着如果我们返回Null 那么我们创建的对象事件是匿名的一个事件的内核对象
		/*第三个参数是由CREATE_EVENT_INITIAL_SET 0x00000002(代表着当前对象的初始化是否被触发 如果为否是未触发)
		与CREATE_EVENT_MANUAL_RESET 0x00000001组成  (代表着这个事件对象需要用"ResetEvents"来进行重置) */
		//第四个参数代表指定我们的事件对象所有可能的访问权限
		HANDLE EventEX = CreateEventEx(
			NULL,
			NULL, 
			0, 
			EVENT_ALL_ACCESS);

		//GPU完成后会通知我们的Handle
		ANALYSIS_HRESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		//等待GPU，会阻塞主线程,防止GPU无限提交
		WaitForSingleObject(EventEX, INFINITE);//无限等待等待
		CloseHandle(EventEX);//接收到完成的事件，然后唤醒线程，关掉Handle
	}
}

bool CDirectXRenderingEngine::InitDirect3D()
{
	//DX12 deBug
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12Debug))))
	{
		D3D12Debug->EnableDebugLayer();
	}

	////////////////////////////////////////////////////////////////////////////////////////
		//HRESULT
		//S_OK				0x00000000
		//E_UNEXPECTED		0x8000FFFF 意外的失败
		//E_NOTIMPL			0x80004001 未实现
		//E_OUTOFMEMORY		0x8007000E 未能分配所需的内存
		//E_INVALIDARG		0x80070057 一个或多个参数无效 
		//E_NOINTERFACE		0x80004002 不支持此接口
		//E_POINTER			0x80004003 无效指针
		//E_HANDLE			0x80070006 无效句柄
		//E_ABORT			0x80004004 操作终止
		//E_FAIL			0x80004005 错误
		//E_ACCESSDENIED	0x80070005 一般的访问被拒绝错误
	ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory)));

	/*
	D3D_FEATURE_LEVEL_9_1  目标功能级别支持Direct3D 9.1包含 shader model 2.
	D3D_FEATURE_LEVEL_9_2  目标功能级别支持Direct3D 9.2包含 shader model 2.
	D3D_FEATURE_LEVEL_9_3  目标功能级别支持Direct3D 9.3包含 shader model 3.
	D3D_FEATURE_LEVEL_10_0 目标功能级别支持Direct3D 10.0包含 shader model 4.
	D3D_FEATURE_LEVEL_10_1 目标功能级别支持Direct3D 10.1包含 shader model 4.
	D3D_FEATURE_LEVEL_11_0 目标功能级别支持Direct3D 11.0包含 shader model 5.
	*/
	//"NULL"指向我们的显示窗口，这里显示主窗口
	//"D3D_FEATURE_LEVEL_11_0"目标功能级别渲染 对应这DX9-DX11的各种版本
	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		//warp高级光栅化平台 可以通过软件显示适配器去模拟3D的图形硬件功能
		//如果硬件渲染失败，我们可以采用WARP软件渲染↓
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		//如果驱动失效,我们去创建WARP的驱动
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	/*
	创建Fence对象，为了CPU和GPU同步做准备
	"0"代表我们创建了一个初始值Fence围栏
	"D3D12_FENCE_FLAG_NONE"是一个枚举
	"IID_PPV_ARGS"就是为了给“Fence”赋值

	其中“D3D12_FENCE_FLAG_NONE”的解释：
	D3D12_FENCE_FLAG_NONE				  代表着我们创建了一个不共享的Fence
	D3D12_FENCE_FLAG_SHARED				  代表着当前Fence是共享的
	D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER 代表着当前Fence跟另外的一个适配器共享

	Fence->SteEventOnCompletion  通过Fence去设置EventOnCompletion
	执行命令
	提交呈现
	Queue->Signal	队列会去设置信号
	wait	等待CPU去渲染

	*/
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));//创建围栏

	//初始化命令对象
////////////////////////////////////////////////////////////////////////////////////////
	//INT Priority 
	//D3D12_COMMAND_QUEUE_PRIORITY
	//D3D12_COMMAND_QUEUE_PRIORITY_NORMAL
	//D3D12_COMMAND_QUEUE_PRIORITY_HIGH
	//NodeMask 指示该命令队列应在哪个GPU节点上执行
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;//直接
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));//队列


	//	ID3D12CommandAllocator Allocator();//创建分配器
	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,//直接类型
		IID_PPV_ARGS(CommandAllocator.GetAddressOf())));

	//创建列表列表
	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0, //默认单个Gpu 
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,//直接类型
		CommandAllocator.Get(),//创建分配器 将Commandlist关联到Allocatorr
		NULL, //传入一个接口“ID3D12PipelineState”  这个接口是当前的管线状态.这里默认一个NULL代表没有，这个时候它会自己设置一个虚拟管线状态
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));//获取当前的命令

	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	
////////////////////////////////////////////////////////////////////
	//多重采样 SSAA 或MSAA
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;//指定采样格式
	QualityLevels.SampleCount = 4;//设置支持的采样 这里是4次采样
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;//默认不支持任何选项
	QualityLevels.NumQualityLevels = 0;

	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(//检测当前设置质量是不是支持的
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, //指定检测的是当前质量级别
		&QualityLevels,							 //传入质量	
		sizeof(QualityLevels)));				//当前质量大小

	M4XQualityLevels = QualityLevels.NumQualityLevels;

	
////////////////////////////////////////////////////////////////////
	//缓冲区交换链
	SwapChain.Reset();//清除 保证我们创建的是新的交换链
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;//描述交换链的结构体
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScrrenWidth;//设定长度
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScrrenHight;//设定宽度
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;//设置帧数
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//光栅
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;//设置交换链数量
	
																					   //常用的：DXGI_USAGE_BACK_BUFFER 意思是表面或资源的用于后台缓冲区
	//DXGI_USAGE_READ_ONLY 当我们提供了表面的资源，这里只提供阅读 仅可读模式
	//DXGI_USAGE_SHADER_INPUT  使用表面或资源作为我们当前着色器的输入
	//DXGI_USAGE_SHARED 共享表面资源
	//DXGI_USAGE_UNORDERED_ACCESS 使用表面或者资源进行无序的访问
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//BUFF缓冲区的使用 "GXGI"这串代码的意思就是使用表面或资源作为输出渲染目标
	SwapChainDesc.OutputWindow = MianWindowsHandle;//指定Windows句柄
	SwapChainDesc.Windowed = true;//以窗口运行
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;//进行数据交换的时候如何处置。 这里指定的是丢弃
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//自由切换窗口大小，比如全屏或者窗口化
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;//纹理格式

	//多重采样设置
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();//判断采样是否开启，如果开启就输入4，否则就输入1
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();//判断采样是否开启，如果开启那么输入采样质量的默认值减1，否则就是0
	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(
		CommandQueue.Get(),
		&SwapChainDesc, SwapChain.GetAddressOf()));


	////////////////////////////////////////////////////////////////////
	//资源描述符
	//D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV	CBV常量缓冲区视图 SRV着色器资源视图 UAV无序访问视图
	//D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER		采样器视图
	//D3D12_DESCRIPTOR_HEAP_TYPE_RTV			渲染目标的视图资源
	//D3D12_DESCRIPTOR_HEAP_TYPE_DSV			深度/模板的视图资源

	//RTV
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&RTVDescriptorHeapDesc,
		IID_PPV_ARGS(RTVHeap.GetAddressOf())));

	//DSV
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.NumDescriptors = 1;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&DSVDescriptorHeapDesc,
		IID_PPV_ARGS(DSVHeap.GetAddressOf())));

	return false;
}

void CDirectXRenderingEngine::PostInitDirect3D()
{
	//CPU同步GPU
	WaitGPUCommandQueueComplete();

	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	DepthStencilBuffer.Reset();

	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScrrenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScrrenHight,
		BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	//拿到描述size
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle);
		HeapHandle.Offset(1, RTVDescriptorSize);
	}

	//深度与模板缓冲
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScrrenWidth;
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScrrenHight;
	ResourceDesc.Alignment = 0;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	ResourceDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	ResourceDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DepthStencilFormat;

	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(
		&Properties,
		D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON, &ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Format = DepthStencilFormat;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	GraphicsCommandList->ResourceBarrier(1, &Barrier);
	//提交录入
	GraphicsCommandList->Close();

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//这些会覆盖原先windows画布
	//描述视口尺寸
	ViewprotInfo.TopLeftX = 0;//设置渲染视口的X轴
	ViewprotInfo.TopLeftY = 0;//设置渲染视口的y轴
	ViewprotInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScrrenWidth;//设置渲染视口的宽度
	ViewprotInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScrrenHight;//设置渲染视口的高度
	ViewprotInfo.MinDepth = 0.f;//当前最小深度
	ViewprotInfo.MaxDepth = 1.f;//当前最大深度

	//绘制矩形
	ViewprotRect.left = 0;//DX绘制是左上角是原点，所以左边绘制为零即可
	ViewprotRect.top = 0;//DX绘制是左上角是原点，上边绘制为零即可
	ViewprotRect.right = FEngineRenderConfig::GetRenderConfig()->ScrrenWidth;//视口绘制宽度即可
	ViewprotRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScrrenHight;//视口绘制高度即可
	//CPU等待GPU
	WaitGPUCommandQueueComplete();
}

#endif