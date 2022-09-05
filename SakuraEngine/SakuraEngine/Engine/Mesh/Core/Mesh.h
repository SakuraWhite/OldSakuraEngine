#pragma once//��ֹ�ظ�����
#include "../../Rendering/Core/Rendering.h"
#include "../../Actor/Core/ActorObject.h"
#include "MeshType.h"
#include "../../Interface/DirectXDeviceInterfece.h"

class CTransformationComponent;
class CMeshComponent;
class CMaterial;
enum EMeshRenderLayerType;

class GMesh
	:public GActorObject,    //�̳�����GActorObject 
	public IRenderingInterface,//�����ǽӿ�
	public IDirectXDeviceInterfece
{
	typedef GActorObject Super;

	CVARIABLE()  //�����
		CTransformationComponent* TransformationComponent;//����ϣ��ģ�������ܹ��ƶ� ����Ҫ�������

	CVARIABLE()
		CMeshComponent* MeshComponent;

public:
	GMesh();//��ʼ��Index��Vertex��һЩ����������

	virtual void Init(); //Rendeing.h��ĵ���Ⱦ��ʼ��

	virtual void PreDraw(float DeltaTime); //��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	virtual void Draw(float DeltaTime);  //��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	virtual void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw

	//static CMeshComponent*CreateMesh(const FMeshRenderingData *InRenderingData); //��̬�ṹ�������������� ��̬��ʽ������Mesh

public:
	virtual void SetPosition(const XMFLOAT3& InNewPosition);  //����λ��
	virtual void SetRotation(const fvector_3d& InRotation);   //������ת	
	virtual void SetScale(const fvector_3d& InNewScale);      //��������
public:
	virtual CMeshComponent* GetMeshComponent() { return MeshComponent; }//�ӿ� ��ȡ���

	//ʹ��ģ��ķ�ʽ������
	template<class T>
	T* GetMeshComponent()
	{
		return dynamic_cast<T*>(MeshComponent);//ת����ָ������
	}
public:
	//����ģ����Ⱦ�㼶����
	virtual void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);

protected:
	//�������
	virtual void SetMeshComponent(CMeshComponent* InMeshComponent); 

public:
	//��ȡ���ʵ����� 
	UINT GetMaterialNum()const;
	//��ȡ����
	vector<CMaterial*>* GetMaterials();


};