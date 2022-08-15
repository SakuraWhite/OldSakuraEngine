#pragma once
#include "../../EngineMinimal.h"

//���������������ṹ
class FRenderingResourcesUpdate :public enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
	FRenderingResourcesUpdate();//���캯��
	~FRenderingResourcesUpdate();//��������

	//��ʼ����Դ					������		Ԫ�صĴ�С,		   Ԫ�ص�����		 �жϳ���������ĥ��true		
	void Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount, bool bConstBuffer = true);

	void Update(int Index, const void* InData);//��������

	//���ٻ�ȡ���ǵ�ǰ����Buffer�ֽ�Size   ���ַ���
	UINT GetConstantBufferByteSize(UINT InTypeSzie);//Ԫ�����ʹ�С 
	UINT GetConstantBufferByteSize();//Ԫ�����ʹ�С ֱ�ӿ��Ի�ȡ

	ID3D12Resource* GetBuffer() { return UploadBuffer.Get(); } //��ȡ�ϴ���Budder �������ϴ�Buffer��Ҫ��©����
private:
	ComPtr<ID3D12Resource> UploadBuffer;//�ϴ�Buffer 
	UINT ElementSize;//����Ԫ�ش�С
	BYTE* Data;//�ֽ�  ʵ������
};