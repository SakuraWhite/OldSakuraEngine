#pragma once//��ֹ�ظ�����
#include "../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "Geometry/GeometryMap.h"
#include "RootSignature/DirectXRootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"

class GMesh;

//��¼�ṩ��Ⱦ��ص����ݵĽӿ�
class  FRenderingPipeline :public IDirectXDeviceInterfece //�̳���DirectXDeviceInterfece
{	//��Ⱦ����


public:
	FRenderingPipeline();

	//���ģ�ͽ׶�
	void BuildMesh(GMesh *InMesh,const FMeshRenderingData& MeshData);

	//����MeshManage����ѧ�ķ���
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	//��������
	void BuildPipeline();


	virtual void PreDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);//��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw
protected:
	


//��ɫ��
	FShader VertexShader;//������ɫ
	FShader PixelShader;//������ɫ

	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;//�������������

	FDirectXPipelineState DirectXPipelineState; //��Ⱦ����״̬
	FDirectXRootSignature RootSignature;//��ǩ��
	FGeometryMap GeometryMap;//ӵ�м���ģ��
};