#pragma once//��ֹ�ظ�����
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

struct FVertex //�������ݽṹ
{
	FVertex(const XMFLOAT3 &InPos,const XMFLOAT4 &InColor);//�����Զ����BOX�Ķ���λ���붥����ɫ
	//���			   ����λ��           ������ɫ                           ����                          UV
	FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3 &InNormal, const XMFLOAT2& InTexCoord = XMFLOAT2(0.f, 0.f));

	XMFLOAT3 Position;//����λ��  XYZ��ͨ��
	XMFLOAT4 Color;//������ɫ RGBA��ͨ��

	XMFLOAT3 Normal; //��ӷ���
	XMFLOAT3 UTangent;//�������  U��������

	XMFLOAT2 TexCoord;//�������UV����
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

//Pyramid��׷��ı���
enum EPyramidNumberSides
{
	Pyramid_3 = 3,
	Pyramid_4,
	Pyramid_5,
};