#include "PipeMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/PipeMeshComponent.h"

GPipeMesh::GPipeMesh()
{
	//����ģ�����  ���󹹽�ģ��(������)
	SetMeshComponent(ConstructionObject<CPipeMeshComponent>());
}

void GPipeMesh::Init()
{

}

void GPipeMesh::Draw(float DeltaTime)
{

}

void GPipeMesh::CreateMesh(float InTopRadius, float InBottomRadius, float InHeight, float InThickness, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	//ʹ�ù��캯��������mesh
	CREATE_RENDER_DATA(CPipeMeshComponent, 
		InTopRadius, 
		InBottomRadius, 
		InHeight, 
		InThickness, 
		InAxialSubdivision, 
		InHeightSubdivision);

	//SetMeshComponent(GetMeshManage()->CreatePipeMeshComponent(InTopRadius, InBottomRadius, InHeight, InThickness, InAxialSubdivision, InHeightSubdivision));
}
