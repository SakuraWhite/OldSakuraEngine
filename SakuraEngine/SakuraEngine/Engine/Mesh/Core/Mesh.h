#pragma once//��ֹ�ظ�����
#include "../../Rendering/Core/Rendering.h"
#include "../../Actor/Core/ActorObject.h"
#include "MeshType.h"

class CTransformationComponent;
//				�̳�����GActorObject           �����ǽӿ�
class GMesh :public GActorObject,public IRenderingInterface
{
	CVARIABLE()  //�����
	CTransformationComponent* TransformationComponent;//����ϣ��ģ�������ܹ��ƶ� ����Ҫ�������
public:
	GMesh();//��ʼ��Index��Vertex��һЩ����������

	virtual void Init(); //Rendeing.h��ĵ���Ⱦ��ʼ��

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);//��������Mesh�ӿ�

	virtual void PreDraw(float DeltaTime); //��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);  //��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw

	//static GMesh*CreateMesh(const FMeshRenderingData *InRenderingData); //��̬�ṹ�������������� ��̬��ʽ������Mesh



};