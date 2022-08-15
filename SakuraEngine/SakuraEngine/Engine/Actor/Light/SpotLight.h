#pragma  once 
#include "Core/RangeLight.h"

// ���Դ����		  �̳���GLight���Ķ���
class GSpotLight : public GRangeLight
{
	typedef GRangeLight Super;

public:
	GSpotLight();

	virtual void Tick(float DeltaTime);//����ִ֡��

	float GetConicalInnerCorner() const;//��þ۹���ڽ�
	float GetConicalOuterCorner() const;//��þ۹�����

	void SetConicalInnerCorner(float InConicalInnerCorner);//���þ۹���ڽ�		 
	void SetConicalOuterCorner(float InConicalOuterCorner);//���þ۹�����

};