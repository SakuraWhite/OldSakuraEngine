#pragma once
#include "MeshType.h"

struct FObjectTransformation//描述在模型世界中的位置  是个矩阵 这里暂时弄个函数
{
	FObjectTransformation(); //初始化一下值

	XMFLOAT4X4 World;

	//数学方法  数学计算库 
	static XMFLOAT4X4 IdentityMatrix4x4();//单位化4x4矩阵  
};
