#pragma once
#include "LightComponent.h"

/// ��Χ��������					�̳��Եƹ�������
class CRangeLightComponent :public CLightComponent
{
	typedef CLightComponent Super;

public:
	CRangeLightComponent();

public:
	//���õ��Դ��˥�����
	void SetStartAttenuation(float InNewStartAttenuation) { StartAttenuation = InNewStartAttenuation; }
	//���õ��Դ��˥���յ�
	void SetEndAttenuation(float InNewEndAttenuation) { EndAttenuation = InNewEndAttenuation; }
public:
	//��õ��Դ��˥�����
	float GetStartAttenuation() const { return StartAttenuation; }
	//��õ��Դ��˥���յ�
	float GetEndAttenuation() const { return EndAttenuation; }

protected:
	//���Դ��˥�����
	float StartAttenuation;
	//���Դ��˥���յ�
	float EndAttenuation;
};