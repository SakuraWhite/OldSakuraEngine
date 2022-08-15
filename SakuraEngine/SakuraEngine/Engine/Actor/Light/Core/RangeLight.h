#pragma  once 
#include "Light.h"


// 点光源控制		  继承自GActorObject核心对象
class GRangeLight : public GLight
{
	typedef GLight Super;

public:
	//获取点光源衰减起始点
	float GetStartAttenuation() const;
	//获取点光源衰减终点
	float GetEndAttenuation() const;

public:
	//设置点光源衰减起始点
	void SetStartAttenuation(float InNewStartAttenuation);
	//设置点光源衰减终点
	void SetEndAttenuation(float InNewEndAttenuation);
};