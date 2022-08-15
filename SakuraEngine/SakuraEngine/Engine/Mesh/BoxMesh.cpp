#include "BoxMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/BoxMeshComponent.h"

GBoxMesh::GBoxMesh()
{

	//����ģ�����  ���󹹽�ģ��(������)
	SetMeshComponent(ConstructionObject<CBoxMeshComponent>());

}

void GBoxMesh::Init()
{
	Super::Init();//ִ�и���
}



void GBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}


void GBoxMesh::CreateMesh( float InHeight, float InWidth, float InDepth)
{
	//ʹ�ù��캯��������mesh
	CREATE_RENDER_DATA(CBoxMeshComponent, InHeight, InWidth, InDepth);
	//SetMeshComponent(GetMeshManage()->CreateBoxMeshComponent(InHeight, InWidth, InDepth));
}
