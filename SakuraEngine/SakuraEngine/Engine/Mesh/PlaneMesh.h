#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

class GPlaneMesh :public GMesh   //����һ����׼����BOX �̳���FMesh
{
	typedef GMesh Super;
public:
	GPlaneMesh();

	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 

	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	void CreateMesh(
		float InHeight,   //ƽ��߶�
		float InWidth, //ƽ����
		uint32_t InHeightSubdivide,//ƽ��ĸ߶�ϸ��
		uint32_t InWidthSubdivide);//ƽ����ϸ��

};