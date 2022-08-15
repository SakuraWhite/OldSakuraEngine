#pragma once
#include "LightComponent.h"

/// 范围光控制组件					继承自灯光控制组件
class CRangeLightComponent :public CLightComponent
{
	typedef CLightComponent Super;

public:
	CRangeLightComponent();

public:
	//设置点光源的衰减起点
	void SetStartAttenuation(float InNewStartAttenuation) { StartAttenuation = InNewStartAttenuation; }
	//设置点光源的衰减终点
	void SetEndAttenuation(float InNewEndAttenuation) { EndAttenuation = InNewEndAttenuation; }
public:
	//获得点光源的衰减起点
	float GetStartAttenuation() const { return StartAttenuation; }
	//获得点光源的衰减终点
	float GetEndAttenuation() const { return EndAttenuation; }

protected:
	//点光源的衰减起点
	float StartAttenuation;
	//点光源的衰减终点
	float EndAttenuation;
};