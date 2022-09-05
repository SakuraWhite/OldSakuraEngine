#include "RenderingPipeline.h"
#include "../../../../Component/Mesh/Core/MeshComponentType.h"

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
	//���¶�̬Cubemap ���㳣��������
	DynamicCubeMap.UpdateCalculations(DeltaTime, ViewportInfo);
	//���¼���map�� ���㳣��������
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
	//��Ⱦ�㼶���� ���㳣��������
	RenderLayer.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{	//��������
	
	//��ʼ��GPS����
	DirectXPipelineState.ResetGPSDesc();//���� ͼ��_����_״̬_����

	//��Ⱦ�㼶��ʼ��		����				DX����״̬
	RenderLayer.Init(&GeometryMap, &DirectXPipelineState);
	//ͳһ����Ⱦ�㼶��������
	RenderLayer.Sort();

	//������Դ��ȡ ��Ҫ�ȶ�ȡ��ͼ �ٹ�����ǩ��
	GeometryMap.LoadTexture();
	//��������
	GeometryMap.BuildFog();

	//������̬��CubeMap ��ʼ��
	DynamicCubeMap.Init(
		&GeometryMap,  //����map
		&DirectXPipelineState,//DX����״̬
		&RenderLayer);//��Ⱦ�㼶

	//������ǩ��						������ǩ��ʱ���ǿ���ͨ������ģ������ȡ������ͼ��Դ����
	RootSignature.BuildRootSignature(GeometryMap.GetDrawTexture2DResourcesNumber());//������ǩ��
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());//����ǩ��������Ⱦ����״̬��

	//��Ⱦÿ���㼶��Shader
	//RenderLayer.BuildShader();

	//����ģ��
	GeometryMap.Build();

	//������̬����Mesh
	GeometryMap.BuildDynamicReflectionMesh();

	//����CBV���������� ������������ͼ(constant buffer view)
	GeometryMap.BuildDescriptorHeap();


	//��ʼ����̬CubeMap ����� �����ӿ�
	DynamicCubeMap.BuildViewport(fvector_3d(1.f, 1.f, 1.f));//�����λ��Ϊ�Լ����õķ������λ�� �������� �Ѿ�����ͬ������

	//����RTVDes ��ȾĿ��������
	DynamicCubeMap.BuildRenderTargetDescriptor();

	//�������ģ������ ��̬Cubemap
	DynamicCubeMap.BuildDepthStencilDescriptor();

	//�������ģ��  ��̬Cubemap
	DynamicCubeMap.BuildDepthStencil();

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

	//������������������
	GeometryMap.BuildFogConstantBuffer();

	//��������״̬�������  PSO���� 
	//DirectXPipelineState.BuildParam();


	//ͨ����Ⱦ�㼶����������״̬ PSO
	RenderLayer.BuildPSO();

}


void FRenderingPipeline::PreDraw(float DeltaTime)
{
	//Ԥ��ʼ��  ����ʼ��PSO��
	DirectXPipelineState.PreDraw(DeltaTime);
	
	//����ģ��
	GeometryMap.PreDraw(DeltaTime);
	//���ø�ǩ��
	RootSignature.PreDraw(DeltaTime);

	//��Ⱦ�ƹ���ͼ����������
	GeometryMap.Draw(DeltaTime);//����map��Ⱦ

	//���ӿ��������
	ClearMainSwapChainCanvas();
	//Ԥ��Ⱦ��̬Cubemap
	if (DynamicCubeMap.IsExitDynamicReflectionMesh()) //�ж��Ƿ��ж�̬����
	{
		DynamicCubeMap.PreDraw(DeltaTime);//������Ⱦ��̬����Cubemap
	}

	//��Ⱦ�㼶����Ԥ��Ⱦ
	RenderLayer.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	//���ӿ� �Ӽ���map����û����ӿ�
	GeometryMap.DrawViewport(DeltaTime);


	//����CubeMap ����ԭ�ȱ��޸ĵ�CubeMap
	GeometryMap.DrawCubeMapTexture(DeltaTime);

	//��Ⱦ����㼶 ������
	RenderLayer.Draw(RENDERLAYER_BACKGROUND, DeltaTime);
	//��Ⱦ����㼶 ��͸����
	RenderLayer.Draw(RENDERLAYER_OPAQUE, DeltaTime);
	//��Ⱦ����㼶 ͸����
	RenderLayer.Draw(RENDERLAYER_TRANSPARENT, DeltaTime);


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
