#include "TransparentRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
	//�趨͸����Ⱦ�㼶
	RenderPriority = 3000;
}

void FTransparentRenderLayer::Draw(float DeltaTime)
{
	
	DirectXPipelineState->ResetPSO(Transparent);//���ù���״̬����PSO  ����Ϊ͸��ģʽ
	Super::Draw(DeltaTime);//��Ⱦ����
}

void FTransparentRenderLayer::BuildShader()
{
	//����Shader HLSL

	char TextureNumBuff[10] = { 0 };//����һ��������ͼ����������
	D3D_SHADER_MACRO ShaderMacro[] =  //shader�� ���� ������ͼ
	{
		"TEXTURE2D_MAP_NUM", //���嵱ǰ�ĺ� ��������ͼ������
		_itoa(GeometryMap->GetDrawTextureResourcesNumber(),TextureNumBuff,10), //��������ͼ�����������ͼ����ȡ��Ȼ������ת��Ϊ�ַ���(10����) Ȼ������������� ��̬������
		NULL,NULL,   //���������β����������null
	};

	VertexShader.BuildShaders(//����������ɫ��
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"VertexShaderMain",//��ں�����
		"vs_5_1",//ʹ�õİ汾
		ShaderMacro);//ָ������
	PixelShader.BuildShaders(			//����������ɫ��
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"PixelShaderMain", //��ں�����
		"ps_5_1",//����������ɫ��
		ShaderMacro);//ָ������
	DirectXPipelineState->BindShader(VertexShader, PixelShader);//��Shader������Ⱦ����״̬��

	//���벼�� ������ λ����ɫ���ߵȵ���Ϣ
	InputElementDesc =
	{
		//����λ����Ϣ
		//ָ��λ�� Ԫ������  ָ����ʽռ12���ֽ�    �����  �ڴ��е�ƫ��		 �������			��ǰ��λ��
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,  0 , 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},

		//������ɫ��Ϣ
		//ָ����ɫ Ԫ������  ָ����ʽռ16���ֽ�	����� �ڴ��е�ƫ��	 �������					��ǰ��λ��
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

		//����������Ϣ
		//ָ������ Ԫ������  ָ����ʽ				����� �ڴ��е�ƫ��	 �������					��ǰ��λ��
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

		//����������Ϣ
		//ָ������
		{"TANGENT", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0 , 40,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

		//����UV����
		//ָ��UV����		ָ��R32λG32λ��ʽ			�ڴ���ƫ��52�ֽ�	
		{"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT, 0 , 52,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());////�����벼�ְ�����Ⱦ����״̬��

}

void FTransparentRenderLayer::BuildPSO()
{
	//��ȾĿ����״̬����
	D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc;

	RenderTargetBlendDesc.BlendEnable = true;//�������
	RenderTargetBlendDesc.LogicOpEnable = false;//�����Ͽ��� ���뽫LogicOP�ص�

	RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA; //�������  �����ǰ���ԭ����alphaͨ�� src=ԭ��������
	RenderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//�������  �����Ƿ����ԭ����alphaͨ��

	RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD; //�������  ��������Ƭ���׵�
	RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;//F(1,1,1)  �������
	RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;//F(0,0,0) �������
	RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//Alpha�Ļ�����㣬 ��������Ƭ���׵�

	RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;//�߼�������  �������õ��Ǳ���ԭ��
	RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//д�뵱ǰ������ ����д�����RGBAȫ��д��

	//������ȾĿ��							
	DirectXPipelineState->SetRenderTarget(0, RenderTargetBlendDesc);
	//����PSO����״̬����״̬
	DirectXPipelineState->Build(Transparent);//����Ϊ͸��ģʽ
}
