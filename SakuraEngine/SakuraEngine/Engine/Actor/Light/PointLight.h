#pragma  once 
#include "Core/RangeLight.h"

// ���Դ����		  �̳���GLight���Ķ���
class GPointLight : public GRangeLight
{
	typedef GRangeLight Super;

public:
	GPointLight();

	virtual void Tick(float DeltaTime);//����ִ֡��


};