#pragma  once 
#include "Core/Light.h"
#include "../../Component/Light/ParallelLightComponent.h"

// ƽ�й����		  �̳���GLight���Ķ���
class GParallelLight : public GLight
{
	typedef GLight super;


public:
	GParallelLight();

	virtual void Tick(float DeltaTime);//����ִ֡��
};