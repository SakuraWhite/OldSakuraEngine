//��������ӿڻ������  ����ָ��ʹ�õ�
#pragma once//��ֹ�ظ�����
#include "core/Component.h"

class CTransformationComponent :public CComponent
{

public:
	CTransformationComponent();
	
	//��������λ��
	void SetPosition(const XMFLOAT3& InNewPosition);
	//����������ת
	void SetRotation(const fvector_3d& InNewRotation);
	//������������
	void SetScale(const fvector_3d& InNewScale);

	//�������峯������
	void SetForwardVector(const XMFLOAT3& InForwardVector);
	void SetRightVector(const XMFLOAT3& InRightVevtor);
	void SetUPVector(const XMFLOAT3& InUPVevtor);

public:
	//�������λ��
	XMFLOAT3 &GetPosition()  { return Position; }
	//���������ת
	fvector_3d GetRotation() const { return fvector_3d(Rotation.x, Rotation.y, Rotation.z); }
	//�����������
	fvector_3d GetScale() const { return fvector_3d(Scale.x, Scale.y, Scale.z); }

	//������峯������
	XMFLOAT3 &GetForwardVector()  { return ForwardVector; }
	XMFLOAT3 &GetRightVector()  { return RightVector; }
	XMFLOAT3 &GetUPVector()  { return UPVector; }

public:
	//����ռ�ת������Ļ�ռ�Ľ�������֮�� ֮��᷵��һ��POSλ��  fvector_3d�Ƿ�װ�õ���ѧ����ļ���
	void CorrectionVector();

	//��ȡ����ռ�ת����Ļ�ռ���λ�� ���ַ�ʽ
	fvector_3d GetCorrectionPosition();//ͨ�����ַ�ʽ����
	void GetCorrectionPosition(fvector_3d& InPos3);//���ַ�ʽ����ֱ��ȡֵ
private:
	//�����λ�� P
	XMFLOAT3 Position;
	//������ת R
	XMFLOAT3 Rotation;
	//��������
	XMFLOAT3 Scale;

	//�������泯���� F  ����������ϵ��Z���������� ������0.f, 0.f, 1.f
	XMFLOAT3 ForwardVector;
	//�������泯���� R  X�����ҷ��� ������1.f, 0.f, 0.f
	XMFLOAT3 RightVector;
	//���峯�ϵķ��� UP   Y�ᳯ��������0.f, 1.f, 0.f
	XMFLOAT3 UPVector;


};