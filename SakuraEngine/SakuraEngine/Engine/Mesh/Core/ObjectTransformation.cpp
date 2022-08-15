#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
	:World(EngineMath::IdentityMatrix4x4())//单位矩阵
	, TextureTransformation(EngineMath::IdentityMatrix4x4())//单位化贴图变换
	, MaterialIndex(0)//初始化材质索引
	, VV0(0)
	, VV1(0)
	, VV2(0)
{

}
