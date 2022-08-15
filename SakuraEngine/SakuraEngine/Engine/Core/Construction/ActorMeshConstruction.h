#pragma once
#include "MeshConstruction.h"
#include "../../Mesh/Core/Mesh.h"

namespace ActorMeshConstruction//��ֹ���ظ� ActorMesh�������   ���ƿռ�
{
	//��������������Ⱦ�������
	template<class T, typename ...ParamTypes>
	T* CreateMeshRenderData(CMeshManage* InManage, GMesh* InGMesh, ParamTypes &&...Params)
	{
		//ͨ��mesh�����õ�����mesh��������Ϣ				��Ϣ			ģ�����						�Զ���ӿ�
		return MeshConstruction::CreateMeshComponent<T>(InManage, InGMesh->GetMeshComponent<T>(), Params...);

	}

}