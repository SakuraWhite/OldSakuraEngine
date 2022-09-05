#include "DirectXPipelineState.h"
#include "../../../../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
{
	//��ʼ������״̬  ��ʼ��Ϊʵ���ģ״̬
	PipelineState = EPipelineState::GrayModel;

	//�������״̬����								 4�Ǽ������������ �л�Ϊ�߿���ʾ
	PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(4, ComPtr<ID3D12PipelineState>()));//�߿�

	//�������״̬����								 4�Ǽ������������ �л�Ϊָ��ʵ����Ⱦģʽ
	PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(5, ComPtr<ID3D12PipelineState>()));//Shader 
}

void FDirectXPipelineState::PreDraw(float DeltaTime)
{
	//���ֵ�ǰ����״̬������ʲô��ʽ����ʾ							��ȡ��ͬ�Ĺ���״̬ö��
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO[(int)PipelineState].Get());//��PSO���г�ʼ��
}

void FDirectXPipelineState::Draw(float DeltaTime)
{
	//���̰�������
	CaptureKeyboardKeys();

}

void FDirectXPipelineState::PostDraw(float DeltaTime)
{
}

void FDirectXPipelineState::ResetGPSDesc()
{
	//���� ͼ��_����_״̬_����
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
}

void FDirectXPipelineState::BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize)
{
	//�����벼��	
	GPSDesc.InputLayout.pInputElementDescs = InInputElementDescs; //����Ԫ������
	GPSDesc.InputLayout.NumElements = InSize;//����size
}

void FDirectXPipelineState::BindRootSignature(ID3D12RootSignature* InRootSignature)
{
	//�󶨸�ǩ��
	GPSDesc.pRootSignature = InRootSignature;

}

void FDirectXPipelineState::BindShader(const FShader& InVertexShader, const FShader& InPixelShader)
{
	//�󶨶�����ɫ������
	GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(InVertexShader.GetBufferPointer());
	GPSDesc.VS.BytecodeLength = InVertexShader.GetBufferSize();
	//��������ɫ������
	GPSDesc.PS.pShaderBytecode = InPixelShader.GetBufferPointer();
	GPSDesc.PS.BytecodeLength = InPixelShader.GetBufferSize();
}

void FDirectXPipelineState::BuildParam()
{

	//���ù�դ��״̬
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//Ĭ����ʵ��ģʽ��ʾ

	//0000..0000
	GPSDesc.SampleMask = UINT_MAX;//���״̬�µ�ʾ���ɰ� ��Ϊ���ز�������ǿ��Բ���32λ������ ����UINT_MAX�������е�����

	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//ͼԪ���������ͣ� �������������εķ�ʽ�γ���
	GPSDesc.NumRenderTargets = 1;//ָ����ȾĿ��

	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);//ָ�����״̬
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//���ģ��

	GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();//�ж϶��ز���������
	GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();//�ж϶��ز���������

	//RTV �� DSV��ʽ
	GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat(); //����RTV�ĸ�ʽ �����ʽ�󻺳���
	GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();//����DSV  ���ģ��
}

void FDirectXPipelineState::Build(int InPSOType)
{
	if (PSO.find(InPSOType) == PSO.end()) //�жϹ���״̬�������� 
	{
		//���δ�ҵ����ݣ������һ������״̬���� ��֤��ֵ
		//�������״̬����										�л�Ϊָ��ʵ����Ⱦģʽ
		PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(InPSOType, ComPtr<ID3D12PipelineState>()));//Shader 
	}


	//�߿�ģʽע��	  ��ȡD3D����														ָ���߿�ģʽ
	ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[InPSOType])))//��ȡGraph����״̬

	
	////ָ��shader��Ⱦģʽ  �ٴδ���ͼԪ��������
	//GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//��ʵ����Ⱦ��ʽ��ʾ
	////ʵ����Ⱦģʽע��   ��ȡD3D����														ָ��ʵ����Ⱦģʽ
	//ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[GrayModel])))//��ȡGraph����״̬

}

void FDirectXPipelineState::ResetPSO(int InPSOType)
{
	//ͨ����ȡͼ�������б� ���ùܵ�״̬(����)
	GetGraphicsCommandList()->SetPipelineState(PSO[InPSOType].Get());
}

void FDirectXPipelineState::ResetPSO()
{
	//ֱ��ʹ���Լ����õĹ���״̬
	ResetPSO(PipelineState);
}

void FDirectXPipelineState::SetFillMode(bool bWireframe)
{
	//�ж���Ⱦ״̬�Ƿ�Ϊ�߿�ģʽ
	GPSDesc.RasterizerState.FillMode = bWireframe ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
}

void FDirectXPipelineState::SetRenderTarget(int Index, const D3D12_RENDER_TARGET_BLEND_DESC& InRenderTargetBlend)
{
	//������ȾĿ��
	GPSDesc.BlendState.RenderTarget[Index] = InRenderTargetBlend;
}

void FDirectXPipelineState::SetRasterizerState(const CD3DX12_RASTERIZER_DESC& InRasterizerDesc)
{
	//���ù�դ��״̬
	GPSDesc.RasterizerState = InRasterizerDesc;
}

void FDirectXPipelineState::SetDepthStencilState(const CD3DX12_DEPTH_STENCIL_DESC& InDepthStencilDesc)
{
	//�������ģ�����״̬
	GPSDesc.DepthStencilState = InDepthStencilDesc;
}

void FDirectXPipelineState::CaptureKeyboardKeys()
{
	//���̲���
	if (GetAsyncKeyState('4') & 0x8000)//�жϼ����Ƿ����������֡�4��
	{
		PipelineState = EPipelineState::Wireframe;//���ǵĹ���״̬����Ϊ�߿�
	}
	else if (GetAsyncKeyState('5') & 0x8000)//�жϼ����Ƿ����������֡�5��
	{
		PipelineState = EPipelineState::GrayModel;//���ǵĹ���״̬����Ϊʵ����Ⱦ
	}

}
