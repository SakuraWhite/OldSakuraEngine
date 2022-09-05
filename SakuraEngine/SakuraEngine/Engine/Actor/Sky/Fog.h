#pragma once
#include "../Core/ActorObject.h"

class CFogComponent;

//  雾控制		  继承自GActorObject核心对象
class GFog : public GActorObject
{
	typedef GActorObject Super;

	CVARIABLE()  //反射宏
	CFogComponent* FogComponent;//积类的雾控制组件 


public:
	GFog();

	void SetFogColor(const fvector_color& InNewColor);//设置雾的颜色
	void SetFogStart(float InNewFogStart);//设置雾的起始点
	void SetFogRange(float InNewFogRange);//设置雾的范围
	void SetFogHeight(float InNewFogHeight);//设置雾的高度
	void SetFogTransparentCoefficient(float InNewFogTransparentCoefficient);//设置雾的透明系数
};