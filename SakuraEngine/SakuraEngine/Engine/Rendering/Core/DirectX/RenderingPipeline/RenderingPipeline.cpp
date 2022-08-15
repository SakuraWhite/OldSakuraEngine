#include "RenderingPipeline.h"

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
	//更新几何map里 计算常量缓冲区
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
	//渲染层级调用 计算常量缓冲区
	RenderLayer.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{	//构建管线
	
	//初始化GPS描述
	DirectXPipelineState.ResetGPSDesc();//重置 图形_管线_状态_描述

	//注册渲染层级		几何				DX管线状态
	RenderLayer.Init(&GeometryMap, &DirectXPipelineState);

	//统一对渲染层级进行排序
	RenderLayer.Sort();

	//纹理资源读取 需要先读取贴图 再构建根签名
	GeometryMap.LoadTexture();

	//构建根签名						构建根签名时我们可以通过几何模型来获取纹理贴图资源数量
	RootSignature.BuildRootSignature(GeometryMap.GetDrawTextureResourcesNumber());//构建根签名
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());//将根签名绑定在渲染管线状态上

	//渲染每个层级的Shader
	RenderLayer.BuildShader();

	//构建模型
	GeometryMap.Build();

	//构建CBV常量描述堆 常量缓冲区视图(constant buffer view)
	GeometryMap.BuildDescriptorHeap();

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

	//构建管线状态参数  PSO参数 
	DirectXPipelineState.BuildParam();

	//通过渲染层级来构建管线状态 PSO
	RenderLayer.BuildPSO();

}


void FRenderingPipeline::PreDraw(float DeltaTime)
{
	//预初始化  （初始化PSO）
	DirectXPipelineState.PreDraw(DeltaTime);
	
	//渲染层级调用预渲染
	RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	//几何模型
	GeometryMap.PreDraw(DeltaTime);
	//设置根签名
	RootSignature.PreDraw(DeltaTime);

	//渲染
	GeometryMap.Draw(DeltaTime);
	//渲染层级调用渲染(进行时)
	RenderLayer.Draw(DeltaTime);

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
