#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

class GBoxMesh :public GMesh   //����һ����׼����BOX �̳���FMesh
{
	typedef GMesh Super;
public:
	GBoxMesh();

	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 
	
	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	void CreateMesh(
		float InHeight,   //BOX�߶�
		float InWidth,   //BOX���
		float InDepth);//BOX���
};