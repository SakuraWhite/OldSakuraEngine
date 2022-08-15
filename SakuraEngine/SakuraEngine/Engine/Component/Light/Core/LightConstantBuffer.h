
#pragma once
#include "../../../EngineMinimal.h"

struct FLight
{
	FLight();

	XMFLOAT3 LightIntensity;//灯光强度
	float StartAttenuation;  //灯光的衰减起点（这里使用float节点 正好可以占位）

	XMFLOAT3 LightDirection;//灯光方向
	float EndAttenuation; //灯光的衰减的结束点

	XMFLOAT3 Position;//灯光位置
	int LightType;//灯光类型

	float ConicalInnerCorner; //聚光灯内角  弧度计算
	float ConicalOuterCorner; //聚光灯外角  弧度计算
	float xx1;//占位符
	float xx2;//占位符

};

//灯光常量缓冲区
struct FLightConstantBuffer
{
	FLightConstantBuffer(); //初始化一下值

	FLight SceneLights[16];//设置16个灯光为一个数组
};
