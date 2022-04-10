#include "DirectXPipelineState.h"
#include "../../../../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
{

}

void FDirectXPipelineState::PreDraw(float DeltaTime)
{
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());//对PSO进行初始化
}

void FDirectXPipelineState::Draw(float DeltaTime)
{
}

void FDirectXPipelineState::PostDraw(float DeltaTime)
{
}

void FDirectXPipelineState::ResetGPSDesc()
{
	//重置 图形_管线_状态_描述
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
}

void FDirectXPipelineState::BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize)
{
	//绑定输入布局	
	GPSDesc.InputLayout.pInputElementDescs = InInputElementDescs; //输入元素描述
	GPSDesc.InputLayout.NumElements = InSize;//输入size
}

void FDirectXPipelineState::BindRootSignature(ID3D12RootSignature* InRootSignature)
{
	//绑定根签名
	GPSDesc.pRootSignature = InRootSignature;

}

void FDirectXPipelineState::BindShader(const FShader& InVertexShader, const FShader& InPixelShader)
{
	//绑定顶点着色器代码
	GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(InVertexShader.GetBufferPointer());
	GPSDesc.VS.BytecodeLength = InVertexShader.GetBufferSize();
	//绑定像素着色器代码
	GPSDesc.PS.pShaderBytecode = InPixelShader.GetBufferPointer();
	GPSDesc.PS.BytecodeLength = InPixelShader.GetBufferSize();
}

void FDirectXPipelineState::Build()
{

	//配置光栅化状态
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//以线框方式显示

	//0000..0000
	GPSDesc.SampleMask = UINT_MAX;//混合状态下的示例蒙版 因为多重采样最多是可以采样32位样本的 这里UINT_MAX采样所有的样本

	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//图元的拓扑类型， 这里是以三角形的方式形成面
	GPSDesc.NumRenderTargets = 1;//指定渲染目标

	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);//指定混合状态
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//深度模板

	GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();//判断多重采样的数量
	GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();//判断多重采样的质量

	//RTV 和 DSV格式
	GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat(); //设置RTV的格式 纹理格式后缓冲区
	GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();//设置DSV  深度模板
	//获取D3D驱动
	ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)))//获取Graph管线状态
}
