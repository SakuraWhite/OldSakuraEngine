#pragma once
#include "../Core/ActorObject.h"

class CFogComponent;

//  �����		  �̳���GActorObject���Ķ���
class GFog : public GActorObject
{
	typedef GActorObject Super;

	CVARIABLE()  //�����
	CFogComponent* FogComponent;//������������� 


public:
	GFog();

	void SetFogColor(const fvector_color& InNewColor);//���������ɫ
	void SetFogStart(float InNewFogStart);//���������ʼ��
	void SetFogRange(float InNewFogRange);//������ķ�Χ
	void SetFogHeight(float InNewFogHeight);//������ĸ߶�
	void SetFogTransparentCoefficient(float InNewFogTransparentCoefficient);//�������͸��ϵ��
};