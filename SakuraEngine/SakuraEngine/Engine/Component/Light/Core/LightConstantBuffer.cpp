#include "LightConstantBuffer.h"
#include "LightType.h"

FLightConstantBuffer::FLightConstantBuffer()
{
	//进行初始化
	memset(&SceneLights, 0, sizeof(SceneLights));

}

FLight::FLight()
	:StartAttenuation(1.f) //初始化灯光衰减起点
	,EndAttenuation(10.f) //初始化灯光衰减结束点
	,LightType((int)ELightType::DirectionalLight)//初始化灯光类型(平行光)
{
}
