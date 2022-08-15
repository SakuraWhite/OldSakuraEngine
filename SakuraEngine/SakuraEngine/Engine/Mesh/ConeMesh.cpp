#include "Core/MeshType.h"
#include "ConeMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Component/Mesh/ConeMeshComponent.h"
#include "../Core/Construction/MacroConstruction.h"

GConeMesh::GConeMesh()
{
	//����ģ�����  ���󹹽�ģ��(������)
	SetMeshComponent(ConstructionObject<CConeMeshComponent>());
}

void GConeMesh::Init()
{
	Super::Init();//ִ�и���
}


void GConeMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}

void GConeMesh::CreateMesh(
	float InRadius,  //Բ׶��ײ��뾶  ��ΪԲ׶�嶥���ǵ������� ������ֻ�еײ��а뾶
	float InHeight,  //Բ׶��߶�
	uint32_t InAxialSubdivision,//��ϸ��
	uint32_t InHeightSubdivision) //�߶�ϸ��
{

	//ʹ�ù��캯��������mesh
	CREATE_RENDER_DATA(CConeMeshComponent, InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
	//SetMeshComponent(GetMeshManage()->CreateConeMeshComponent(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision));
}