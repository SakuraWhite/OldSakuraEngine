//��ѧ��
#pragma once//��ֹ�ظ�����

#include "../EngineMinimal.h"
#include <DirectXMath.h>//DX��ѧ��

namespace EngineMath
{
	//��ѧ����  ��ѧ����� 
	 XMFLOAT4X4 IdentityMatrix4x4();//��λ��4x4����  

	 XMFLOAT3 ToFloat3(const fvector_3d& InV3d);
}

