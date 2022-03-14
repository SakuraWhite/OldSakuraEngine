#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"

class FRenderingResourcesUpdate;

class CMeshManage :public CCoreMinimalObject, public IRenderingInterface
{
public:
	CMeshManage();//��ʼ��Index��Vertex��һЩ����������

	virtual void Init(); //Rendeing.h��ĵ���Ⱦ��ʼ��

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);//��������Mesh�ӿ�

	virtual void PreDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);//��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();//��ȡ����Buffer��ͼ
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();//��ȡIndexBuffer��ͼ
public:
	CMesh* CreateBoxMesh(
		float InHeight,			//BOX�߶�
		float InWidth,			//BOX���
		float InDepth);			//BOX���

	CMesh* CreateConeMesh(
		float InRadius,						//Բ׶��ײ��뾶  ��ΪԲ׶�嶥���ǵ������� ������ֻ�еײ��а뾶
		float InHeight,						//Բ׶��߶�
		uint32_t InAxialSubdivision,		//��ϸ��
		uint32_t InHeightSubdivision);		//�߶�ϸ��

	CMesh* CreateCylinderMesh(
		float InTopRadius,					//Բ�������뾶
		float InBottomRadius,				//Բ���ײ��뾶
		float InHeight,						//Բ����ĸ߶�
		uint32_t InAxialSubdivision,		//���������ϸ��
		uint32_t InHeightSubdivision);		//����ĸ߶�ϸ��

	CMesh* CreatePlaneMesh(
		float InHeight,						//ƽ��߶�
		float InWidth,						//ƽ����
		uint32_t InHeightSubdivide,			//ƽ��ĸ߶�ϸ��
		uint32_t InWidthSubdivide);			//ƽ����ϸ��

	CMesh* CreateSphereMesh(
		float InRadius,						//������ǰ뾶
		uint32_t InAxialSubdivision,		//���������ϸ��
		uint32_t InHeightSubdivision);		//����ĸ߶�ϸ��

	CMesh* CreateMesh(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;//CPU���㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;//CPU��Index������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;//GPU���㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;//GPU��Index������

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;	//��ʱ�Ķ��㻺����������CPU���ݴ���GPU
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;	//��ʱ��Index������������CPU���ݴ���GPU
	

	//��ǩ��
	ComPtr<ID3D12RootSignature>  RootSignature;
	//����ջ
	ComPtr<ID3D12DescriptorHeap> CBVHeap;
	//��������ָ��
	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;//���������������Ķ�����Ψһ�� ���ﴴ����һ��������
	

	ComPtr<ID3D12PipelineState> PSO;//Psoָ��
	//��ɫ��
	FShader VertexShader;//������ɫ
	FShader PixelShader;//������ɫ

	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;//�������������
protected:
	UINT VertexSizeInBytes;//���㻺�����ֽڴ�С
	UINT VertexStrideInBytes;//ÿ������ֽڴ�С

	UINT IndexSizeInBytes;//Index�ֽڴ�С
	DXGI_FORMAT IndexFormat;//Index��ʽ
	UINT IndexSize;//���ڱ���һ�ݶ�������

	//��ʱ���ӿ����  ����ռ����� ��βü��ռ����� ����ռ�����
	XMFLOAT4X4 WorldMatrix;//��Ϊ����ռ�����
	XMFLOAT4X4 ViewMatrix;//��βü��ռ����� Ҳ��Ϊ��Ļ�ռ�����
	XMFLOAT4X4 ProjectMatrix;//����ռ�����
};
