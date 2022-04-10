#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

struct  FGeometry :public IDirectXDeviceInterfece_Struct 
{
	//��Դ�ķ������ ����ģ����Ⱦ����
	friend struct FGeometryMap;


	//�ж��Ƿ��Ѿ���������Ⱦ����
	bool bRenderingDataExistence(GMesh* InKey);
	//���ģ��
	void BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData);

	//����ģ��(����)
	void Build();

	//��ȡ DescribeMeshRenderingData ����ģ����Ⱦ����
	UINT GetDrawObjectNumber() const { return DescribeMeshRenderingData.size(); }

	//��ȡ����Buffer��ͼ VBV
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	//��ȡIndexBuffer��ͼ IBV
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;//CPU���㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;//CPU��Index������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;//GPU���㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;//GPU��Index������

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;	//��ʱ�Ķ��㻺����������CPU���ݴ���GPU
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;	//��ʱ��Index������������CPU���ݴ���GPU


	//ģ����Ⱦ����
	FMeshRenderingData MeshRenderingData;

	//DescribeMeshRenderingData ����ģ����Ⱦ����
	vector<FRenderingData> DescribeMeshRenderingData;

};


//��¼�ṩ��Ⱦ��ص����ݵĽӿ�    
struct  FGeometryMap :public IDirectXDeviceInterfece_Struct //�̳���DirectXDeviceInterfece
{	//����mapӳ��

	FGeometryMap();

	virtual void PreDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);//��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw

	//���ģ�ͽ׶�
	void BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData);

	//MeshManage����ѧ�ķ���
	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	//����ģ��
	void Build();

	//���������� CBV������
	void BuildDescriptorHeap();

	//��������������
	void BuildConstantBuffer();
	
	//�����Ⱦ��������  �ýӿ�֮������޸ı仯
	UINT GetDrawObjectNumber();

	//�����ӿڳ�����������ͼ
	void BuildViewportConstantBufferView();

public:
	//��Ⱦ�ӿ�
	void DrawViewport(float DeltaTime);
	//��Ⱦģ��Mesh
	void DrawMesh(float DeltaTime);
public:
	//��ȡ�����������ӿ������ѵĽӿ�
	ID3D12DescriptorHeap* GetHeap()const { return DescriptorHeap.GetHeap(); }
protected:

	map<int, FGeometry> Geometrys;

	FDirectXDescriptorHeap DescriptorHeap;//������

	FConstantBufferViews ObjectConstantBufferViews;//������������
	FConstantBufferViews ViewportConstantBufferViews;//�ӿڳ���������
	
};