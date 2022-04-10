#pragma once//防止重复包含
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"


//记录提供渲染相关的内容的接口    
struct  FDirectXPipelineState :public IDirectXDeviceInterfece_Struct //继承自DirectXDeviceInterfece
{		//渲染管线状态

public:
	FDirectXPipelineState();

	void PreDraw(float DeltaTime);//在模型里使用Rendeing.h的预渲染接口 
	void Draw(float DeltaTime);//在模型里使用Rendeing.h的渲染接口  进行时
	void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw

	void ResetGPSDesc();//重置 图形_管线_状态_描述

	//绑定输入布局
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize);

	//绑定根签名
	void BindRootSignature(ID3D12RootSignature* InRootSignature);

	//绑定顶点着色器和像素着色器
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

	//创管线状态对象(pipeline state object)  PSO
	void Build();
private:
	ComPtr<ID3D12PipelineState> PSO;//管线状态对象(pipeline state object) 

	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;//描述当前的内容  图形_管线_状态_描述
};