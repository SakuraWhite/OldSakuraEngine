
#pragma once
#include "MeshType.h"

struct FObjectTransformation//描述在模型世界中的位置  是个矩阵 这里暂时弄个函数
{
	FObjectTransformation(); //初始化一下值

	XMFLOAT4X4 World;//世界位置变换

	XMFLOAT4X4 TextureTransformation;//添加纹理变换
	
	UINT MaterialIndex;//材质索引
	UINT VV0;//占位符
	UINT VV1;
	UINT VV2;
};
