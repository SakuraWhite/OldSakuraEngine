#pragma once
#include "../../TransformationComponent.h"
#include "MeshComponentType.h"

//ǰ�ð���
class CMaterial;
struct FMeshRenderingData;

/// Mesh�������					�̳�����ҿ������
class CMeshComponent :public CTransformationComponent
{
	CVARIABLE()
		vector<CMaterial*> Materials;//������ ����
public:
	CMeshComponent();//��ʼ��


	virtual void Init(); //Rendeing.h��ĵ���Ⱦ��ʼ��

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);//��������Mesh�ӿ�

	//����ģ����Ⱦ�㼶����
	void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);
public:
	//���ʻ�ȡ�ӿ�

	//��ȡ���ʵ����� 
	UINT GetMaterialNum()const;

	//��ȡģ����Ⱦ�㼶����
	EMeshRenderLayerType GetRenderLayerType() const { return MeshRenderLayerType; }

	//��ȡ����
	vector<CMaterial*>* GetMaterials() { return &Materials; }

	//ģ����Ⱦ�㼶����
	EMeshRenderLayerType MeshRenderLayerType;
};