#include "SkyConstantBuffer.h"

FFogConstantBuffer::FFogConstantBuffer()
	:FogColor(1.f, 1.f, 1.f) //初始化雾颜色
	, FogStart(10.f) //初始化雾起始点
	, FogRange(100.f)//初始化雾范围半径
	, FogHeight(100.f)//初始化雾高度
	, FogTransparentCoefficient(0.f)//初始化雾的透明系数
{
}
