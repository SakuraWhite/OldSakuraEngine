#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
	:World(EngineMath::IdentityMatrix4x4())//��λ����
	, TextureTransformation(EngineMath::IdentityMatrix4x4())//��λ����ͼ�任
	, MaterialIndex(0)//��ʼ����������
	, VV0(0)
	, VV1(0)
	, VV2(0)
{

}
