
#pragma once
#include "../../../EngineMinimal.h"

//材质常量缓冲区
struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer(); //初始化一下值

	int MaterialType; //定义材质类型
	float Roughness;//定义材质 粗糙度
	int BaseColorIndex; //基本颜色纹理贴图序号（从外部传入）
	int NormalIndex;//法线索引
	
	int SpecularIndex;//高光贴图索引
	int XX1;//占位
	int XX2;
	int XX3;

	XMFLOAT4 BaseColor;// 定义材质 基本颜色
	XMFLOAT3 SpecularColor;//定义高光颜色
	float Refraction;//折射度
	XMFLOAT3 FresnelF0;//菲涅尔因子
	float Transparency;//透明
	//材质变换信息
	XMFLOAT4X4 TransformInformation;
};
