#include "TransformationComponent.h"

CTransformationComponent::CTransformationComponent()
	:Position		(0.f, 0.f, 0.f) //物体位置初始化
	,Rotation		(0.f, 0.f, 0.f) //物体旋转初始化
	,Scale			(1.f, 1.f, 1.f) //物体缩放初始化

	,RightVector	(1.f, 0.f, 0.f) //物体右方朝向初始化
	,UPVector		(0.f, 1.f, 0.f) //物体上方朝向初始化
	,ForwardVector	(0.f, 0.f, 1.f) //物体正面朝向初始化
{


}

void CTransformationComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	//给Position赋值
	Position = InNewPosition;
}

void CTransformationComponent::SetRotation(const fvector_3d& InNewRotation)
{
	//将物体从角度转换为弧度 注意计算顺序先Roll 然后Pith最后Yaw
	float RollRadians = XMConvertToRadians(InNewRotation.z);
	float PithRadians = XMConvertToRadians(InNewRotation.x);
	float YawRadians = XMConvertToRadians(InNewRotation.y);

	//利用弧度来计算旋转矩阵					DX12封装的矩阵算法
	XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(
		PithRadians, YawRadians, RollRadians);

	//拿到物体的朝向来进行旋转的计算
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	//求出每一个朝向经过旋转矩阵计算后的旋转效果
	XMStoreFloat3(&RightVector, XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&UPVector, XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationRollPitchYawMatrix));
}

void CTransformationComponent::SetScale(const fvector_3d& InNewScale)
{
	//缩放直接设置XYZ得值即可
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
	//转置为齐次坐标
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	//单位化正面朝向的向量 只需要方向不需要大小
	Forward = XMVector3Normalize(Forward);
	//进行叉乘计算出下一个轴 计算单位化的面朝上的向量  
	Up = XMVector3Normalize(XMVector3Cross(Forward, Right));//叉乘我们的正面朝向方向与右面朝向方向 得出垂直于这两个向量的朝上方向的向量
	//叉乘计算出右方向的向量
	Right = XMVector3Cross(Up, Forward);//叉乘上方已经拿到的上方向向量，正面方向向量 得到垂直于这两个向量的右方向

	//讲求出的向量的值 赋值到原本的RightVector、UPVector、ForwardVector中
	XMStoreFloat3(&RightVector, Right);
	XMStoreFloat3(&UPVector, Up);
	XMStoreFloat3(&ForwardVector, Forward);


}

fvector_3d CTransformationComponent::GetCorrectionPosition()
{
	//本质是创建了一个3D
	fvector_3d F3D;
	GetCorrectionPosition(F3D);
	return F3D;
}

void CTransformationComponent::GetCorrectionPosition(fvector_3d& InPos3)
{
	//将方向再次转置一下空间才能参与计算
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UPVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);


	//将世界空间转置为屏幕空间
	XMVECTOR NewPosition = XMLoadFloat3(&Position);





	//点乘  用屏幕空间下的新的位置点乘右朝向向量就是我们的转换后的屏幕空间位置X轴
	InPos3.x = -XMVectorGetX(XMVector3Dot(NewPosition, Right));
	//同上 这是Y轴  新位置点乘朝上向量
	InPos3.y = -XMVectorGetX(XMVector3Dot(NewPosition, Up));
	//同上 Z轴  新位置点乘正面朝向向量
	InPos3.z = -XMVectorGetX(XMVector3Dot(NewPosition, Forward));
}
