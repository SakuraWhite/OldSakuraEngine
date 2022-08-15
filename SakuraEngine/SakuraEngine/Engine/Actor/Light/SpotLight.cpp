#include "SpotLight.h"
#include "../../Component/Light/SpotLightComponent.h"

//测试动画数值
float index_test = 0.f;

GSpotLight::GSpotLight()
	:Super()
{
	//生成灯光组件										创建灯光组件对象(点光源)
	SetLightComponent(CreateObject<CSpotLightComponent>(new CSpotLightComponent()));
	index_test = 0.f;
}

void GSpotLight::Tick(float DeltaTime)
{
	index_test += DeltaTime;//累加
	float I = 30.f;//内角圈
	float O = 50.f;//外角圈
	//SetConicalInnerCorner(I * fabsf(cos(index_test)) + 10.f);//内角圈进行cos周期波动
	//SetConicalOuterCorner(O * fabsf(cos(index_test)) + 20.f);//外角圈进行cos周期波动

	fvector_3d v3 = GetRotation();//获得灯光旋转
	v3.x += DeltaTime * 10.f;//设置X轴
	//v3.y += DeltaTime * 10.f;//设置y轴
	//v3.z += DeltaTime * 10.f;//设置z轴
	SetRotation(v3);//设置灯光旋转
}

float GSpotLight::GetConicalInnerCorner() const
{
	//强转为自己的灯光组件
	if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//获取聚光灯的内角
		InSpotLightComponent->GetConicalInnerCorner();
	}
	return 0.0f;
}

float GSpotLight::GetConicalOuterCorner() const
{
	if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//获取聚光灯的外角
		InSpotLightComponent->GetConicalOuterCorner();
	}
	return 0.0f;
}

void GSpotLight::SetConicalInnerCorner(float InConicalInnerCorner)
{
	if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//设置聚光灯内角
		InSpotLightComponent->SetConicalInnerCorner(InConicalInnerCorner);
	}
}

void GSpotLight::SetConicalOuterCorner(float InConicalOuterCorner)
{
	if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		//设置聚光灯外角
		InSpotLightComponent->SetConicalOuterCorner(InConicalOuterCorner);
	}

}


