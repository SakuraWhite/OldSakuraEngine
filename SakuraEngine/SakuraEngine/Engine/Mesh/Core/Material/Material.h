#pragma once//防止重复包含
#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

//		材质		继承来自CCoreMinimalObject
class CMaterial :public CCoreMinimalObject
{
public:
	CMaterial();

	// 设置当前模型的显示状态 线框/材质切换		输入显示模式
	void SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus);
	// 设置基本颜色
	void SetBaseColor(const fvector_4d& InBaseColor);
	// 设置材质类型		
	void SetMaterialType(const EMaterialType& InMaterialType);
	// 设置粗糙度		
	void SetRoughness(const float InRoughness);
	//设置高光
	void SetSpecular(const std::string& InAssetFilename);
	void SetSpecular(const fvector_3d& InVector);//控制值的方式高光
	//设置菲涅尔因子
	void SetFresnelF0(const fvector_3d& InF0Vector);
	//设置透明度
	void SetTransparency(float InTransparency);

	//设置材质贴图      输入贴图路径
	void SetBaseColor(const std::string& InAssetFilename);
	//设置法线贴图
	void SetNormal(const std::string& InAssetFilename);
	//设置判定动态材质内容修改状态	（如果动态材质被修改过） 
	void SetDirty(bool bNewDirty);
	//设置材质索引
	void SetMaterialIndex(int InNewIndex);
	//设置是否开启动态反射
	void SetDynamicReflection(bool InDynamicReflection);
	//设置折射度
	void SetRefractiveValue(float InRefractiveValue);
public:
	//动态反射 开关
	FORCEINLINE float IsDynamicReflection() const {   //判断是否是动态反射
		return bDynamicReflection &&     //以下这些材质可以有动态反射
			(MaterialType == EMaterialType::BackLight || //判断材质类型是否为此表面散射材质 玉器
				MaterialType == EMaterialType::Phong || //或者材质类型为 冯高光
				MaterialType == EMaterialType::BinnPhong|| //或 布林冯高光
				MaterialType == EMaterialType::Transparency);}//或折射球

	//获取透明度
	FORCEINLINE float GetTransparency()const { return Transparency; }
	//获取粗糙度
	FORCEINLINE float GetRoughness() const { return Roughness; }
	//获取基本颜色
	FORCEINLINE fvector_4d GetBaseColor() const { return BaseColor; }
	//获取高光
	FORCEINLINE fvector_3d GetSpecularColor()const { return SpecularColor; }
	//获取菲涅尔因子
	FORCEINLINE fvector_3d GetFresnelF0()const { return FresnelF0; }
	//获取材质类型
	FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
	//获取折射度
	FORCEINLINE float GetRefractiveValue()const { return Refractive; }
	//获取材质显示方式
	FORCEINLINE EMaterialDisplayStatusType GetMaterialDisplayStatus() const { return MaterialDisplayStatus; }
	//获取材质变换
	FORCEINLINE XMFLOAT4X4& GetMaterialTransform() { return MaterialTransform; }

	//判断 动态材质是否被修改
	FORCEINLINE bool IsDirty() const { return bDirty; }
	//获取基本颜色贴图索引的路径（字符串）
	FORCEINLINE const std::string& GetBaseColorIndexKey() const { return BaseColorIndexKey; }
	//获取法线贴图索引的路径（字符串）
	FORCEINLINE const std::string& GetNormalIndexKey() const { return NormalIndexKey; }
	//或许高光贴图索引路径（字符串）
	FORCEINLINE const std::string& GetSpecularKey() const { return SpecularIndexKey; }
	//获取材质索引
	FORCEINLINE int GetMaterialIndex() const { return MaterialIndex; }
private:
	bool bDirty;//判定材质是不是动态被修改过了，是的话就更新
	int MaterialIndex;//材质索引

	fvector_4d BaseColor; // 材质 基本颜色(指定的RGB数值 非贴图)
	fvector_3d SpecularColor;//高光颜色(指定的值)
	fvector_3d FresnelF0;//菲涅尔因子

	float Roughness; //材质 粗糙度
	EMaterialType MaterialType;//材质类型 枚举

	std::string BaseColorIndexKey;//读取基本颜色贴图索引的路径（字符串）
	std::string NormalIndexKey;//读取法线贴图索引的路径（字符串）
	std::string SpecularIndexKey;//读取高光贴图索引的路径（字符串）

	EMaterialDisplayStatusType MaterialDisplayStatus;//材质显示方式
	XMFLOAT4X4 MaterialTransform;//材质变换4x4矩阵

	//透明度
	float Transparency;

	//是否开启动态反射
	bool bDynamicReflection;

	//折射度
	float Refractive;
};