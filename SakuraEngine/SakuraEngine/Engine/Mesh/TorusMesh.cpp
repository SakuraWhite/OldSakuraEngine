#include "TorusMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "TorusMesh.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/TorusMeshComponent.h"

GTorusMesh::GTorusMesh()
{
	//构造模型组件  对象构建模型(非生成)
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
	//使用构造函数来构造mesh
	CREATE_RENDER_DATA(CTorusMeshComponent, InRadius, InSectionRadius, InAxialSubdivision, InHeightSubdivision);

	//SetMeshComponent(GetMeshManage()->CreateTorusMeshComponent(InRadius, InSectionRadius, InAxialSubdivision,InHeightSubdivision));
}
