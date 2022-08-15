#include "ViewportTransformation.h"


FViewportTransformation::FViewportTransformation()
	:ViewportPosition(0.f, 0.f, 0.f, 1.f) //初始化摄像机位置
	,ViewProjectionMatrix(EngineMath::IdentityMatrix4x4())//单位矩阵
{

}
