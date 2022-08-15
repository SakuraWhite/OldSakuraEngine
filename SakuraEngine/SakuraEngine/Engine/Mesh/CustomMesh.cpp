#include "Core/MeshType.h"
#include "CustomMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/CustomMeshComponent.h"

GCustomMesh::GCustomMesh()
{
	//����ģ�����  ���󹹽�ģ��(������)
	SetMeshComponent(ConstructionObject<CCustomMeshComponent>());
}

void GCustomMesh::Init()
{
	Super::Init();//ִ�и���
}


void GCustomMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}


void GCustomMesh::CreateMesh(string& InPath)
{
	//ʹ�ù��캯��������mesh
	CREATE_RENDER_DATA(CCustomMeshComponent, InPath);
	//SetMeshComponent(GetMeshManage()->CreateMeshComponent(InPath));
}
