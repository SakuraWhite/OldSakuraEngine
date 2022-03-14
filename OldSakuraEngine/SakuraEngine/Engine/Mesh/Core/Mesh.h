#pragma once//��ֹ�ظ�����
#include "../../Rending/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"


struct FObjectTransformation        //������ģ�������е�λ��  �Ǹ����� ������ʱŪ������
{
	FObjectTransformation(); //��ʼ��һ��ֵ
	XMFLOAT4X4 World;
	
	//��ѧ����  ��ѧ����� 
	static XMFLOAT4X4 IdentityMatrix4x4();//��λ��4x4����              
};
//��Ҫ�̳�һ�¼̳�����C��ͷ�ĺ���                      �����ǽӿ�
class CMesh :public CCoreMinimalObject ,public IRenderingInterface
{
public:
	CMesh();//��ʼ��Index��Vertex��һЩ����������

	virtual void Init(); //Rendeing.h��ĵ���Ⱦ��ʼ��

	virtual void BuildMesh(const FMeshRendingData* InRendingData);//��������Mesh�ӿ�

	virtual void PreDraw(float DeltaTime); //��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);  //��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw

	static CMesh*CreateMesh(const FMeshRendingData *InRendingData); //��̬�ṹ�������������� ��̬��ʽ������Mesh

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();//��ȡ����Buffer��ͼ
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(); //��ȡIndexBuffer��ͼ
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr; //CPU���㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;	 //CPU��Index������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;	//GPU���㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;	//GPU��Index������

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;	//��ʱ�Ķ��㻺����������CPU���ݴ���GPU
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;	//��ʱ��Index������������CPU���ݴ���GPU

	//��ǩ��
	ComPtr<ID3D12RootSignature> RootSignature;
	//����ջ
	ComPtr<ID3D12DescriptorHeap> CBVHeap;
												
	//��������ָ��
	shared_ptr<FRenderingResourcesUpdate> objectConstants;  //���������������Ķ�����Ψһ�� ���ﴴ����һ��������

	ComPtr<ID3D12PipelineState> PSO;//Psoָ��

	//��ɫ��
	FShader VertexShader;//������ɫ
	FShader PixelShader;//������ɫ
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;//�������������

protected:
	UINT VertexSizeInBytes;//���㻺�����ֽڴ�С
	UINT VertexStrideInBytes; //ÿ������ֽڴ�С

	UINT IndexSizeInBytes;//Index�ֽڴ�С
	DXGI_FORMAT IndexFormat;//Index��ʽ
	UINT IndexSize; //���ڱ���һ�ݶ�������


	//��ʱ���ӿ����  ����ռ����� ��βü��ռ����� ����ռ�����
	XMFLOAT4X4 WorldMatrix;//��Ϊ����ռ�����
	XMFLOAT4X4 ViewMatrix; //��βü��ռ����� Ҳ��Ϊ��Ļ�ռ�����
	XMFLOAT4X4 ProjectMatrix;//����ռ�����

};