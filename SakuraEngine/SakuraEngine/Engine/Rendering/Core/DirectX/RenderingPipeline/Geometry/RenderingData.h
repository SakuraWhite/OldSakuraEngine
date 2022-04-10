#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"


class GMesh;

//��¼�ṩ��Ⱦ��ص����ݵĽӿ�    
struct  FRenderingData :public IDirectXDeviceInterfece_Struct //�̳���DirectXDeviceInterfece
{		//����Ҫ��Ⱦ������

public:
	FRenderingData();

	UINT IndexSize;//���ڱ���һ��Index����
	UINT VertexSize;//���ڱ���һ�ݶ�������

	UINT IndexOffsetPosition;//Indexƫ�Ƶ�λ��
	UINT VertexOffsetPosition;//����ƫ�Ƶ�λ��

	UINT VertexTypeSize;//���������ַ���С
	UINT IndexTypeSize;//Index�����ַ���С

public:
	UINT GetVertexSizeInBytes() const { return VertexSize * VertexTypeSize; } //ֱ�Ӽ��㶥���ֽڴ�С
	UINT GetIndexSizeInBytes() const { return IndexSize * IndexTypeSize; }//ֱ�Ӽ���Index�ֽڴ�С
public:

	DXGI_FORMAT IndexFormat;//Index��ʽ
	
	//��ʱ���ӿ����  ����ռ����� ��βü��ռ����� ����ռ�����
	XMFLOAT4X4 WorldMatrix;//��Ϊ����ռ�����

	GMesh* Mesh;

	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;
};