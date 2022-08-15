#pragma once
#include "Core/RangeLightComponent.h"

//平行光								继承自灯光范围控制组件
class CSpotLightComponent :public CRangeLightComponent
{
	typedef CRangeLightComponent Super;

public:
	CSpotLightComponent();

	float GetConicalInnerCorner() const { return ConicalInnerCorner; }//获取聚光灯内角  角度
	float GetConicalOuterCorner() const { return ConicalOuterCorner; }//获取聚光灯外角  角度

	void SetConicalInnerCorner(float InConicalInnerCorner); //设置聚光灯内角  角度
	void SetConicalOuterCorner(float InConicalOuterCorner); //设置聚光灯外角  角度
protected:
	float ConicalInnerCorner; //聚光灯内角  角度计算
	float ConicalOuterCorner; //聚光灯外角  角度计算
};