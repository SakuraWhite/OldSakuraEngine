#include "TorusMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "TorusMesh.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/TorusMeshComponent.h"

GTorusMesh::GTorusMesh()
{
	//����ģ�����  ���󹹽�ģ��(������)
	SetMeshComponent(ConstructionObject<CTorusMeshComponent>());
}

void GTorusMesh::Init()
{

}

void GTorusMesh::Draw(float DeltaTime)
{

}

void GTorusMesh::CreateMesh(float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	//ʹ�ù��캯��������mesh
	CREATE_RENDER_DATA(CTorusMeshComponent, InRadius, InSectionRadius, InAxialSubdivision, InHeightSubdivision);

	//SetMeshComponent(GetMeshManage()->CreateTorusMeshComponent(InRadius, InSectionRadius, InAxialSubdivision,InHeightSubdivision));
}
