#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

//�Զ���Mesh
class GCustomMesh :public GMesh   //�Զ���Mesh
{
	typedef GMesh Super;
public:
	GCustomMesh();

	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 

	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	//�̳���Mesh.h��ľ�̬�ṹ�������������� ��̬��ʽ�������Զ���Mesh
	void CreateMesh(string& InPath);//������ȡFBX

};