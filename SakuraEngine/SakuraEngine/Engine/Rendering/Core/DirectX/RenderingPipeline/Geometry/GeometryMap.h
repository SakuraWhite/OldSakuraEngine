#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

//ǰ������
class CMaterial;

struct  FGeometry :public IDirectXDeviceInterfece_Struct 
{
	//��Դ�ķ������ ����ģ����Ⱦ����
	friend struct FGeometryMap;


	//�ж��Ƿ��Ѿ���������Ⱦ����
	bool IsRenderingDataExistence(CMeshComponent* InKey);
	//���ģ��
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData, int InKey);
	//����ģ��
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData, int InKey);
	//ͨ����ϣѰ��ģ����Ⱦ����
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex = -1);
	//����ģ��(����)
	void Build();

	//��ȡ DescribeMeshRenderingData ����ģ����Ⱦ����
	UINT GetDrawObjectNumber() const;

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
		
};


//��¼�ṩ��Ⱦ��ص����ݵĽӿ�    
struct  FGeometryMap :public IDirectXDeviceInterfece_Struct //�̳���DirectXDeviceInterfece
{	//����mapӳ��

	
	friend class FRenderLayer;//��Ⱦ�㼶 ��Ե

	FGeometryMap();
	~FGeometryMap();//����
	

	virtual void PreDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);//��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw



	//����MeshManage����ѧ�ķ���
	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	//���²�����ɫ����Դ��ͼ
	void UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo);

	//���ģ�ͽ׶�
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData);
	//����ģ��
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData);
	//ͨ����ϣѰ��ģ����Ⱦ����
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex = -1);

	//����������ͼ
	void LoadTexture();

	//����ģ��
	void Build();

	//���������� CBV������
	void BuildDescriptorHeap();

	//����ģ�ͳ���������
	void BuildMeshConstantBuffer();

	//�������ʳ���������
	void BuildMaterialShaderResourceView();

	//�����ƹⳣ��������
	void BuildLightConstantBuffer();

	
	//��û������ǵ�ǰģ�Ͷ��������  �ýӿ�֮������޸ı仯
	UINT GetDrawObjectNumber();

	//��û������ǵ�ǰ���ʶ��������  �ýӿ�֮������޸ı仯
	UINT GetDrawMaterialObjectNumber();

	//��û������ǵ�ǰ�ƹ���������  �ýӿ�֮������޸ı仯
	UINT GetDrawLightObjectNumber();

	//��ȡ������ͼ���������
	UINT GetDrawTextureResourcesNumber();

	//������ͼSRV��ͼ  ������������
	void BuildTextureConstantBuffer();


	//�����ӿڳ�����������ͼ
	void BuildViewportConstantBufferView();

public:
	//���Ƶƹ�
	void DrawLight(float DeltaTime);
	//��Ⱦ�ӿ�
	void DrawViewport(float DeltaTime);
	//��Ⱦģ��Mesh
	void DrawMesh(float DeltaTime);
	//��Ⱦ���Ʋ���
	void DrawMaterial(float DeltaTime);
	//��Ⱦ������ͼ(��ʱ)
	void DrawTexture(float DeltaTime);
public:
	//��ȡ�����������ӿ������ѵĽӿ�
	ID3D12DescriptorHeap* GetHeap()const { return DescriptorHeap.GetHeap(); }
protected:

	map<int, FGeometry> Geometrys;

	FDirectXDescriptorHeap DescriptorHeap;//������

	FConstantBufferViews MeshConstantBufferViews;//ģ�ͳ���������
	FConstantBufferViews MaterialConstantBufferViews;//���ʳ���������
	FConstantBufferViews ViewportConstantBufferViews;//�ӿڳ���������
	FConstantBufferViews LightConstantBufferViews;//�ƹⳣ��������
	
	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingTextureResources;//������ͼ����������������ָ�룩
	std::vector<CMaterial*> Materials;//��װ���ʶ�̬�����˳����������

};