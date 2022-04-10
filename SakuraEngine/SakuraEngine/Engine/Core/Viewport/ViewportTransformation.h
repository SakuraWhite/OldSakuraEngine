
#pragma  once
#include "../../Core/Engine.h"
#include "../../Math/EngineMath.h"


//屏幕视口变换矩阵  描述物体在屏幕空间的位置
struct FViewportTransformation
{
	FViewportTransformation(); //初始化一下值

	XMFLOAT4X4 ViewProjectionMatrix;//视图投影透视矩阵

};
