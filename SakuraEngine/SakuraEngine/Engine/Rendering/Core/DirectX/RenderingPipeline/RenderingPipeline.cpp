#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMeshHash, InMesh, MeshData);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData)
{
	//����ģ��
	GeometryMap.DuplicateMesh(InMesh, MeshData);
}

bool FRenderingPipeline::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	//����ͨ����ϣѰ�ҵ�ģ����Ⱦ����
	return GeometryMap.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//���¼���map�� ���㳣��������
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
	//��Ⱦ�㼶���� ���㳣��������
	RenderLayer.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{	//��������
	
	//��ʼ��GPS����
	DirectXPipelineState.ResetGPSDesc();//���� ͼ��_����_״̬_����

	//ע����Ⱦ�㼶		����				DX����״̬
	RenderLayer.Init(&GeometryMap, &DirectXPipelineState);

	//ͳһ����Ⱦ�㼶��������
	RenderLayer.Sort();

	//������Դ��ȡ ��Ҫ�ȶ�ȡ��ͼ �ٹ�����ǩ��
	GeometryMap.LoadTexture();

	//������ǩ��						������ǩ��ʱ���ǿ���ͨ������ģ������ȡ������ͼ��Դ����
	RootSignature.BuildRootSignature(GeometryMap.GetDrawTextureResourcesNumber());//������ǩ��
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());//����ǩ��������Ⱦ����״̬��

	//��Ⱦÿ���㼶��Shader
	RenderLayer.BuildShader();

	//����ģ��
	GeometryMap.Build();

	//����CBV���������� ������������ͼ(constant buffer view)
	GeometryMap.BuildDescriptorHeap();

	//����ģ�ͳ���������
	GeometryMap.BuildMeshConstantBuffer();

	//�������ʳ���������
	GeometryMap.BuildMaterialShaderResourceView();

	//�����ƹⳣ��������
	GeometryMap.BuildLightConstantBuffer();

	
	//�����ӿڳ�����������ͼ
	GeometryMap.BuildViewportConstantBufferView();

	//������ͼ����������
	GeometryMap.BuildTextureConstantBuffer();

	//��������״̬����  PSO���� 
	DirectXPipelineState.BuildParam();

	//ͨ����Ⱦ�㼶����������״̬ PSO
	RenderLayer.BuildPSO();

}


void FRenderingPipeline::PreDraw(float DeltaTime)
{
	//Ԥ��ʼ��  ����ʼ��PSO��
	DirectXPipelineState.PreDraw(DeltaTime);
	
	//��Ⱦ�㼶����Ԥ��Ⱦ
	RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	//����ģ��
	GeometryMap.PreDraw(DeltaTime);
	//���ø�ǩ��
	RootSignature.PreDraw(DeltaTime);

	//��Ⱦ
	GeometryMap.Draw(DeltaTime);
	//��Ⱦ�㼶������Ⱦ(����ʱ)
	RenderLayer.Draw(DeltaTime);

	//��Ⱦ����״̬
	DirectXPipelineState.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);

	//��Ⱦ�㼶������Ⱦ(����)
	RenderLayer.PostDraw(DeltaTime);
	//��Ⱦ����״̬  
	DirectXPipelineState.PostDraw(DeltaTime);
}
