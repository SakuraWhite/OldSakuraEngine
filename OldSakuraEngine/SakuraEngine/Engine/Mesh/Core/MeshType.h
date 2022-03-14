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

struct FMeshRendingData //ģ����Ⱦ����
{
	vector<FVertex> VertexDeta; //���涥������
	vector<uint16_t> IndexDeta; //ģ�͵�Index
};