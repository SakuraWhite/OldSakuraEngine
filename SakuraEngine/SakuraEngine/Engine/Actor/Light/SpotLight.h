#pragma  once 
#include "Core/RangeLight.h"

// 点光源控制		  继承自GLight核心对象
class GSpotLight : public GRangeLight
{
	typedef GRangeLight Super;

public:
	GSpotLight();

	virtual void Tick(float DeltaTime);//随着帧执行

	float GetConicalInnerCorner() const;//获得聚光灯内角
	float GetConicalOuterCorner() const;//获得聚光灯外角

	void SetConicalInnerCorner(float InConicalInnerCorner);//设置聚光灯内角		 
	void SetConicalOuterCorner(float InConicalOuterCorner);//设置聚光灯外角

};