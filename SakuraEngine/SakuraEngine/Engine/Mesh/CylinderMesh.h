#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

class CCylinderMesh :public CMesh   //����һ����׼�������� �̳���FMesh
{
	typedef CMesh Super;
public:
	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 

	virtual void BuildMesh(const FMeshRenderingData* InRenderIngData);//�̳���Mesh.h�����������Mesh�ӿ�

	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	//�̳���Mesh.h��ľ�̬�ṹ�������������� ��̬��ʽ������CCylinderMesh
	void CreateMesh(
		FMeshRenderingData& MeshData,//������ǰMesh�ṹ����
		float InTopRadius, //Բ�������뾶
		float InBottomRadius, //Բ���ײ��뾶
		float InHeight,//Բ����ĸ߶�
		uint32_t InAxialSubdivision,//���������ϸ��
		uint32_t InHeightSubdivision); //����ĸ߶�ϸ��
};