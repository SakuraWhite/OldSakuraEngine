#include "ActorObject.h"
#include "../../EngineMinimal.h"
#include "../../Component/TransformationComponent.h"

GActorObject::GActorObject()
{
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

void GActorObject::SetPosition(const XMFLOAT3& InNewPosition)
{
	//��������λ����Ϣ
	TransformationComponent->SetPosition(InNewPosition);
}

void GActorObject::SetRotation(const fvector_3d& InRotation)
{
	//����������ת��Ϣ
	TransformationComponent->SetRotation(InRotation);
}

void GActorObject::SetScale(const fvector_3d& InNewScale)
{
	//��������������Ϣ
	TransformationComponent->SetScale(InNewScale);
}


XMFLOAT3& GActorObject::GetPosition()
{
	//�������λ����Ϣ
	return TransformationComponent->GetPosition();
}

fvector_3d GActorObject::GetRotation() const
{
	//���������ת��Ϣ
	return TransformationComponent->GetRotation();
}

fvector_3d GActorObject::GetScale() const
{
	//�������������Ϣ
	return TransformationComponent->GetScale();
}

XMFLOAT3& GActorObject::GetForwardVector()
{
	//�������ǰ����Ϣ
	return TransformationComponent->GetForwardVector();
}

XMFLOAT3& GActorObject::GetRightVector()
{
	//��������ҳ�����Ϣ
	return TransformationComponent->GetRightVector();
}

XMFLOAT3& GActorObject::GetUPVector()
{
	//������峯����Ϣ
	return TransformationComponent->GetUPVector();
}

