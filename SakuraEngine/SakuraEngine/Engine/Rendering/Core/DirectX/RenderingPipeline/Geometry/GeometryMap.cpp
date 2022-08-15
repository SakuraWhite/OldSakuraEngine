#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Mesh.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../Component/Light/SpotLightComponent.h"
#include "../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../Manage/LightManage.h"
#include "../../../../../Component/Light/Core/LightComponent.h"
#include "../../../RenderingTextureResourcesUpdate.h"
#include "../RenderLayer/RenderLayerManage.h"

UINT MeshObjectCount = 0;//累加 模型对象计数 

FGeometryMap::FGeometryMap()
{
	Geometrys.insert(pair<int, FGeometry>(0, FGeometry()) );

	RenderingTextureResources = std::make_shared<FRenderingTextureResourcesUpdate>();//纹理贴图常量缓冲区实例
}

FGeometryMap::~FGeometryMap()
{
	//将模型对象数量累加值进行析构
	MeshObjectCount = 0;
}

void FGeometryMap::PreDraw(float DeltaTime)
{	
	//预初始化
	//设置描述堆
	//DescriptorHeap.PreDraw(DeltaTime);
 	ID3D12DescriptorHeap* DescriptorHeap[] = { GetHeap() };//堆里是常量缓冲区
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);//设置描述堆
}

void FGeometryMap::Draw(float DeltaTime)
{
	//渲染绘制视口
	DrawViewport(DeltaTime);

	//渲染绘制灯光
	DrawLight(DeltaTime);

	//渲染绘制纹理贴图
	DrawTexture(DeltaTime);

	//绘制材质
	DrawMaterial(DeltaTime);

}

void FGeometryMap::PostDraw(float DeltaTime)
{

}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{

	UpdateMaterialShaderResourceView(DeltaTime, ViewportInfo);//独立更新材质着色器资源视图

	//视口投影矩阵
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);//转换成矩阵 视口投影 定义摄像机向上向左向右向前向后的一些方向设置
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);//转换成矩阵   对摄像机的裁剪距离 近远剪裁面，FOV等的设置


	//更新灯光
	FLightConstantBuffer LightConstantBuffer;//更新(创建)灯光
	for (int i = 0; i < GetLightManage()->Lights.size(); i++)//遍历灯光 
	{
		//拿到灯光组件
		if (CLightComponent* InLightComponent = GetLightManage()->Lights[i])
		{
			//将光强度的fvector_3d类型 转成XMFLOAT3类型 进入下面的计算
			fvector_3d LightIntensity = InLightComponent->GetLightIntensity();
			//获取灯光强度信息			通过组件来获取灯光强度		给灯光强度赋值
			LightConstantBuffer.SceneLights[i].LightIntensity = XMFLOAT3(LightIntensity.x, LightIntensity.y, LightIntensity.z);
			//获取灯光方向信息							通过组件来获取灯光的方向
			LightConstantBuffer.SceneLights[i].LightDirection = InLightComponent->GetForwardVector();

			//获取灯光位置信息
			LightConstantBuffer.SceneLights[i].Position = InLightComponent->GetPosition();
			//获取灯光类型信息
			LightConstantBuffer.SceneLights[i].LightType = InLightComponent->GetLightType();
			//判断灯光类型
			switch (InLightComponent->GetLightType())
			{
			case ELightType::PointLight://如果是点光源
			case ELightType::SpotLight://如果是聚光灯
			{
				if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(InLightComponent))//转换类型
				{
					//获取点光源衰减起始点
					LightConstantBuffer.SceneLights[i].StartAttenuation = InRangeLightComponent->GetStartAttenuation();
					//获取点光源衰减终点
					LightConstantBuffer.SceneLights[i].EndAttenuation = InRangeLightComponent->GetEndAttenuation();
				}

				if (InLightComponent->GetLightType() == ELightType::SpotLight)//如果判断为聚光灯
				{
					if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(InLightComponent))//转换类型
					{
						//获得聚光灯内角角度										角度转换为弧度的API  把角度转换为弧度才能使用																				
						LightConstantBuffer.SceneLights[i].ConicalInnerCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalInnerCorner());
						//获得聚光灯外角角度
						LightConstantBuffer.SceneLights[i].ConicalOuterCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalOuterCorner());
					}
				}

				break;
			}
			}
		}
	}
	LightConstantBufferViews.Update(0, &LightConstantBuffer);//更新灯光(临时)




	//更新视口投影常量缓冲区
	//XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);//转换成矩阵 视口投影
	XMMATRIX ViewProject = XMMatrixMultiply(ViewMatrix, ProjectMatrix);//把上面对象矩阵与视口投影矩阵相乘
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProject));//存储我们计算的结果

	//拿到视口位置 给摄像机位置赋值  
	ViewportTransformation.ViewportPosition = ViewportInfo.ViewPosition;

	//更新视口
	ViewportConstantBufferViews.Update(0, &ViewportTransformation);

}

void FGeometryMap::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{

	for (auto& Tmp : Geometrys)//遍历几何数据
	{
		//创建模型			输入模型哈希	模型		模型数据   模型key
		Tmp.second.BuildMesh(InMeshHash, InMesh, MeshData, Tmp.first);
	}
}

void FGeometryMap::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData)
{
	for (auto& Tmp : Geometrys)//遍历几何数据
	{
		//复制模型				传入模型  模型数据   模型key
		Tmp.second.DuplicateMesh(InMesh, MeshData, Tmp.first);
	}
}


bool FGeometryMap::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	for (auto& Tmp : Geometrys)//遍历Geometrys几何
	{
		
		if (Tmp.second.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex))//寻找通过哈希的模型渲染数据
		{
			//找到渲染数据返回true
			return true;
		}
	}

	return false;
}



void FGeometryMap::UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//在模型中更新材质
	FMaterialConstantBuffer MaterialConstantBuffer = {};

	//遍历材质信息
	for (size_t i = 0; i < Materials.size(); i++)
	{
		//具体更新材质
		if (CMaterial* InMaterial = Materials[i])  //获取遍历的材质信息 
		{
			//判断材质是否因为输入资源变化 如果输入资源产生变化就做材质更新 
			if (InMaterial->IsDirty())
			{
				//获取材质中的基本颜色
				fvector_4d InBaseColor = InMaterial->GetBaseColor();
				//材质常量缓冲区赋值 基本颜色
				MaterialConstantBuffer.BaseColor = XMFLOAT4(InBaseColor.x, InBaseColor.y, InBaseColor.z, InBaseColor.w);
				//材质常量缓冲区赋值 高光
				fvector_3d InSpecularColor = InMaterial->GetSpecularColor();
				MaterialConstantBuffer.SpecularColor = XMFLOAT3(InSpecularColor.x, InSpecularColor.y, InSpecularColor.z);

				//材质常量缓冲区赋值 粗糙度
				MaterialConstantBuffer.Roughness = InMaterial->GetRoughness();

				//材质常量缓冲区赋值 材质类型
				MaterialConstantBuffer.MaterialType = InMaterial->GetMaterialType();

				//外部资源导入
				{
					//基本颜色贴图
					//外部导入BaseColor资源	更新通过当前的渲染纹理资源寻找资源路径或key			通过材质获取到基本颜色贴图索引key
					if (auto BaseColorTextureResourcesPtr = RenderingTextureResources->FindRenderingTexture(InMaterial->GetBaseColorIndexKey()))
					{
						//常量缓冲区赋值 基本颜色贴图索引					通过上面判断后的唯一指针来获得贴图ID
						MaterialConstantBuffer.BaseColorIndex = (*BaseColorTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						//如果没有找到外部资源路径或key 则为空 -1
						MaterialConstantBuffer.BaseColorIndex = -1;
					}

					//法线贴图
					//外部导入法线资源											通过材质获取到法线贴图索引key
					if (auto NormalTextureResourcesPtr = RenderingTextureResources->FindRenderingTexture(InMaterial->GetNormalIndexKey()))
					{
						//常量缓冲区赋值 基本法线贴图索引					通过上面判断后的唯一指针来获得贴图ID
						MaterialConstantBuffer.NormalIndex = (*NormalTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						//如果没有找到外部资源路径或key 则为空 -1
						MaterialConstantBuffer.NormalIndex = -1;
					}

					//高光贴图
					//外部导入高光资源											通过材质获取到高光贴图索引key
					if (auto SpecularTextureResourcesPtr = RenderingTextureResources->FindRenderingTexture(InMaterial->GetSpecularKey()))
					{
						//常量缓冲区赋值 基本高光贴图索引					通过上面判断后的唯一指针来获得贴图ID
						MaterialConstantBuffer.SpecularIndex = (*SpecularTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						//如果没有找到外部资源路径或key 则为空 -1
						MaterialConstantBuffer.SpecularIndex = -1;
					}

				}

				//材质矩阵转换
				XMMATRIX MaterialTransform = XMLoadFloat4x4(&InMaterial->GetMaterialTransform());
				//将材质变换矩阵从行矩阵转换为列矩阵
				XMStoreFloat4x4(&MaterialConstantBuffer.TransformInformation,
					XMMatrixTranspose(MaterialTransform));

				//一旦更新过材质 就把判断材质动态更新给设置为false
				InMaterial->SetDirty(false);

				//更新材质常量缓冲视图		获取材质索引		材质常量缓冲区
				MaterialConstantBufferViews.Update(i, &MaterialConstantBuffer);
			}
		}
	}
}

void FGeometryMap::LoadTexture()
{
	def_c_paths Paths;//创建一个默认的C路径 api是教程中封装的
	init_def_c_paths(&Paths);//初始化这个路径

	char RootPath[] = "../SakuraEngine/Asset";//根路径
	find_files(RootPath, &Paths, true);//寻找路径文件  分别输入指定根路径  指定搜索的路径  是否递归

	for (int i = 0; i < Paths.index; i++)//遍历路径中的文件夹
	{
		if (find_string(Paths.paths[i], ".dds", 0) != -1)//判断文件格式是否为".dds" 从序号0号开始遍历寻找 遍历到的资源如果不为空则继续运行
		{
			//单位化路径资源
			normalization_path(Paths.paths[i]);

			wchar_t TexturePath[1024] = { 0 };
			char_to_wchar_t(TexturePath, 1024, Paths.paths[i]);//将char转换为tchar 转为宽字符

			RenderingTextureResources->LoadTextureResources(TexturePath);//读取贴图资源

		}

	}
}

void FGeometryMap::Build()
{
	//构建模型
	for (auto &Tmp : Geometrys)//遍历
	{
		Tmp.second.Build();// 对每个对象都开始构建

	}
	

}

void FGeometryMap::BuildDescriptorHeap()
{
	//构建描述堆		              	       
	DescriptorHeap.Build(  
		GetDrawObjectNumber()  // 获取当前模型对象的数量
		//+ GetDrawMaterialObjectNumber()	//	这里是加上模型中材质对象的数量  
		+ 1			//这里+1是摄像机
		+GetDrawLightObjectNumber()//这里再加灯光
		+GetDrawTextureResourcesNumber()); //这里获取渲染贴图资源数量



}

void FGeometryMap::BuildMeshConstantBuffer()
{	//创建模型常量缓冲区

	//创建对象常量缓冲区					    求字节数运算符  模型对象变换          获得绘制模型对象的数量
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawObjectNumber());

	//CPU_描述符_句柄																					获取堆启动的CPU描述符句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//构建常量缓冲区								CPU描述句柄	    获得绘制模型对象的数量
	MeshConstantBufferViews.BuildMeshConstantBuffer(DesHandle, GetDrawObjectNumber());
}

void FGeometryMap::BuildMaterialShaderResourceView()
{	//创建材质常量缓冲区

	//收集材质
	//设置shader索引
	int ShaderIndex = 0;
	//真正更新Shader-Index的地方
	//遍历渲染层级
	for(auto& Tmp : FRenderLayerManage::RenderLayers)
	{
		for (auto& InData : Tmp->RenderDatas)//再遍历 拿到渲染数据
		{
			//是否从数据中获取材质
			if (auto InMaterials = InData.Mesh->GetMaterials())
			{
				//遍历材质中的size 将数据中的size收集
				for (size_t j = 0; j < InMaterials->size(); j++)
				{
					//设置材质index
					(*InMaterials)[j]->SetMaterialIndex(ShaderIndex);
					//将数据内容收集到容器中
					Materials.push_back((*InMaterials)[j]);

					ShaderIndex++;//每遍历一次 shader索引增加
				}
			}
		}
	}
	//先进行收集数据 再进行创建
	//创建对象常量缓冲区					            				
	MaterialConstantBufferViews.CreateConstant(
		sizeof(FMaterialConstantBuffer), //求字节数运算符  材质对象变换  
		GetDrawMaterialObjectNumber(), //获得绘制材质对象的数量
		false);//这里不用内存对齐 这里并不是常量缓冲区
}


	/*
	//CPU_描述符_句柄																					获取堆启动的CPU描述符句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//构建常量缓冲区								CPU描述句柄	    
	MaterialConstantBufferViews.BuildMeshConstantBuffer(
		DesHandle, 
		GetDrawMaterialObjectNumber(),//获得绘制材质对象的数量
		GetDrawObjectNumber());//与上方模型常量缓冲区不同的是 材质常量缓冲区比模型要多出一个偏移,这里是在模型的基础上进行偏移
	*/


void FGeometryMap::BuildLightConstantBuffer()
{

	
		//创建对象常量缓冲区					    求字节数运算符  灯光对象变换    获得绘制材灯光对象的数量
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectNumber());

	//CPU_描述符_句柄																					获取堆启动的CPU描述符句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//构建常量缓冲区								CPU描述句柄	    
	LightConstantBufferViews.BuildMeshConstantBuffer(
		DesHandle,
		GetDrawLightObjectNumber(),//获得绘灯光对象的数量
		GetDrawObjectNumber());//这里是在模型缓冲区的基础上进行偏移

		//+ GetDrawMaterialObjectNumber() //在加上材质缓冲区偏移
}

UINT FGeometryMap::GetDrawObjectNumber()
{
	return Geometrys[0].GetDrawObjectNumber();//获取渲染对象的数量
	
}

UINT FGeometryMap::GetDrawMaterialObjectNumber()
{
	//直接返回材质size
	return Materials.size();
}

UINT FGeometryMap::GetDrawLightObjectNumber()
{
	return 1;
}

UINT FGeometryMap::GetDrawTextureResourcesNumber()
{
	return RenderingTextureResources->Size();//获取渲染纹理资源数量
}

void FGeometryMap::BuildTextureConstantBuffer()
{
	//构建纹理常数缓冲区
	RenderingTextureResources->BuildTextureConstantBuffer(
		DescriptorHeap.GetHeap(),		//传入描述符堆
		GetDrawObjectNumber()		//绘制模型对象数量
		+ GetDrawLightObjectNumber()	//绘制灯光对象数量
		+ 1);//+视口  然后进行偏移

		//+ GetDrawMaterialObjectNumber() //绘制材质对象数量
}

void FGeometryMap::BuildViewportConstantBufferView()
{
	//创建视口常量缓冲区					    求字节数运算符  视口变换          数量
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	//CPU_描述符_句柄																					获取堆启动的CPU描述符句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//构建视口常量缓冲区						          
	ViewportConstantBufferViews.BuildMeshConstantBuffer(
		DesHandle,  //CPU描述句柄
		1,			//偏移	
		GetDrawObjectNumber()//获取绘制对象数量 绘制模型对象数量
		+ GetDrawLightObjectNumber());//绘制灯光对象数量的基础上进行偏移

		//+GetDrawObjectNumber()//绘制材质对象数量

}

void FGeometryMap::DrawLight(float DeltaTime)
{
	//偏移  当前描述符偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//获取Gpu相关的描述符句柄
	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(
		GetDrawObjectNumber()//在模型绘制之后偏移
		,DescriptorOffset);//对Gpu相关描述符做偏移

	//设置图形 根描述符
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesHandle);
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	//偏移  当前描述符偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//获取Gpu相关的描述符句柄
	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(
		GetDrawObjectNumber() //获取模型数量
		+ GetDrawLightObjectNumber(), //获取灯光数量之后进行偏移  最后进行渲染视口
		DescriptorOffset);//对Gpu相关描述符做偏移

	//设置图形 根描述符
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	

}

void FGeometryMap::DrawMaterial(float DeltaTime)
{
	//设置更新  设置图形根源着色器资源视图
	GetGraphicsCommandList()->SetGraphicsRootShaderResourceView(
		4,//输入序号 描述表中的序号
		MaterialConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());//在常量缓冲区中拿到上传缓冲区  然后获取到GPU的地址
}

void FGeometryMap::DrawTexture(float DeltaTime)
{
	//偏移  当前描述符偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//获取Gpu相关的描述符句柄
	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(
		GetDrawObjectNumber()//模型对象数量
		+ GetDrawLightObjectNumber() //灯光对象数量
		+ 1, //视口 在以上描述之后做偏移
		DescriptorOffset);//对Gpu相关描述符做偏移

	//设置图形 根描述符
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(3, DesHandle);

}

bool FGeometry::IsRenderingDataExistence(CMeshComponent* InKey)
{
	//找到对应渲染层级											
	if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InKey->GetRenderLayerType()))
	{
		for (auto& Tmp : InRenderLayer->RenderDatas) //遍历信息
		{
			if (Tmp.Mesh == InKey)  //找到信息就返回True
			{
				return true;
			}

		}
	}
	return false;
}

void FGeometry::BuildMesh(
	const size_t InMeshHash, //输入哈希
	CMeshComponent* InMesh,	//输入模型
	const FMeshRenderingData& MeshData,//输入模型数据
	int InKey)//输入模型Key
{
	if (!IsRenderingDataExistence(InMesh))   //判断模型是否已经被添加过了
	{
		//找到对应渲染层级											
		if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
		{
			//从渲染层级里读取渲染信息
			InRenderLayer->RenderDatas.push_back(FRenderingData());//添加新的Mesh
			//拿到最初始的数据
			FRenderingData& InRenderingData = InRenderLayer->RenderDatas[InRenderLayer->RenderDatas.size() - 1];

			//基础注册
			InRenderingData.MeshObjectIndex = MeshObjectCount++;//注册模型对象索引
			InRenderingData.Mesh = InMesh;//注册Mesh
			InRenderingData.MeshHash = InMeshHash;//注册哈希
			InRenderingData.GeometryKey = InKey;//注册模型key

			InRenderingData.IndexSize = MeshData.IndexData.size();//记录Index
			InRenderingData.VertexSize = MeshData.VertexData.size();//记录顶点

			//记录Inedxsize位置偏移
			InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
			//记录顶点size位置偏移
			InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();


			//索引数据的合并
			MeshRenderingData.IndexData.insert( //高校的插入
				MeshRenderingData.IndexData.end(), //数据结尾的地址
				MeshData.IndexData.begin(),//要插入的地址开始
				MeshData.IndexData.end()); //要插入地址的结尾

			//顶点数据的合并
			MeshRenderingData.VertexData.insert(
				MeshRenderingData.VertexData.end(),
				MeshData.VertexData.begin(),
				MeshData.VertexData.end());
		}
	}

}

//缓冲池内部复制模型                      输入模型						模型数据				模型Key
void FGeometry::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData , int InKey)
{
	//复制模型

	if (!IsRenderingDataExistence(InMesh))//判断模型是否已经被添加过了
	{
		//找到对应渲染层级											
		if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
		{
			//从渲染层级里读取模型信息
			InRenderLayer->RenderDatas.push_back(MeshData);//添加新的Mesh
			//拿到最初始的数据
			FRenderingData& InRenderingData = InRenderLayer->RenderDatas[InRenderLayer->RenderDatas.size() - 1];

			//基础注册
			InRenderingData.Mesh = InMesh;
			InRenderingData.MeshObjectIndex = MeshObjectCount++;//渲染数据里的模型对象索引进行同步累加
			InRenderingData.GeometryKey = InKey;//注册模型key

		}

	}
}

bool FGeometry::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	//寻找RenderData 如果找到渲染数据则进行下一步
	auto FindMeshRenderingDataByHashSub = [&](std::shared_ptr<FRenderLayer> InRenderLayer)->FRenderingData*
	{
		for (auto& SubTmp : InRenderLayer->RenderDatas)//对模型渲染数据描述堆进行遍历
		{
			if (SubTmp.MeshHash == InHash)//判断模型的哈希是否一致
			{
				//返回数据
				return &SubTmp;
			}
		}

		return NULL;
	};

	if (InRenderLayerIndex == -1)//开启暴力遍历
	{
		//每一个层级都进行遍历
		for (auto& Tmp : FRenderLayerManage::RenderLayers)
		{
			//如果通过Sub里的渲染层级找到渲染data
			if (FRenderingData* InRenderingData = FindMeshRenderingDataByHashSub(Tmp))
			{
				//将渲染数据给模型数据中
				MeshData = *InRenderingData;
				return true;
			}
		}
	}
		//精准寻找渲染层级数据	
	else if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer(InRenderLayerIndex))
	{
		//如果通过Sub里的渲染层级找到渲染data
		if (FRenderingData* InRenderingData = FindMeshRenderingDataByHashSub(InRenderLayer))
		{
			//将渲染数据给模型数据中
			MeshData = *InRenderingData;
			return true;
		}
	}

	return false;
}



UINT FGeometry::GetDrawObjectNumber() const //获取渲染对象数量
{
	//获取渲染对象 累加（创建一个函数 然后进行++）
	return MeshObjectCount;

	int Count = 0;//计数
	for (auto& Tmp : FRenderLayerManage::RenderLayers) //通过渲染层级进行遍历
	{
		for (auto& SubTmp : Tmp->RenderDatas)
		{
			Count++;
		}
	}

	return Count;
}

void FGeometry::Build()
{
	//构建模型
	//声明参数
	UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();//声明顶点Size的字节
	UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();//声明Index的Size字节
	//创建顶点缓冲区																	 
	ANALYSIS_HRESULT(D3DCreateBlob(
		VertexSizeInBytes,//顶点Size的字节
		&CPUVertexBufferPtr));//CPU顶点的指针
	//通过MeshRenderingData拿到一些顶点Size的字节的指针与CPU顶点Buffer的首地址	最后到CPUVertexBufferPtr里面
	memcpy(CPUVertexBufferPtr->GetBufferPointer(),
		MeshRenderingData.VertexData.data(),//通过MeshRenderingData拿到VertexData
		VertexSizeInBytes);//顶点Size的字节

	//创建Index缓冲区
	ANALYSIS_HRESULT(D3DCreateBlob(
		IndexSizeInBytes,//IndexSize的字节
		&CPUIndexBufferPtr));//CPU顶点的指针

	memcpy(CPUIndexBufferPtr->GetBufferPointer(),   //拷贝值到CPUVertexBufferPtr里面
		MeshRenderingData.IndexData.data(),//通过MeshRenderingData拿到IndexData
		IndexSizeInBytes);//Index的Size字节

	//构建默认缓冲区
	//GPU顶点Buffer缓冲区
	ConstructBuffer::FConstructBuffer ConstructBuffer;//构造BUFF
	GPUVertexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		VertexBufferTmpPtr,//顶点上传缓冲区
		MeshRenderingData.VertexData.data(),//顶点数据指针
		VertexSizeInBytes); //顶点数据大小
	//GPUIndex缓冲区
	GPUIndexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		IndexBufferTmpPtr,//Index上传缓冲区
		MeshRenderingData.IndexData.data(),//Index数据指针
		IndexSizeInBytes);//Index数据大小

}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV;//创建Buffer
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();//指定Buffer位置
	VBV.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();//指定顶点缓冲区字节大小
	VBV.StrideInBytes = sizeof(FVertex);//指定每顶点的Size大小

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV;//创建INDEX
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();//指定INDEXBuffer位置
	IBV.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();//指定INDEX字节大小
	IBV.Format = DXGI_FORMAT_R16_UINT;//Index格式

	return IBV;
}


