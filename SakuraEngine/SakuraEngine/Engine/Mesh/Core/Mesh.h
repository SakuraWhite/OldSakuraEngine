#pragma once//��ֹ�ظ�����
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"


//��Ҫ�̳�һ�¼̳�����C��ͷ�ĺ���                      �����ǽӿ�
class CMesh :public CCoreMinimalObject ,public IRenderingInterface
{
public:
	CMesh();//��ʼ��Index��Vertex��һЩ����������

	virtual void Init(); //Rendeing.h��ĵ���Ⱦ��ʼ��

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);//��������Mesh�ӿ�

	virtual void PreDraw(float DeltaTime); //��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);  //��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw

	//static CMesh*CreateMesh(const FMeshRenderingData *InRenderingData); //��̬�ṹ�������������� ��̬��ʽ������Mesh


};