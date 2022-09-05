#include "DirectXPipelineState.h"
#include "../../../../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
{
	//初始化管线状态  初始化为实体灰模状态
	PipelineState = EPipelineState::GrayModel;

	//插入管线状态对象								 4是键盘输入的数字 切换为线框显示
	PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(4, ComPtr<ID3D12PipelineState>()));//线框

	//插入管线状态对象								 4是键盘输入的数字 切换为指定实体渲染模式
	PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(5, ComPtr<ID3D12PipelineState>()));//Shader 
}

void FDirectXPipelineState::PreDraw(float DeltaTime)
{
	//区分当前管线状态对象按照什么方式来显示							读取不同的管线状态枚举
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO[(int)PipelineState].Get());//对PSO进行初始化
}

void FDirectXPipelineState::Draw(float DeltaTime)
{
	//键盘案件捕获
	CaptureKeyboardKeys();

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

void FDirectXPipelineState::BuildParam()
{

	//配置光栅化状态
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//默认以实体模式显示

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
}

void FDirectXPipelineState::Build(int InPSOType)
{
	if (PSO.find(InPSOType) == PSO.end()) //判断管线状态对象类型 
	{
		//如果未找到内容，则插入一个管线状态对象 保证有值
		//插入管线状态对象										切换为指定实体渲染模式
		PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(InPSOType, ComPtr<ID3D12PipelineState>()));//Shader 
	}


	//线框模式注册	  获取D3D驱动														指定线框模式
	ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[InPSOType])))//获取Graph管线状态

	
	////指定shader渲染模式  再次创建图元拓扑类型
	//GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//以实体渲染方式显示
	////实体渲染模式注册   获取D3D驱动														指定实体渲染模式
	//ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[GrayModel])))//获取Graph管线状态

}

void FDirectXPipelineState::ResetPSO(int InPSOType)
{
	//通过获取图形命令列表 设置管道状态(类型)
	GetGraphicsCommandList()->SetPipelineState(PSO[InPSOType].Get());
}

void FDirectXPipelineState::ResetPSO()
{
	//直接使用自己设置的管线状态
	ResetPSO(PipelineState);
}

void FDirectXPipelineState::SetFillMode(bool bWireframe)
{
	//判断渲染状态是否为线框模式
	GPSDesc.RasterizerState.FillMode = bWireframe ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
}

void FDirectXPipelineState::SetRenderTarget(int Index, const D3D12_RENDER_TARGET_BLEND_DESC& InRenderTargetBlend)
{
	//设置渲染目标
	GPSDesc.BlendState.RenderTarget[Index] = InRenderTargetBlend;
}

void FDirectXPipelineState::SetRasterizerState(const CD3DX12_RASTERIZER_DESC& InRasterizerDesc)
{
	//设置光栅化状态
	GPSDesc.RasterizerState = InRasterizerDesc;
}

void FDirectXPipelineState::SetDepthStencilState(const CD3DX12_DEPTH_STENCIL_DESC& InDepthStencilDesc)
{
	//设置深度模板测试状态
	GPSDesc.DepthStencilState = InDepthStencilDesc;
}

void FDirectXPipelineState::CaptureKeyboardKeys()
{
	//键盘捕获
	if (GetAsyncKeyState('4') & 0x8000)//判断键盘是否输入了数字“4”
	{
		PipelineState = EPipelineState::Wireframe;//我们的管线状态更改为线框
	}
	else if (GetAsyncKeyState('5') & 0x8000)//判断键盘是否输入了数字“5”
	{
		PipelineState = EPipelineState::GrayModel;//我们的管线状态更改为实体渲染
	}

}
