#pragma once
#include "../../EngineMinimal.h"

struct FFogConstantBuffer //雾的常量缓冲区 结构体
{
	FFogConstantBuffer();

	XMFLOAT3 FogColor;//雾的颜色
	float FogStart;//雾的起始点

	float FogRange;//雾的范围半径
	float FogHeight;//雾的高度
	float FogTransparentCoefficient;//雾透明系数
	float xx2;
};