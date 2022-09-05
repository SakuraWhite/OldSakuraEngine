#include "ClientViewport.h"
#include "../../Component/TransformationComponent.h"
#include "../../Math/EngineMath.h"

GClientViewport::GClientViewport()
	: SuperV()		//��ʼ������ FViewport
	, SuperA()		//��ʼ������ GActorObject
	, YFOV(0.f)     //��ʼ��fov
	, Aspect(0.f)	//��ʼ����߱�
	, ZNear(0.f)	//��ʼ����������
	, ZFar(0.f)		//��ʼ��Զ������
	, bDirty(true)	//��ʼ����� ��
{
}

void GClientViewport::SetFrustum(float InYFOV, float InAspect, float InZNear, float InZFar)
{
	
	YFOV = InYFOV;//�ɼ�fov
	Aspect = InAspect;//�ɼ���߱�
	ZNear = InZNear;//�ɼ���������
	ZFar = InZFar;//�ɼ�Զ������

	//ͶӰ����
	//���ǽ�3D�ռ��ת����2D����ת����3D  ���ǻ�����Ұ��������͸��ͶӰ����
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		InYFOV, //�Ի���Ϊ��λ�����϶��µ��ӳ��ǡ�������45�� FOV
		InAspect,//��ͼor��Ļ�ռ� X:Y ���ݺ�ȡ�
		InZNear,		//��������ƽ��ľ��� ���������
		InZFar		//������ƽ�浽Զ������ľ��� ��
	);

	XMStoreFloat4x4(&ProjectMatrix, Project);//����ռ��ת��

	//����
	SetDirty(true);
}

void GClientViewport::FaceTarget(
	const fvector_3d& InPosition,     //����λ��	
	const fvector_3d& InTargetPosition, //Ŀ��λ��	
	const fvector_3d& InUP)	//��һ�����Ϸ���  Ĭ��ֵ
{
	//����Ŀ�곯��					 Ŀ��λ��  - ԭ����λ�� 
	fvector_3d TmpFaceVector = InTargetPosition - InPosition;
	TmpFaceVector.normalize();//��λ������

	//���ҷ���								���			Ĭ�ϵ��Ϸ��������泯��
	fvector_3d TmpRightVector = fvector_3d::cross_product(InUP, TmpFaceVector);
	TmpRightVector.normalize();//��λ������

	//���Ϸ���(�����Ϸ��� ������Ĭ�ϵ��Ϸ���)		���       ���泯���˳��ҷ���
	fvector_3d TmpUPVector = fvector_3d::cross_product(TmpFaceVector, TmpRightVector);
	TmpUPVector.normalize();//��λ������

	//ת��					ͨ��EngineMath����ת��
	XMFLOAT3 XMFaceVector = EngineMath::ToFloat3(TmpFaceVector);
	XMFLOAT3 XMRightVector = EngineMath::ToFloat3(TmpRightVector);
	XMFLOAT3 XMUPVector = EngineMath::ToFloat3(TmpUPVector);

	//��ֵ����
	GetForwardVector() = XMFaceVector;
	GetRightVector() = XMRightVector;
	GetUPVector() = XMUPVector;

	//����
	SetDirty(true);
}

void GClientViewport::Tick(float DeltaTime)
{
	//ִ�й�����Ļ�ռ�
	BuildViewMatrix(DeltaTime);
}

void GClientViewport::BuildViewMatrix(float DeltaTime)
{
	//��������ռ�ת������Ļ�ռ����� ������
	GetTransformationComponent()->CorrectionVector();
	//���㰴�������ƶ���ͼ
	fvector_3d V3;	// fvector_3d�Ƿ�װ�õ���ѧ����ļ���
	//��ô�����ռ�ת������Ļ�ռ��Ľ���λ��
	GetTransformationComponent()->GetCorrectionPosition(V3);

	//������Ļ�ռ�VewMatrix
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();//��ȡ�ҳ�����
	XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();//��ȡ�ϳ�����
	XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();//��ȡ��������

	//�����������ʽ  ��Ļ�ռ����
	ViewMatrix = {
		RightVector.x,	UPVector.x,	ForwardVector.x,	0.f,
		RightVector.y,	UPVector.y,	ForwardVector.y,	0.f,
		RightVector.z,	UPVector.z,	ForwardVector.z,	0.f,
		V3.x,			V3.y,		V3.z,				1.f };
}
