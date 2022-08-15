#include "MaterialConstantBuffer.h"
#include "../../../Math/EngineMath.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: MaterialType(0) //�����������
	, BaseColor(0.5f, 0.5f, 0.5f, 1.f)   //���� ������ɫ��ʼ��
	, Roughness(0.2f)  //�ֲڶ�  ���Ƹ߹ⷶΧ
	, BaseColorIndex(-1)//��ɫ������ͼ��ų�ʼ�� -1����ʲôҲû��
	, NormalIndex(-1) //��ʼ����������
	, SpecularIndex(-1)//��ʼ���߹�����
	, TransformInformation(EngineMath::IdentityMatrix4x4())   //��λ�����ʱ任��Ϣ
{
}
