#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMesh, MeshData);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{	//构建管线
	
	//初始化GPS描述
	DirectXPipelineState.ResetGPSDesc();//重置 图形_管线_状态_描述

	//构建根签名
	RootSignature.BuildRootSignature();//构建根签名
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());//将根签名绑定在渲染管线状态上

	//构建Shader HLSL
	VertexShader.BuildShaders(//构建顶点着色器
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//寻找外部的hlsl着色器语言文件
		"VertexShaderMain",//入口函数名
		"vs_5_0");//使用的版本
	PixelShader.BuildShaders(			//构建像素着色器
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//寻找外部的hlsl着色器语言文件
		"PixelShaderMain", //入口函数名
		"ps_5_0");//构建像素着色器
	DirectXPipelineState.BindShader(VertexShader, PixelShader);//将Shader绑定在渲染管线状态上

	//输入布局 构建层 位置颜色法线等等信息
	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},//指定位置 元素语义  指定格式  插入槽  内存中的偏移  当前的位置
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//指定颜色 元素语义  指定格式  插入槽  内存中的偏移  当前的位置
	};
	DirectXPipelineState.BindInputLayout(InputElementDesc.data(), InputElementDesc.size());////将输入布局绑定在渲染管线状态上

	//构建模型
	GeometryMap.Build();

	//构建CBV常量描述堆 常量缓冲区视图(constant buffer view)
	GeometryMap.BuildDescriptorHeap();

	//构建对象常量缓冲区
	GeometryMap.BuildConstantBuffer();

	//构建视口常量缓冲区视图
	GeometryMap.BuildViewportConstantBufferView();

	//构建管线
	DirectXPipelineState.Build();

}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	//预初始化  （初始化PSO）
	DirectXPipelineState.PreDraw(DeltaTime);
	

}

void FRenderingPipeline::Draw(float DeltaTime)
{
	//几何模型
	GeometryMap.PreDraw(DeltaTime);
	//设置根签名
	RootSignature.PreDraw(DeltaTime);
	//渲染
	GeometryMap.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
}
