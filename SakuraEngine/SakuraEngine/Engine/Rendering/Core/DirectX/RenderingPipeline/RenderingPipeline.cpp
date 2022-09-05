#include "RenderingPipeline.h"
#include "../../../../Component/Mesh/Core/MeshComponentType.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMeshHash, InMesh, MeshData);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData)
{
	//复制模型
	GeometryMap.DuplicateMesh(InMesh, MeshData);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	//返回通过哈希寻找的模型渲染数据
	return GeometryMap.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//更新动态Cubemap 计算常量缓冲区
	DynamicCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
	//更新几何map里 计算常量缓冲区
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
	//渲染层级调用 计算常量缓冲区
	RenderLayer.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{	//构建管线
	
	//初始化GPS描述
	DirectXPipelineState.ResetGPSDesc();//重置 图形_管线_状态_描述

	//渲染层级初始化		几何				DX管线状态
	RenderLayer.Init(&GeometryMap, &DirectXPipelineState);
	//统一对渲染层级进行排序
	RenderLayer.Sort();

	//纹理资源读取 需要先读取贴图 再构建根签名
	GeometryMap.LoadTexture();
	//构建雾气
	GeometryMap.BuildFog();

	//构建动态的CubeMap 初始化
	DynamicCubeMap.Init(
		&GeometryMap,  //几何map
		&DirectXPipelineState,//DX管线状态
		&RenderLayer);//渲染层级

	//构建根签名						构建根签名时我们可以通过几何模型来获取纹理贴图资源数量
	RootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DResourcesNumber());//构建根签名
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());//将根签名绑定在渲染管线状态上

	//渲染每个层级的Shader
	//RenderLayer.BuildShader();

	//构建模型
	GeometryMap.Build();

	//构建动态反射Mesh
	GeometryMap.BuildDynamicReflectionMesh();

	//构建CBV常量描述堆 常量缓冲区视图(constant buffer view)
	GeometryMap.BuildDescriptorHeap();


	//初始化动态CubeMap 摄像机 构建视口
	DynamicCubeMap.BuildViewport(fvector_3d(1.f, 1.f, 1.f));//输入的位置为自己设置的反射球的位置 可以随便给 已经可以同步更新

	//构建RTVDes 渲染目标描述堆
	DynamicCubeMap.BuildRenderTargetDescriptor();

	//构建深度模板描述 动态Cubemap
	DynamicCubeMap.BuildDepthStencilDescriptor();

	//构建深度模板  动态Cubemap
	DynamicCubeMap.BuildDepthStencil();

	//构建模型常量缓冲区
	GeometryMap.BuildMeshConstantBuffer();

	//构建材质常量缓冲区
	GeometryMap.BuildMaterialShaderResourceView();

	//构建灯光常量缓冲区
	GeometryMap.BuildLightConstantBuffer();

	//构建视口常量缓冲区视图
	GeometryMap.BuildViewportConstantBufferView();

	//构建贴图常量缓冲区
	GeometryMap.BuildTextureConstantBuffer();

	//构建雾气常量缓冲区
	GeometryMap.BuildFogConstantBuffer();

	//构建管线状态对象参数  PSO参数 
	//DirectXPipelineState.BuildParam();


	//通过渲染层级来构建管线状态 PSO
	RenderLayer.BuildPSO();

}


void FRenderingPipeline::PreDraw(float DeltaTime)
{
	//预初始化  （初始化PSO）
	DirectXPipelineState.PreDraw(DeltaTime);
	
	//几何模型
	GeometryMap.PreDraw(DeltaTime);
	//设置根签名
	RootSignature.PreDraw(DeltaTime);

	//渲染灯光贴图材质雾气等
	GeometryMap.Draw(DeltaTime);//几何map渲染

	//主视口清除画布
	ClearMainSwapChainCanvas();
	//预渲染动态Cubemap
	if (DynamicCubeMap.IsExitDynamicReflectionMesh()) //判断是否有动态反射
	{
		DynamicCubeMap.PreDraw(DeltaTime);//进行渲染动态反射Cubemap
	}

	//渲染层级调用预渲染
	RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	//主视口 从几何map里调用绘制视口
	GeometryMap.DrawViewport(DeltaTime);


	//绘制CubeMap 覆盖原先被修改的CubeMap
	GeometryMap.DrawCubeMapTexture(DeltaTime);

	//渲染各类层级 背景层
	RenderLayer.Draw(RENDERLAYER_BACKGROUND, DeltaTime);
	//渲染各类层级 不透明层
	RenderLayer.Draw(RENDERLAYER_OPAQUE, DeltaTime);
	//渲染各类层级 透明层
	RenderLayer.Draw(RENDERLAYER_TRANSPARENT, DeltaTime);


	//渲染管线状态
	DirectXPipelineState.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);

	//渲染层级调用渲染(结束)
	RenderLayer.PostDraw(DeltaTime);
	//渲染管线状态  
	DirectXPipelineState.PostDraw(DeltaTime);
}
