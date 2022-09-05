#pragma once//防止重复包含
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"
#include "../RenderingPipelineType.h"

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

	//构建PSO参数
	void BuildParam();

	//创管线状态对象(pipeline state object)  PSO
	void Build(int InPSOType);

	//绘制的时候调用  重置管线状态对象(类型)
	void ResetPSO(int InPSOType);

	//重置管线状态对象(类型) 空值 只提供给不透明层级使用
	void ResetPSO();
public:
	//设置填充模式  判断是否为线框模式
	void SetFillMode(bool bWireframe);

	//设置渲染目标		渲染层级序号				渲染目标混合系数
	void SetRenderTarget(int Index, const D3D12_RENDER_TARGET_BLEND_DESC& InRenderTargetBlend);

	//设置光栅化状态								输入光栅化渲染器描述
	void SetRasterizerState(const CD3DX12_RASTERIZER_DESC& InRasterizerDesc);
	//设置深度模板测试状态					输入深度模板描述
	void SetDepthStencilState(const CD3DX12_DEPTH_STENCIL_DESC& InDepthStencilDesc);
private:
	//捕获按键 来更改图形渲染状态 shader渲染、线框渲染
	void CaptureKeyboardKeys();

private:

	//哈希类型map效率高	 管线状态对象(Pipeline State Object)  
	unordered_map<int, ComPtr<ID3D12PipelineState>> PSO;//分开管线状态对象 用int来区分线框渲染还是材质渲染
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;//描述当前的内容  图形_管线_状态_描述

	//不同的管线状态 渲染、线框的选择
	EPipelineState PipelineState;
};