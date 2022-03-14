#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

class CPlaneMesh :public CMesh   //����һ����׼����BOX �̳���FMesh
{
	typedef CMesh Super;
public:
	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 

	virtual void BuildMesh(const FMeshRendingData* InRenderIngData);//�̳���Mesh.h�����������Mesh�ӿ�

	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	static CPlaneMesh* CreateMesh(
		float InHeight,   //ƽ��߶�
		float InWidth, //ƽ����
		uint32_t InHeightSubdivide,//ƽ��ĸ߶�ϸ��
		uint32_t InWidthSubdivide);//ƽ����ϸ��
};