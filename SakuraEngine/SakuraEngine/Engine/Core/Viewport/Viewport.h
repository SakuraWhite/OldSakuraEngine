//���ӿ�ʹ�õ�.h
#pragma once//��ֹ�ظ�����
#include "../../Core/Engine.h"

class FViewport   //���������F��ͷ��
{
public:
	FViewport();

	void ViewportInit();//��ʼ���ӿ�

	//���������λ��  UPֵ:�������������  Right:������������� Look:�����Ǹ�����  ��βü��ռ����� Ҳ��Ϊ��Ļ�ռ�����
	XMFLOAT4X4 ViewMatrix;//���Ծ���������Ⱦ�������������ǰ�滹�Ǻ���  
	
	//���ӷ�Χ ���ӽǶ�  ����ռ�����
	XMFLOAT4X4 ProjectMatrix;//����FOV 

};