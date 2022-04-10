#pragma once//��ֹ�ظ�����
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

struct FVertex //�������ݽṹ
{
	FVertex(const XMFLOAT3 &InPos,const XMFLOAT4 &InColor);//�����Զ����BOX�Ķ���λ���붥����ɫ
	
	XMFLOAT3 Position;//����λ��  XYZ��ͨ��
	XMFLOAT4 Color;//������ɫ RGBA��ͨ��
	XMFLOAT3 Normal; //��ӷ���
};

struct FMeshRenderingData //ģ����Ⱦ����
{
	vector<FVertex> VertexData; //���涥������
	vector<uint16_t> IndexData; //ģ�͵�Index

public:
	//��ȡ��ǰ�����Size���ֽ�
	UINT GetVertexSizeInBytes() { return VertexData.size() * sizeof(FVertex); }
	//��ȡ��ǰIndex��Size���ֽ�
	UINT GetIndexSizeInBytes() { return IndexData.size() * sizeof(uint16_t); }
};