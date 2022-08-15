#include "PyramidMesh.h"
#include "Core/MeshType.h"
#include "PlaneMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/PyramidMeshComponent.h"
GPyramidMesh::GPyramidMesh()
{
	//构造模型组件  对象构建模型(非生成)
	SetMeshComponent(ConstructionObject<CPyramidMeshComponent>());
}
void GPyramidMesh::Init()
{
	Super::Init();


}

void GPyramidMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GPyramidMesh::CreateMesh(EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize)
{
	//使用构造函数来构造mesh
	CREATE_RENDER_DATA(CPyramidMeshComponent, InPyramidNumberSidesType, InHeightSubdivide, InSize);

	//SetMeshComponent(GetMeshManage()->CreatePyramidMeshComponent(InPyramidNumberSidesType, InHeightSubdivide, InSize));
}
