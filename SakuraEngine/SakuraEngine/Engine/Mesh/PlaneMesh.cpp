#include "Core/MeshType.h"
#include "PlaneMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/PlaneMeshComponent.h"

GPlaneMesh::GPlaneMesh()
{
	//����ģ�����  ���󹹽�ģ��(������)
	SetMeshComponent(ConstructionObject<CPlaneMeshComponent>());
}

void GPlaneMesh::Init()
{
	Super::Init();


}


void GPlaneMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}

void GPlaneMesh::CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	//ʹ�ù��캯��������mesh
	CREATE_RENDER_DATA(CPlaneMeshComponent, InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);

	//SetMeshComponent(GetMeshManage()->CreatePlaneMeshComponent(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide));

}
