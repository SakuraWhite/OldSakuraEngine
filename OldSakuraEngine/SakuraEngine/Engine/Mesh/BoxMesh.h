#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

class CBoxMesh :public CMesh   //����һ����׼����BOX �̳���FMesh
{
	typedef CMesh Super;
public:
	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 
	
	virtual void BuildMesh(const FMeshRendingData* InRenderIngData);//�̳���Mesh.h�����������Mesh�ӿ�
	
	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	static CBoxMesh* CreateMesh(
		float InHeight,   //BOX�߶�
		float InWidth,   //BOX���
		float InDepth);//BOX���
};