#include "Fog.h"
#include "../../Component/Sky/FogComponent.h"

GFog::GFog()
{
	//创建组件实例
	FogComponent = CreateObject<CFogComponent>(new CFogComponent());
}

void GFog::SetFogColor(const fvector_color& InNewColor)
{
	//通过控制组件设置雾颜色
	FogComponent->SetFogColor(InNewColor);
}

void GFog::SetFogStart(float InNewFogStart)
{
	//通过控制组件设置雾起始点
	FogComponent->SetFogStart(InNewFogStart);

}

void GFog::SetFogRange(float InNewFogRange)
{
	//通过控制组件设置雾范围
	FogComponent->SetFogRange(InNewFogRange);

}

void GFog::SetFogHeight(float InNewFogHeight)
{
	//通过控制组件设置雾高度
	FogComponent->SetFogHeight(InNewFogHeight);
}

void GFog::SetFogTransparentCoefficient(float InNewFogTransparentCoefficient)
{
	//通过控制组件设置雾透明系数
	FogComponent->SetFogTransparentCoefficient(InNewFogTransparentCoefficient);
}
