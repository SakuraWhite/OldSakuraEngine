#include "OpaqueRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FOpaqueRenderLayer::FOpaqueRenderLayer()
{
	//�趨��͸����Ⱦ�㼶
	RenderPriority = 2000;
}

void FOpaqueRenderLayer::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void FOpaqueRenderLayer::BuildShader()
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

void FOpaqueRenderLayer::BuildPSO()
{
	//����PSO����״̬����
	//����״̬					�߿�ģʽ
	DirectXPipelineState->Build(Wireframe);
	//����ģ�����ģʽ(�߿�/��ģ)
	DirectXPipelineState->SetFillMode(false);
	//����״̬				�Ҷ�ģ��
	DirectXPipelineState->Build(GrayModel);

}
