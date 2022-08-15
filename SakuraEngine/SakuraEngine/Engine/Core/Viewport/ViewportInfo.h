
#pragma  once
#include "../../Core/Engine.h"


//用来记录一些信息
struct FViewportInfo
{
	//设定摄像机位置
	XMFLOAT4 ViewPosition; 

	//定义摄像机位置矩阵  UP值:摄像机朝上向量  Right:摄像机朝右向量 Look:看向那个物体  齐次裁剪空间坐标 也作为屏幕空间坐标
	XMFLOAT4X4 ViewMatrix;//可以决定我们渲染的物体在摄像机前面还是后面  

	//可视范围 可视角度  物体空间坐标
	XMFLOAT4X4 ProjectMatrix;//类似FOV 

};