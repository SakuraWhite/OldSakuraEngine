#include "Material.h"
#include "../../../Math/EngineMath.h"

CMaterial::CMaterial()
	:bDirty(true)   //初始化判断是否为动态更新 默认为true
	, MaterialIndex(-1)//初始化材质索引 -1为无
	,BaseColor(0.5f, 0.5f, 0.5f, 1.f) //初始化基本颜色
	,SpecularColor(0.f, 0.f, 0.f)//初始化高光颜色
	,Roughness(0.2f)  //初始化粗糙度
	,MaterialType(EMaterialType::Lambert) //材质类型初始化为兰伯特
	,MaterialDisplayStatus(EMaterialDisplayStatusType::TriangleDisplay) //材质显示初始化三角形显示
	,MaterialTransform(EngineMath::IdentityMatrix4x4())//初始化材质变换矩阵
{
}

void CMaterial::SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus)
{
	//线框/材质切换输入显示模式
	MaterialDisplayStatus = InDisplayStatus;
	//如果输入显示模式 则更改判定动态贴图测试状态为true
	SetDirty(true);
}

void CMaterial::SetBaseColor(const fvector_4d& InBaseColor)
{
	//输入基本颜色（RGB并不是贴图）
	BaseColor = InBaseColor;
	//如果输入指定RGB颜色  则更改判定动态贴图测试状态为true
	SetDirty(true);
}

void CMaterial::SetMaterialType(const EMaterialType& InMaterialType)
{
	//输入材质类型
	MaterialType = InMaterialType;

	//如果输入材质类型 则更改判定动态贴图测试状态为true
	SetDirty(true);
}

void CMaterial::SetRoughness(const float InRoughness)
{
	//输入粗糙度
	Roughness = InRoughness;

	//如果输入粗糙度 则更改判定动态贴图测试状态为true
	SetDirty(true);
}

void CMaterial::SetSpecular(const std::string& InAssetFilename)
{
	//输入高光度贴图
	SpecularIndexKey = InAssetFilename;
	//动态贴图测试状态为true
	SetDirty(true);
}

void CMaterial::SetSpecular(const fvector_3d& InVector)
{
	//输入数值控制高光
	SpecularColor = InVector;
	//动态贴图测试状态为true
	SetDirty(true);
}

void CMaterial::SetBaseColor(const std::string& InAssetFilename)
{
	//输入的颜色贴图路径
	BaseColorIndexKey = InAssetFilename;

	//如果输入新的颜色贴图 则更改判定动态贴图测试状态为true
	SetDirty(true);
}

void CMaterial::SetNormal(const std::string& InAssetFilename)
{
	//输入的法线贴图路径
	NormalIndexKey = InAssetFilename;

	SetDirty(true);
}

void CMaterial::SetDirty(bool bNewDirty)
{
	//设置判定动态材质新状态
	bDirty = bNewDirty;
}

void CMaterial::SetMaterialIndex(int InNewIndex)
{
	//更新新的材质索引
	MaterialIndex = InNewIndex;
	//设置判定动态材质新状态
	SetDirty(true);
}
