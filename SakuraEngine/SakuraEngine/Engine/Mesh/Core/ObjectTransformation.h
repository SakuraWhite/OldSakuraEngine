
#pragma once
#include "MeshType.h"

struct FObjectTransformation//������ģ�������е�λ��  �Ǹ����� ������ʱŪ������
{
	FObjectTransformation(); //��ʼ��һ��ֵ

	XMFLOAT4X4 World;//����λ�ñ任

	XMFLOAT4X4 TextureTransformation;//�������任
	
	UINT MaterialIndex;//��������
	UINT VV0;//ռλ��
	UINT VV1;
	UINT VV2;
};
