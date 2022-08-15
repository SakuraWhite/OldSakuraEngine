
#pragma once
#include "../../../EngineMinimal.h"

//���ʳ���������
struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer(); //��ʼ��һ��ֵ

	int MaterialType; //�����������
	float Roughness;//������� �ֲڶ�
	int BaseColorIndex; //������ɫ������ͼ��ţ����ⲿ���룩
	int NormalIndex;//��������
	
	int SpecularIndex;//�߹���ͼ����
	int XX1;//ռλ
	int XX2;
	int XX3;

	XMFLOAT4 BaseColor;// ������� ������ɫ
	XMFLOAT3 SpecularColor;//����߹���ɫ
	int XX4;//ռλ
	//���ʱ任��Ϣ
	XMFLOAT4X4 TransformInformation;
};
