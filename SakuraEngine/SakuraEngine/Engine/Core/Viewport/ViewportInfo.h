
#pragma  once
#include "../../Core/Engine.h"


//������¼һЩ��Ϣ
struct FViewportInfo
{
	//�趨�����λ��
	XMFLOAT4 ViewPosition; 

	//���������λ�þ���  UPֵ:�������������  Right:������������� Look:�����Ǹ�����  ��βü��ռ����� Ҳ��Ϊ��Ļ�ռ�����
	XMFLOAT4X4 ViewMatrix;//���Ծ���������Ⱦ�������������ǰ�滹�Ǻ���  

	//���ӷ�Χ ���ӽǶ�  ����ռ�����
	XMFLOAT4X4 ProjectMatrix;//����FOV 

};