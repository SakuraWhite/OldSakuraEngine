#include "ViewportTransformation.h"


FViewportTransformation::FViewportTransformation()
	:ViewportPosition(0.f, 0.f, 0.f, 1.f) //��ʼ�������λ��
	,ViewProjectionMatrix(EngineMath::IdentityMatrix4x4())//��λ����
{

}
