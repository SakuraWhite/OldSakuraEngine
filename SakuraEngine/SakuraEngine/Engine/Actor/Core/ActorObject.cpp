#include "ActorObject.h"
#include "../../EngineMinimal.h"
#include "../../Component/TransformationComponent.h"

GActorObject::GActorObject()
{
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

void GActorObject::SetPosition(const XMFLOAT3& InNewPosition)
{
	//设置物体位置信息
	TransformationComponent->SetPosition(InNewPosition);
}

void GActorObject::SetRotation(const fvector_3d& InRotation)
{
	//设置物体旋转信息
	TransformationComponent->SetRotation(InRotation);
}

void GActorObject::SetScale(const fvector_3d& InNewScale)
{
	//设置物体缩放信息
	TransformationComponent->SetScale(InNewScale);
}


XMFLOAT3& GActorObject::GetPosition()
{
	//获得物体位置信息
	return TransformationComponent->GetPosition();
}

fvector_3d GActorObject::GetRotation() const
{
	//获得物体旋转信息
	return TransformationComponent->GetRotation();
}

fvector_3d GActorObject::GetScale() const
{
	//获得物体缩放信息
	return TransformationComponent->GetScale();
}

XMFLOAT3& GActorObject::GetForwardVector()
{
	//获得物体前向信息
	return TransformationComponent->GetForwardVector();
}

XMFLOAT3& GActorObject::GetRightVector()
{
	//获得物体右朝向信息
	return TransformationComponent->GetRightVector();
}

XMFLOAT3& GActorObject::GetUPVector()
{
	//获得物体朝上信息
	return TransformationComponent->GetUPVector();
}

