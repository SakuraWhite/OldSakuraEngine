#pragma once
#include "Core/RangeLightComponent.h"

//ƽ�й�								�̳��ԵƹⷶΧ�������
class CSpotLightComponent :public CRangeLightComponent
{
	typedef CRangeLightComponent Super;

public:
	CSpotLightComponent();

	float GetConicalInnerCorner() const { return ConicalInnerCorner; }//��ȡ�۹���ڽ�  �Ƕ�
	float GetConicalOuterCorner() const { return ConicalOuterCorner; }//��ȡ�۹�����  �Ƕ�

	void SetConicalInnerCorner(float InConicalInnerCorner); //���þ۹���ڽ�  �Ƕ�
	void SetConicalOuterCorner(float InConicalOuterCorner); //���þ۹�����  �Ƕ�
protected:
	float ConicalInnerCorner; //�۹���ڽ�  �Ƕȼ���
	float ConicalOuterCorner; //�۹�����  �Ƕȼ���
};