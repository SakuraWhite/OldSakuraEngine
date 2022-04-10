#include "DirectXPipelineState.h"
#include "../../../../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
{

}

void FDirectXPipelineState::PreDraw(float DeltaTime)
{
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());//��PSO���г�ʼ��
}

void FDirectXPipelineState::Draw(float DeltaTime)
{
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

void FDirectXPipelineState::Build()
{

	//���ù�դ��״̬
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//���߿�ʽ��ʾ

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
	//��ȡD3D����
	ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)))//��ȡGraph����״̬
}
