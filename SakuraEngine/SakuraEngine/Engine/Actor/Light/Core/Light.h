#pragma  once 
#include "../../Core/ActorObject.h"

class CLightComponent;

// 点光源控制		  继承自GActorObject核心对象
class GLight : public GActorObject
{
	typedef GActorObject Super;

	CVARIABLE()  //反射宏
	CLightComponent* LightComponent;//积类的灯

public:
	GLight();

	virtual void Tick(float DeltaTime);//随着帧执行

public:
	//设置灯光的位置数据
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	//设置灯光的旋转数据  欧拉角
	virtual void SetRotation(const fvector_3d& InRotation);
	//设置灯光的缩放数据
	virtual void SetScale(const fvector_3d& InNewScale);
	
public:
	//设置灯光强度
	void SetLightIntensity(const fvector_3d& InNewLightIntensity);

protected:
	//设置灯光组件
	virtual void SetLightComponent(CLightComponent* InNewComponent);

public:
	//快捷获取灯光组件
	const CLightComponent* GetLightComponent() const { return  LightComponent; }
};