#include "AlphaTestRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
	//�趨Alpha��Ⱦ�㼶
	RenderPriority = 2450;
}

void FAlphaTestRenderLayer::BuildShader()
{
	//����Shader HLSL

	vector<ShaderType::FShaderMacro> ShaderMacro;//��ȡ�Լ�д��Shader��
	BuildShaderMacro(ShaderMacro);//������ʵ��

	vector<D3D_SHADER_MACRO> D3DShaderMacro;//shader��ʵ�� D3D�ĺ�
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);//ת��ΪD3D��shader��

	VertexShader.BuildShaders(//����������ɫ��
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"VertexShaderMain",//��ں�����
		"vs_5_1",//ʹ�õİ汾
		D3DShaderMacro.data());//ָ������
	PixelShader.BuildShaders(			//����������ɫ��
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"PixelShaderMain", //��ں�����
		"ps_5_1",//����������ɫ��
		D3DShaderMacro.data());//ָ������
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

void FAlphaTestRenderLayer::BuildPSO()
{
	Super::BuildPSO();//���� ִ�й�������״̬������
	//����PSO����״̬����
	DirectXPipelineState->Build(AlphaTest);//Alpha����ģʽ

}

void FAlphaTestRenderLayer::Draw(float DeltaTime)
{

	DirectXPipelineState->ResetPSO(AlphaTest);//���ù���״̬����PSO  ����ΪAlpha����ģʽ
	Super::Draw(DeltaTime);//��Ⱦ����
}