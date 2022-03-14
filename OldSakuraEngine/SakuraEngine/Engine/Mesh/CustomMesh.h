#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

//�Զ���Mesh
class CCustomMesh :public CMesh   //�Զ���Mesh
{
	typedef CMesh Super;
public:
	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 

	virtual void BuildMesh(const FMeshRendingData* InRenderIngData);//�̳���Mesh.h�����������Mesh�ӿ�

	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	//�̳���Mesh.h��ľ�̬�ṹ�������������� ��̬��ʽ�������Զ���Mesh
	static CCustomMesh* CreateMesh(string &InPath); //������ȡFBX

	//�����ж϶�ȡ�Ƿ�ΪOBJ
	static bool LoadObjFormBuff(char *InBuff, uint32_t InBuffSize, FMeshRendingData &MeshData);
};