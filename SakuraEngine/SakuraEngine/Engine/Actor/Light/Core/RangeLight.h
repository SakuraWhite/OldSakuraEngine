#pragma  once 
#include "Light.h"


// ���Դ����		  �̳���GActorObject���Ķ���
class GRangeLight : public GLight
{
	typedef GLight Super;

public:
	//��ȡ���Դ˥����ʼ��
	float GetStartAttenuation() const;
	//��ȡ���Դ˥���յ�
	float GetEndAttenuation() const;

public:
	//���õ��Դ˥����ʼ��
	void SetStartAttenuation(float InNewStartAttenuation);
	//���õ��Դ˥���յ�
	void SetEndAttenuation(float InNewEndAttenuation);
};