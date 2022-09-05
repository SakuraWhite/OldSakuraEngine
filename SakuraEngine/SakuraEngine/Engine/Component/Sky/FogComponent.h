//��� ��Ҫ�ṩ�ӿ�
#pragma once//��ֹ�ظ�����
#include "../Core/Component.h"

class CFogComponent :public CComponent
{
public:
	CFogComponent();

	void SetFogColor(const fvector_color& InNewColor);//���������ɫ
	void SetFogStart(float InNewFogStart);//���������ʼ��
	void SetFogRange(float InNewFogRange);//������ķ�Χ
	void SetFogHeight(float InNewFogHeight);//������ĸ߶�
	void SetFogTransparentCoefficient(float InNewFogTransparentCoefficient);//�������͸��ϵ��
	void SetDirty(float InNewDirty);//�����ж�
public:

	FORCEINLINE fvector_color GetFogColor()const { return FogColor; }//��ȡ�����ɫ
	FORCEINLINE float GetFogStart()const { return FogStart; }//��ȡ�����ʼ��
	FORCEINLINE float GetFogRange()const { return FogRange; }//��ȡ��ķ�Χ
	FORCEINLINE float GetFogHeight()const { return FogHeight; }//��ȡ��ĸ߶�
	FORCEINLINE float GetFogTransparentCoefficient()const { return FogTransparentCoefficient; } //��ȡ���͸��ϵ��
	FORCEINLINE bool IsDirty()const { return bDirty; }//����ж����Ƿ񱻸��Ĺ�
protected:
	fvector_color FogColor;//�����ɫ

	float FogStart;//�����ʼ��
	float FogRange;//��ǰ��ķ�Χ(����ʼ�㵽������İ뾶)
	float FogHeight; //��ĸ߶�
	float FogTransparentCoefficient;//���͸��ϵ��


	bool bDirty;//�ж����Ƿ񱻸��Ĺ� ��������¹���֪ͨ��������������

};