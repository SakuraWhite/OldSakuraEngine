#pragma once
#include "MeshConstruction.h"
#include "../../Mesh/Core/Mesh.h"

namespace ActorMeshConstruction//防止有重复 ActorMesh组件构造   名称空间
{
	//创建创建网格渲染数据组件
	template<class T, typename ...ParamTypes>
	T* CreateMeshRenderData(CMeshManage* InManage, GMesh* InGMesh, ParamTypes &&...Params)
	{
		//通过mesh构造拿到构造mesh组件里的信息				信息			模型组件						自定义接口
		return MeshConstruction::CreateMeshComponent<T>(InManage, InGMesh->GetMeshComponent<T>(), Params...);

	}

}