#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

class CSphereMesh :public CMesh   //����һ����׼�������� �̳���FMesh
{
	typedef CMesh Super;
public:
	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 

	virtual void BuildMesh(const FMeshRendingData* InRenderIngData);//�̳���Mesh.h�����������Mesh�ӿ�

	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	//�̳���Mesh.h��ľ�̬�ṹ�������������� ��̬��ʽ������CSphereMesh
	static CSphereMesh* CreateMesh(
		float InRadius, //������ǰ뾶
		uint32_t InAxialSubdivision,//���������ϸ��
		uint32_t InHeightSubdivision); //����ĸ߶�ϸ��
};