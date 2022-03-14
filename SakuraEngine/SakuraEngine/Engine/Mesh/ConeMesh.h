#pragma once//��ֹ�ظ�����

#include "core/Mesh.h"

//�Զ���Mesh
class CConeMesh :public CMesh   //�Զ���Mesh
{
	typedef CMesh Super;
public:
	virtual void Init();//�̳���Mesh.h���Rendeing.h��ĵ���Ⱦ��ʼ�� 

	virtual void BuildMesh(const FMeshRenderingData* InRenderIngData);//�̳���Mesh.h�����������Mesh�ӿ�

	virtual void Draw(float DeltaTime);//�̳���Mesh.h�����ģ����ʹ��Rendeing.h����Ⱦ�ӿ�

	//�̳���Mesh.h��ľ�̬�ṹ��������������
	void CreateMesh(
		FMeshRenderingData& MeshData,//������ǰMesh�ṹ����
		float InRadius,  //Բ׶��ײ��뾶  ��ΪԲ׶�嶥���ǵ������� ������ֻ�еײ��а뾶
		float InHeight,  //Բ׶��߶�
		uint32_t InAxialSubdivision,//��ϸ��
		uint32_t InHeightSubdivision); //�߶�ϸ��
};