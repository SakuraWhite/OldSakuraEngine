#pragma once//��ֹ�ظ�����
#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"
#include "../Core/DynamicMap.h"

//ǰ������
class GClientViewport;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;


//��̬������ͼ
struct  FDynamicCubeMap :public FDynamicMap
{
	struct FTmpViewportCapture  //Tmp�ӿڲ���ṹ��
	{
		FTmpViewportCapture() {}  //����
		FTmpViewportCapture(const fvector_3d& InCenterPoint); //����

		fvector_3d TargetPoint[6]; //�ӿڲ���(Ŀ��) ����6��
		fvector_3d UP[6];

		//�����ӿڲ���
		void BuildViewportCapture(const fvector_3d& InCenterPoint);
	};

public:
	FDynamicCubeMap();
	//���¼���     ����cubemap���         
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	//ע���ʼ��						���뼸��map							 ������߶���״̬									������Ⱦ�㼶
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer);
	//Ԥ����
	virtual void PreDraw(float DeltaTime);
	//���� ʵ����Ⱦ
	virtual void Draw(float DeltaTime);


public:
	//����cubeMap�ӿ�λ��
	void SetCubeMapViewportPosition(const fvector_3d& InPosition);
	//�Ƿ��˳���̬��������
	bool IsExitDynamicReflectionMesh();

public:
	//�����ӿ�(��̬��������ͼ��6����������ӿ�)		�������ĵ� XYZ��һ��ֵ
	virtual void BuildViewport(const fvector_3d& InCenterPoint);
	//�������ģ��
	virtual void BuildDepthStencil();

	//�������ģ��������
	virtual void BuildDepthStencilDescriptor();
	//������ȾĿ��������
	virtual void BuildRenderTargetDescriptor();

protected:
	virtual void BuildRenderTargetRTV();//������ȾĿ����Ⱦ״̬�ӿ�
	virtual void BuildRenderTargetSRV();//������ȾĿ��shader��ɫ����Դ��ͼ
protected:
	std::unique_ptr<FCubeMapRenderTarget> RenderTarget;//��ȾĿ��(��������ͼ)
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDes;//���ģ������(��������ͼ CPU)

	std::vector<GClientViewport*> CubeMapViewport;//6���ӿ�(������������ͼ)

	FGeometryMap* GeometryMap;//����Map
	FDirectXPipelineState* DirectXPipelineState;//���߶��� ���ڰ�

	FRenderLayerManage* RenderLayer;//��Ⱦ�㼶(��)

	ComPtr<ID3D12Resource> DepthStencilBuffer;//���ģ�建����
	UINT Width; //��ͼ�Ŀ��
	UINT Height;//��ͼ�ĸ߶�
};