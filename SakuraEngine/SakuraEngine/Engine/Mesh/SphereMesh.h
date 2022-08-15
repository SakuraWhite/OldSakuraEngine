#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

class GSphereMesh :public GMesh   //����һ����׼�������� �̳���FMesh
{
	typedef GMesh Super;
public:
	GSphereMesh();

	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 


	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	//�̳���Mesh.h��ľ�̬�ṹ�������������� ��̬��ʽ������CSphereMesh
	void CreateMesh(
		float InRadius, //������ǰ뾶
		uint32_t InAxialSubdivision,//���������ϸ��
		uint32_t InHeightSubdivision,//����ĸ߶�ϸ��
		bool bReverse = false); //�Ƿ�ת����
};