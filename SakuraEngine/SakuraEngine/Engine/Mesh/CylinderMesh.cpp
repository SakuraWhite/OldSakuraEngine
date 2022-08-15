#include "Core/MeshType.h"
#include "CylinderMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/CylinderMeshComponent.h"

GCylinderMesh::GCylinderMesh()
{
	//构造模型组件  对象构建模型(非生成)
	SetMeshComponent(ConstructionObject<CCylinderMeshComponent>());
}

void GCylinderMesh::Init()
{
	Super::Init();
}

void GCylinderMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void GCylinderMesh::CreateMesh( float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	//使用构造函数来构造mesh
	CREATE_RENDER_DATA(CCylinderMeshComponent, InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);

	//SetMeshComponent(GetMeshManage()->CreateCylinderMeshComponent(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision));
}


