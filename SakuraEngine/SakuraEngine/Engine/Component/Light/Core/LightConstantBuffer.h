
#pragma once
#include "../../../EngineMinimal.h"

struct FLight
{
	FLight();

	XMFLOAT3 LightIntensity;//�ƹ�ǿ��
	float StartAttenuation;  //�ƹ��˥����㣨����ʹ��float�ڵ� ���ÿ���ռλ��

	XMFLOAT3 LightDirection;//�ƹⷽ��
	float EndAttenuation; //�ƹ��˥���Ľ�����

	XMFLOAT3 Position;//�ƹ�λ��
	int LightType;//�ƹ�����

	float ConicalInnerCorner; //�۹���ڽ�  ���ȼ���
	float ConicalOuterCorner; //�۹�����  ���ȼ���
	float xx1;//ռλ��
	float xx2;//ռλ��

};

//�ƹⳣ��������
struct FLightConstantBuffer
{
	FLightConstantBuffer(); //��ʼ��һ��ֵ

	FLight SceneLights[16];//����16���ƹ�Ϊһ������
};
