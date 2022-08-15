#pragma once//��ֹ�ظ�����
#include "../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "Geometry/GeometryMap.h"
#include "RootSignature/DirectXRootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "RenderLayer/RenderLayerManage.h"

class CMeshComponent;

//��¼�ṩ��Ⱦ��ص����ݵĽӿ�
class  FRenderingPipeline :public IDirectXDeviceInterfece //�̳���DirectXDeviceInterfece
{	//��Ⱦ����


public:
	FRenderingPipeline();

	//���ģ�ͽ׶�
	void BuildMesh(const size_t InMeshHash, CMeshComponent *InMesh,const FMeshRenderingData& MeshData);
	//����ģ��
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData);
	//ͨ����ϣѰ��ģ����Ⱦ����
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex = -1);

	//����MeshManage����ѧ�ķ���
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	//��������
	void BuildPipeline();


	virtual void PreDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);//��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw
protected:

	//��ɫ��

	FRenderLayerManage RenderLayer;//��Ⱦ�㼶

	FDirectXPipelineState DirectXPipelineState; //��Ⱦ����״̬
	FDirectXRootSignature RootSignature;//��ǩ��
	FGeometryMap GeometryMap;//ӵ�м���ģ��
};