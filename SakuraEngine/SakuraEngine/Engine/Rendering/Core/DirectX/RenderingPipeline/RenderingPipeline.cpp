#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMesh, MeshData);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{	//��������
	
	//��ʼ��GPS����
	DirectXPipelineState.ResetGPSDesc();//���� ͼ��_����_״̬_����

	//������ǩ��
	RootSignature.BuildRootSignature();//������ǩ��
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());//����ǩ��������Ⱦ����״̬��

	//����Shader HLSL
	VertexShader.BuildShaders(//����������ɫ��
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"VertexShaderMain",//��ں�����
		"vs_5_0");//ʹ�õİ汾
	PixelShader.BuildShaders(			//����������ɫ��
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"PixelShaderMain", //��ں�����
		"ps_5_0");//����������ɫ��
	DirectXPipelineState.BindShader(VertexShader, PixelShader);//��Shader������Ⱦ����״̬��

	//���벼�� ������ λ����ɫ���ߵȵ���Ϣ
	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},//ָ��λ�� Ԫ������  ָ����ʽ  �����  �ڴ��е�ƫ��  ��ǰ��λ��
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//ָ����ɫ Ԫ������  ָ����ʽ  �����  �ڴ��е�ƫ��  ��ǰ��λ��
	};
	DirectXPipelineState.BindInputLayout(InputElementDesc.data(), InputElementDesc.size());////�����벼�ְ�����Ⱦ����״̬��

	//����ģ��
	GeometryMap.Build();

	//����CBV���������� ������������ͼ(constant buffer view)
	GeometryMap.BuildDescriptorHeap();

	//����������������
	GeometryMap.BuildConstantBuffer();

	//�����ӿڳ�����������ͼ
	GeometryMap.BuildViewportConstantBufferView();

	//��������
	DirectXPipelineState.Build();

}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	//Ԥ��ʼ��  ����ʼ��PSO��
	DirectXPipelineState.PreDraw(DeltaTime);
	

}

void FRenderingPipeline::Draw(float DeltaTime)
{
	//����ģ��
	GeometryMap.PreDraw(DeltaTime);
	//���ø�ǩ��
	RootSignature.PreDraw(DeltaTime);
	//��Ⱦ
	GeometryMap.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
}
