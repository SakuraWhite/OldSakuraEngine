#include "LightComponent.h"
#include "../../../Manage/LightManage.h"
#include "../../Mesh/Core/MeshComponent.h"

CLightComponent::CLightComponent()
	:LightIntensity(1.f, 1.f, 1.f)//初始化灯光强度
{
	//收集灯光信息
	GetLightManage()->AddLight(this);
}

CLightComponent::~CLightComponent()
{
	//之后再写移除代码
}

void CLightComponent::SetLightIntensity(const fvector_3d& InNewLightIntensity)
{
	//赋值新的灯光强度
	LightIntensity = InNewLightIntensity;
}

void CLightComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	//设置灯光位置
	Super::SetPosition(InNewPosition);
	if (LightMesh)
	{

		//灯光的模型模型位置跟着被设置
		LightMesh->SetPosition(InNewPosition);
	}
}

void CLightComponent::SetRotation(const fvector_3d& InNewRotation)
{	
	//设置灯光旋转
	Super::SetRotation(InNewRotation);
	if (LightMesh)
	{

		//灯光的模型模型旋转角度跟着变化
		LightMesh->SetRotation(InNewRotation * (-1.f));//乘以-1 原因是导入的模型跟写的代码是反向
	}

}

void CLightComponent::SetScale(const fvector_3d& InNewScale)
{
	//设置灯光缩放
	Super::SetScale(InNewScale);
	if (LightMesh)
	{

		//灯光的模型模型缩放大小跟着变化
		LightMesh->SetScale(InNewScale);
	}

}

void CLightComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	//设置灯光正向方向
	Super::SetForwardVector(InForwardVector);
	if (LightMesh)
	{

		//灯光的模型模型光正向方向
		LightMesh->SetForwardVector(InForwardVector);
	}

}

void CLightComponent::SetRightVector(const XMFLOAT3& InRightVevtor)
{
	//设置灯光右向方向
	Super::SetRightVector(InRightVevtor);
	if (LightMesh)
	{
		//灯光的模型模型光右向方向
		LightMesh->SetRightVector(InRightVevtor);
	}
}

void CLightComponent::SetUPVector(const XMFLOAT3& InUPVevtor)
{
	//设置灯光上向方向
	Super::SetUPVector(InUPVevtor);
	if (LightMesh) 
	{
		//灯光的模型模型光上向方向
		LightMesh->SetUPVector(InUPVevtor );
	}
	
}

void CLightComponent::SetLightMesh(CMeshComponent* InLightMesh)
{
	//设置灯光灯光模型
	LightMesh = InLightMesh;

}
