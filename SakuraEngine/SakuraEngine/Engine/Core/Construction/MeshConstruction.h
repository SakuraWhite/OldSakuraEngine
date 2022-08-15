#pragma once
#include "../../Mesh/Core/MeshManage.h"
//ģ�͹���

namespace MeshConstruction //��ֹ���ظ� ģ�͹���ռ�
{

	template<class T, typename ...ParamTypes>
	T* CreateMeshComponent(CMeshManage* InManage, T* InMesh, ParamTypes &&...Params)//�����������  ���������ɷ��� 
	{

		if (InManage && InMesh)
		{
			//����һ����ϣkey
			size_t HashKey = 0;
			InMesh->BuildKey(HashKey, forward<ParamTypes>(Params)...);//������ϣkey


			FRenderingData RenderingData;
			//ͨ������ �ҵ���ǰ��Ⱦ��ģ������						ͨ����ϣkey �ҵ�����Ⱦ����		��Ⱦ�㼶
			if (InManage->GetRenderingPipeline().FindMeshRenderingDataByHash(HashKey, RenderingData, (int)InMesh->GetRenderLayerType()))
			{
				//����ģ��
				InManage->GetRenderingPipeline().DuplicateMesh(InMesh, RenderingData);

			}
			else
			{
				//���û�ҵ���ϣkey��Ӧ����Ⱦ���� ��ʼ�����µ�ģ��
				//��ȡģ����Դ
				FMeshRenderingData MeshData;
				InMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);


				//����mesh
				InManage->GetRenderingPipeline().BuildMesh(HashKey, InMesh, MeshData);
			}
			InMesh->Init();

			return InMesh;
		}
		//����ʧ�ܷ���null
		return NULL;

	}

	//����ģ��  �����ǿ��������洫�������  ���ݴ��������ָ��������Ȼ������
	template<class T, typename ...ParamTypes>
	T* CreateMeshComponent(CMeshManage* InManage, ParamTypes &&...Params)//����������� ���������ɷ��� 
	{

		if (InManage)
		{
			T* InMesh = CreateObject<T>(new T());//������UE��NewObject  ����mesh����

			return CreateMeshComponent<T>(InManage, InMesh, Params...);//����mesh֮���ٽ��д���
		}
		//����ʧ�ܷ���null
		return NULL;
	}
}

