#include "FogComponent.h"

CFogComponent::CFogComponent()
	:FogColor(1.f, 1.f, 1.f, 1.f)//初始化雾颜色
	, FogStart(5.f) //初始化雾起始点
	, FogRange(100.f)//初始化雾范围半径
	, FogHeight(9000.f)//初始化雾高度
	, FogTransparentCoefficient(0.f)//初始化透明系数
	, bDirty(false)//初始化判断雾是否被更改过
{
}

void CFogComponent::SetFogColor(const fvector_color& InNewColor)
{
	//输入雾颜色
	FogColor = InNewColor;

	SetDirty(true);//判断是否更改过
}

void CFogComponent::SetFogStart(float InNewFogStart)
{
	//输入雾起始点
	FogStart = InNewFogStart;

	SetDirty(true);
}

void CFogComponent::SetFogRange(float InNewFogRange)
{
	//输入雾范围
	FogRange = InNewFogRange;

	SetDirty(true);
}

void CFogComponent::SetFogHeight(float InNewFogHeight)
{
	//输入雾的高度
	FogHeight = InNewFogHeight;

	SetDirty(true);
}

void CFogComponent::SetFogTransparentCoefficient(float InNewFogTransparentCoefficient)
{
	//输入雾的透明系数
	FogTransparentCoefficient = InNewFogTransparentCoefficient;
	//脏
	SetDirty(true);
}

void CFogComponent::SetDirty(float InNewDirty)
{
	//输入判断
	bDirty = InNewDirty;
}
