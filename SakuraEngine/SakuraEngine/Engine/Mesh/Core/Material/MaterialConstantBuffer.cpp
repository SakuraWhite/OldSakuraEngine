#include "MaterialConstantBuffer.h"
#include "../../../Math/EngineMath.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: MaterialType(0) //材质类型序号
	, BaseColor(0.5f, 0.5f, 0.5f, 1.f)   //材质 基本颜色初始化
	, Roughness(0.2f)  //粗糙度  控制高光范围
	, BaseColorIndex(-1)//颜色材质贴图序号初始化 -1代表什么也没有
	, NormalIndex(-1) //初始化法线索引
	, SpecularIndex(-1)//初始化高光索引
	, TransformInformation(EngineMath::IdentityMatrix4x4())   //单位化材质变换信息
{
}
