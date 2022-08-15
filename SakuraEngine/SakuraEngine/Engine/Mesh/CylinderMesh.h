#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

class GCylinderMesh :public GMesh   //����һ����׼�������� �̳���FMesh
{
	typedef GMesh Super;
public:
	GCylinderMesh();

	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 


	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	//�̳���Mesh.h��ľ�̬�ṹ�������������� ��̬��ʽ������CCylinderMesh
	void CreateMesh(
		float InTopRadius, //Բ�������뾶
		float InBottomRadius, //Բ���ײ��뾶
		float InHeight,//Բ����ĸ߶�
		uint32_t InAxialSubdivision,//���������ϸ��
		uint32_t InHeightSubdivision); //����ĸ߶�ϸ��
};