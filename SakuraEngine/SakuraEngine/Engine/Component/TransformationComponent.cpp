#include "TransformationComponent.h"

CTransformationComponent::CTransformationComponent()
	:Position		(0.f, 0.f, 0.f) //����λ�ó�ʼ��
	,Rotation		(0.f, 0.f, 0.f) //������ת��ʼ��
	,Scale			(1.f, 1.f, 1.f) //�������ų�ʼ��

	,RightVector	(1.f, 0.f, 0.f) //�����ҷ������ʼ��
	,UPVector		(0.f, 1.f, 0.f) //�����Ϸ������ʼ��
	,ForwardVector	(0.f, 0.f, 1.f) //�������泯���ʼ��
{


}

void CTransformationComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	//��Position��ֵ
	Position = InNewPosition;
}

void CTransformationComponent::SetRotation(const fvector_3d& InNewRotation)
{
	//������ӽǶ�ת��Ϊ���� ע�����˳����Roll Ȼ��Pith���Yaw
	float RollRadians = XMConvertToRadians(InNewRotation.z);
	float PithRadians = XMConvertToRadians(InNewRotation.x);
	float YawRadians = XMConvertToRadians(InNewRotation.y);

	//���û�����������ת����					DX12��װ�ľ����㷨
	XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(
		PithRadians, YawRadians, RollRadians);

	//�õ�����ĳ�����������ת�ļ���
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	//���ÿһ�����򾭹���ת�����������תЧ��
	XMStoreFloat3(&RightVector, XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&UPVector, XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationRollPitchYawMatrix));
}

void CTransformationComponent::SetScale(const fvector_3d& InNewScale)
{
	//����ֱ������XYZ��ֵ����
	Scale.x = InNewScale.x;
	Scale.y = InNewScale.y;
	Scale.z = InNewScale.z;
}

void CTransformationComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	ForwardVector = InForwardVector;
}

void CTransformationComponent::SetRightVevtor(const XMFLOAT3& InRightVevtor)
{
	RightVector = InRightVevtor;
}

void CTransformationComponent::SetUPVevtor(const XMFLOAT3& InUPVevtor)
{
	UPVector = InUPVevtor;
}

void CTransformationComponent::CorrectionVector()
{
	//ת��Ϊ�������
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	//��λ�����泯������� ֻ��Ҫ������Ҫ��С
	Forward = XMVector3Normalize(Forward);
	//���в�˼������һ���� ���㵥λ�����泯�ϵ�����  
	Up = XMVector3Normalize(XMVector3Cross(Forward, Right));//������ǵ����泯���������泯���� �ó���ֱ�������������ĳ��Ϸ��������
	//��˼�����ҷ��������
	Right = XMVector3Cross(Up, Forward);//����Ϸ��Ѿ��õ����Ϸ������������淽������ �õ���ֱ���������������ҷ���

	//�������������ֵ ��ֵ��ԭ����RightVector��UPVector��ForwardVector��
	XMStoreFloat3(&RightVector, Right);
	XMStoreFloat3(&UPVector, Up);
	XMStoreFloat3(&ForwardVector, Forward);


}

fvector_3d CTransformationComponent::GetCorrectionPosition()
{
	//�����Ǵ�����һ��3D
	fvector_3d F3D;
	GetCorrectionPosition(F3D);
	return F3D;
}

void CTransformationComponent::GetCorrectionPosition(fvector_3d& InPos3)
{
	//�������ٴ�ת��һ�¿ռ���ܲ������
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);


	//������ռ�ת��Ϊ��Ļ�ռ�
	XMVECTOR NewPosition = XMLoadFloat3(&Position);





	//���  ����Ļ�ռ��µ��µ�λ�õ���ҳ��������������ǵ�ת�������Ļ�ռ�λ��X��
	InPos3.x = -XMVectorGetX(XMVector3Dot(NewPosition, Right));
	//ͬ�� ����Y��  ��λ�õ�˳�������
	InPos3.y = -XMVectorGetX(XMVector3Dot(NewPosition, Up));
	//ͬ�� Z��  ��λ�õ�����泯������
	InPos3.z = -XMVectorGetX(XMVector3Dot(NewPosition, Forward));
}
