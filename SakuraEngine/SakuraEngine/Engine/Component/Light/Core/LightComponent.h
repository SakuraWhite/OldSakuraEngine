#pragma once
#include "../../TransformationComponent.h"
#include "../../../Interface/DirectXDeviceInterfece.h"
#include "LightType.h"

class CMeshComponent;

/// 灯光控制组件					继承自玩家控制组件			   继承渲染内容的接口
class CLightComponent :public CTransformationComponent, public IDirectXDeviceInterfece
{
	typedef CTransformationComponent Super;

	CVARIABLE()
		CMeshComponent* LightMesh; //用来可视化平行光的模型
public:

	//创建
	CLightComponent();

	//移除
	virtual ~CLightComponent();

	//设置灯光强度
	void SetLightIntensity(const fvector_3d& InNewLightIntensity);
public:
	//获取到灯光模型接口
	CMeshComponent* GetLightMesh() { return LightMesh; }
	//获取灯光强度
	const fvector_3d& GetLightIntensity() { return LightIntensity; }
	//获取灯光类型
	ELightType GetLightType() const { return LightType; }
public:
	//设置灯光位置
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	//设置灯光旋转
	virtual void SetRotation(const fvector_3d& InNewRotation);
	//设置灯光缩放
	virtual void SetScale(const fvector_3d& InNewScale);

	//设置灯光朝向向量
	virtual void SetForwardVector(const XMFLOAT3& InForwardVector);
	virtual void SetRightVector(const XMFLOAT3& InRightVector);
	virtual void SetUPVector(const XMFLOAT3& InUPVector);

protected:
	//设置灯光
	void SetLightMesh(CMeshComponent* InLightMesh);

protected:
	//设置灯光强度
	fvector_3d LightIntensity;
	//设置灯光类型
	ELightType LightType;
};