#include "PointLight.h"
#include "../../Component/Light/PointLightComponent.h"
GPointLight::GPointLight()
	:Super()
{
	//生成灯光组件											创建灯光组件对象(点光源)
	SetLightComponent(CreateObject<CPointLightComponent>(new CPointLightComponent()));
}

void GPointLight::Tick(float DeltaTime)
{
	//fvector_3d v3 = GetRotation();//获得灯光旋转

	//v3.x += DeltaTime * 10.f;//设置X轴
	//v3.y += DeltaTime * 10.f;//设置y轴
	//v3.z += DeltaTime * 10.f;//设置z轴
	//SetRotation(v3);//设置灯光旋转
}




