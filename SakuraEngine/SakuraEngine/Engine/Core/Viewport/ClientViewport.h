#pragma once//��ֹ�ظ�����
#include "../../Core/Engine.h"
#include "Viewport.h"
#include "../../Actor/Core/ActorObject.h"



class GClientViewport :
	public GActorObject//�̳���GActorObject
	, public FViewport//�̳���Fviewport
{

	typedef FViewport SuperV;    //���常��
	typedef GActorObject SuperA; //���常��

public:
	GClientViewport();

	//�����������׶��	  ����FOV		��߱�			��������		Զ������
	void SetFrustum(float InYFOV, float InAspect, float InZNear, float InZFar);

	//�Ƿ�����һ�˳��ӿ�			����λ��							Ŀ��λ��						��һ�����Ϸ���  Ĭ��ֵ
	void FaceTarget(const fvector_3d& InPosition, const fvector_3d& InTargetPosition, const fvector_3d& InUP = fvector_3d(0.f, 1.f, 0.f));

	//ִ֡��
	virtual void Tick(float DeltaTime);
	//�����ӿھ���
	virtual void BuildViewMatrix(float DeltaTime);
public:
	FORCEINLINE float GetFOV() const { return YFOV; } //��ȡFOV
	FORCEINLINE float GetAspect() const { return Aspect; }//��ȡ��߱�
	FORCEINLINE float GetNear() const { return ZNear; }//��ȡ��������	
	FORCEINLINE float GetFar() const { return ZFar; }//��ȡԶ������
	FORCEINLINE bool GetDirty() const { return bDirty; }//��ȡ ��� ��

	//���ñ�� ��
	void SetDirty(bool bNewDirty) { bDirty = bNewDirty; }
private:
	//�������������
	float YFOV;   //FOV
	float Aspect; //��߱�
	float ZNear;  //��������
	float ZFar;   //Զ������


	bool bDirty;//��� ��
}; 