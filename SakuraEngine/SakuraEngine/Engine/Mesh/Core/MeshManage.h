#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterfece.h"
#include "../../Core/Viewport/ViewportInfo.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"


class FRenderingResourcesUpdate;
class CMeshComponent;
class CMeshManage 
	:public CCoreMinimalObject, 
	public IRenderingInterface,
	public IDirectXDeviceInterfece
{
public:
	CMeshManage();//��ʼ��Index��Vertex��һЩ����������

	virtual void Init(); //Rendeing.h��ĵ���Ⱦ��ʼ��

	virtual void BuildMesh();//��������Mesh�ӿ�
	//����MeshManage����ѧ�ķ���
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);



	virtual void PreDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);//��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw


public:
	CMeshComponent* CreateBoxMeshComponent(
		float InHeight,			//BOX�߶�
		float InWidth,			//BOX���
		float InDepth);			//BOX���

	CMeshComponent* CreateConeMeshComponent(
		float InRadius,						//Բ׶��ײ��뾶  ��ΪԲ׶�嶥���ǵ������� ������ֻ�еײ��а뾶
		float InHeight,						//Բ׶��߶�
		uint32_t InAxialSubdivision,		//��ϸ��
		uint32_t InHeightSubdivision);		//�߶�ϸ��

	CMeshComponent* CreateCylinderMeshComponent(
		float InTopRadius,					//Բ�������뾶
		float InBottomRadius,				//Բ���ײ��뾶
		float InHeight,						//Բ����ĸ߶�
		uint32_t InAxialSubdivision,		//���������ϸ��
		uint32_t InHeightSubdivision);		//����ĸ߶�ϸ��

	CMeshComponent* CreatePlaneMeshComponent(
		float InHeight,						//ƽ��߶�
		float InWidth,						//ƽ����
		uint32_t InHeightSubdivide,			//ƽ��ĸ߶�ϸ��
		uint32_t InWidthSubdivide);			//ƽ����ϸ��

	CMeshComponent* CreateSphereMeshComponent(
		float InRadius,						//������ǰ뾶
		uint32_t InAxialSubdivision,		//���������ϸ��
		uint32_t InHeightSubdivision,		//����ĸ߶�ϸ��
		bool bReverse);		

	CMeshComponent* CreatePyramidMeshComponent(
		EPyramidNumberSides InPyramidNumberSidesType,//��׶�����
		uint32_t InHeightSubdivide,//��׶��߶�ϸ��
		uint32_t InSize);//�������

	CMeshComponent* CreatePipeMeshComponent(
		float InTopRadius,   //����Բ���嶥���뾶
		float InBottomRadius,//����Բ����ײ��뾶
		float InHeight,//����Բ����߶�
		float InThickness,//����Բ������
		uint32_t InAxialSubdivision,  //��ϸ��
		uint32_t InHeightSubdivision);//��߶�ϸ��

	CMeshComponent* CreateTorusMeshComponent(
		float InRadius,	//Բ���뾶
		float InSectionRadius, //Բ������뾶
		uint32_t InAxialSubdivision,//Բ������ϸ��
		uint32_t InHeightSubdivision);//Բ���߶�ϸ��

	CMeshComponent* CreateMeshComponent(string& InPath);

	//��ȡ��Ⱦ����
	FRenderingPipeline& GetRenderingPipeline() { return RenderingPipeline; }


protected:
	//��Ⱦ����
	FRenderingPipeline RenderingPipeline;

};
