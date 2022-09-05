#include "BackgroundRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FBackgroundRenderLayer::FBackgroundRenderLayer()
{
	//�趨������Ⱦ�㼶
	RenderPriority = 1000;
}

void FBackgroundRenderLayer::BuildShader()
{
	//����Shader HLSL

	
	vector<ShaderType::FShaderMacro> ShaderMacro;//��ȡ�Լ�д��Shader��
	BuildShaderMacro (ShaderMacro);//������ʵ��

	vector<D3D_SHADER_MACRO> D3DShaderMacro;//shader��ʵ�� D3D�ĺ�
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);//ת��ΪD3D��shader��
	

	VertexShader.BuildShaders(//����������ɫ��
		L"../SakuraEngine/Shader/Sky.hlsl",//Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"VertexShaderMain",//��ں�����
		"vs_5_1",//ʹ�õİ汾
		D3DShaderMacro.data());//ָ������  D3D��Shader������
	PixelShader.BuildShaders(			//����������ɫ��
		L"../SakuraEngine/Shader/Sky.hlsl",//Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"PixelShaderMain", //��ں�����
		"ps_5_1",//����������ɫ�� 
		D3DShaderMacro.data());//ָ������ D3D��Shader������
	DirectXPipelineState->BindShader(VertexShader, PixelShader);//��Shader������Ⱦ����״̬��

	//���벼�� ������ λ����ɫ���ߵȵ���Ϣ
	InputElementDesc =
	{
		//����λ����Ϣ
		//ָ��λ�� Ԫ������  ָ����ʽռ12���ֽ�    �����  �ڴ��е�ƫ��		 �������			��ǰ��λ��
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,  0 , 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},

		//����������Ϣ
		//ָ������ Ԫ������  ָ����ʽ				����� �ڴ��е�ƫ��	 �������					��ǰ��λ��
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

		//����UV����
		//ָ��UV����		ָ��R32λG32λ��ʽ			�ڴ���ƫ��24�ֽ�	
		{"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT, 0 , 24,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());////�����벼�ְ�����Ⱦ����״̬��

}

void FBackgroundRenderLayer::BuildPSO()
{
	Super::BuildPSO();//���� ִ�й�������״̬������

	//���ù�դ��״̬
	CD3DX12_RASTERIZER_DESC RasterizerDesc(D3D12_DEFAULT); //��դ������
	RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID; //Ĭ����ʵ��ģʽ��ʾ
	RasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;  //���òü� �������Ҫ�ü�
	DirectXPipelineState->SetRasterizerState(RasterizerDesc);

	//�������ģ�����״̬
	CD3DX12_DEPTH_STENCIL_DESC DepthStencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//���ģ������
	DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //�ü�ģʽΪС�ڵ��ڣ� ��ֹ��Ȳ�����ʧ��
	DirectXPipelineState->SetDepthStencilState(DepthStencilDesc);

	//����PSO����״̬����
	DirectXPipelineState->Build(Background);//����ģʽ

}

void FBackgroundRenderLayer::Draw(float DeltaTime)
{

	DirectXPipelineState->ResetPSO(Background);//���ù���״̬����PSO  ����Ϊ����ģʽ
	Super::Draw(DeltaTime);//��Ⱦ����
}



