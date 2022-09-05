//组件 主要提供接口
#pragma once//防止重复包含
#include "../Core/Component.h"

class CFogComponent :public CComponent
{
public:
	CFogComponent();

	void SetFogColor(const fvector_color& InNewColor);//设置雾的颜色
	void SetFogStart(float InNewFogStart);//设置雾的起始点
	void SetFogRange(float InNewFogRange);//设置雾的范围
	void SetFogHeight(float InNewFogHeight);//设置雾的高度
	void SetFogTransparentCoefficient(float InNewFogTransparentCoefficient);//设置雾的透明系数
	void SetDirty(float InNewDirty);//设置判断
public:

	FORCEINLINE fvector_color GetFogColor()const { return FogColor; }//获取雾的颜色
	FORCEINLINE float GetFogStart()const { return FogStart; }//获取雾的起始点
	FORCEINLINE float GetFogRange()const { return FogRange; }//获取雾的范围
	FORCEINLINE float GetFogHeight()const { return FogHeight; }//获取雾的高度
	FORCEINLINE float GetFogTransparentCoefficient()const { return FogTransparentCoefficient; } //获取雾的透明系数
	FORCEINLINE bool IsDirty()const { return bDirty; }//获得判断雾是否被更改过
protected:
	fvector_color FogColor;//雾的颜色

	float FogStart;//雾的起始点
	float FogRange;//当前雾的范围(从起始点到结束点的半径)
	float FogHeight; //雾的高度
	float FogTransparentCoefficient;//雾的透明系数


	bool bDirty;//判断雾是否被更改过 如果被更新过则通知常量缓冲区更新

};