#pragma once
#include "MeshType.h"

struct FObjectTransformation//������ģ�������е�λ��  �Ǹ����� ������ʱŪ������
{
	FObjectTransformation(); //��ʼ��һ��ֵ

	XMFLOAT4X4 World;

	//��ѧ����  ��ѧ����� 
	static XMFLOAT4X4 IdentityMatrix4x4();//��λ��4x4����  
};
