#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

class GPlaneMesh :public GMesh   //����һ����׼����BOX �̳���FMesh
{
	typedef GMesh Super;
public:
	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 

	virtual void BuildMesh(const FMeshRenderingData* InRenderIngData);//�̳���Mesh.h�����������Mesh�ӿ�

	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	void CreateMesh(
		FMeshRenderingData& MeshData,//������ǰMesh�ṹ����
		float InHeight,   //ƽ��߶�
		float InWidth, //ƽ����
		uint32_t InHeightSubdivide,//ƽ��ĸ߶�ϸ��
		uint32_t InWidthSubdivide);//ƽ����ϸ��

};