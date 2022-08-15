#pragma once
#include "../../TransformationComponent.h"
#include "MeshComponentType.h"

//前置包含
class CMaterial;
struct FMeshRenderingData;

/// Mesh控制组件					继承自玩家控制组件
class CMeshComponent :public CTransformationComponent
{
	CVARIABLE()
		vector<CMaterial*> Materials;//材质球 载体
public:
	CMeshComponent();//初始化


	virtual void Init(); //Rendeing.h里的的渲染初始化

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);//用来构建Mesh接口

	//设置模型渲染层级类型
	void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);
public:
	//材质获取接口

	//获取材质的数量 
	UINT GetMaterialNum()const;

	//获取模型渲染层级类型
	EMeshRenderLayerType GetRenderLayerType() const { return MeshRenderLayerType; }

	//获取材质
	vector<CMaterial*>* GetMaterials() { return &Materials; }

	//模型渲染层级类型
	EMeshRenderLayerType MeshRenderLayerType;
};