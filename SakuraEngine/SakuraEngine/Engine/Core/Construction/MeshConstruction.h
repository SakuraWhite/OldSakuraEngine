#pragma once
#include "../../Mesh/Core/MeshManage.h"
//模型构造

namespace MeshConstruction //防止有重复 模型构造空间
{

	template<class T, typename ...ParamTypes>
	T* CreateMeshComponent(CMeshManage* InManage, T* InMesh, ParamTypes &&...Params)//创建网格组件  创建与生成分离 
	{

		if (InManage && InMesh)
		{
			//创建一个哈希key
			size_t HashKey = 0;
			InMesh->BuildKey(HashKey, forward<ParamTypes>(Params)...);//创建哈希key


			FRenderingData RenderingData;
			//通过管线 找到当前渲染的模型数据						通过哈希key 找到了渲染数据		渲染层级
			if (InManage->GetRenderingPipeline().FindMeshRenderingDataByHash(HashKey, RenderingData, (int)InMesh->GetRenderLayerType()))
			{
				//复制模型
				InManage->GetRenderingPipeline().DuplicateMesh(InMesh, RenderingData);

			}
			else
			{
				//如果没找到哈希key对应的渲染数据 则开始构建新的模型
				//提取模型资源
				FMeshRenderingData MeshData;
				InMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);


				//构建mesh
				InManage->GetRenderingPipeline().BuildMesh(HashKey, InMesh, MeshData);
			}
			InMesh->Init();

			return InMesh;
		}
		//创建失败返回null
		return NULL;

	}

	//制作模板  作用是可以往里面传任意参数  根据传入参数与指定的类型然后生成
	template<class T, typename ...ParamTypes>
	T* CreateMeshComponent(CMeshManage* InManage, ParamTypes &&...Params)//生成网格组件 创建与生成分离 
	{

		if (InManage)
		{
			T* InMesh = CreateObject<T>(new T());//类似于UE的NewObject  生成mesh对象

			return CreateMeshComponent<T>(InManage, InMesh, Params...);//生成mesh之后再进行创建
		}
		//生成失败返回null
		return NULL;
	}
}

