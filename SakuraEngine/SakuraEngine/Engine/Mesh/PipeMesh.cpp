#include "PipeMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/PipeMeshComponent.h"

GPipeMesh::GPipeMesh()
{
	//构造模型组件  对象构建模型(非生成)
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
	//使用构造函数来构造mesh
	CREATE_RENDER_DATA(CPipeMeshComponent, 
		InTopRadius, 
		InBottomRadius, 
		InHeight, 
		InThickness, 
		InAxialSubdivision, 
		InHeightSubdivision);

	//SetMeshComponent(GetMeshManage()->CreatePipeMeshComponent(InTopRadius, InBottomRadius, InHeight, InThickness, InAxialSubdivision, InHeightSubdivision));
}
