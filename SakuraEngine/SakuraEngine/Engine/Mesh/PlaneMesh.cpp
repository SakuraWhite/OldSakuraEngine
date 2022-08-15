#include "Core/MeshType.h"
#include "PlaneMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/PlaneMeshComponent.h"

GPlaneMesh::GPlaneMesh()
{
	//构造模型组件  对象构建模型(非生成)
	SetMeshComponent(ConstructionObject<CPlaneMeshComponent>());
}

void GPlaneMesh::Init()
{
	Super::Init();


}


void GPlaneMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//执行父类
}

void GPlaneMesh::CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	//使用构造函数来构造mesh
	CREATE_RENDER_DATA(CPlaneMeshComponent, InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);

	//SetMeshComponent(GetMeshManage()->CreatePlaneMeshComponent(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide));

}
