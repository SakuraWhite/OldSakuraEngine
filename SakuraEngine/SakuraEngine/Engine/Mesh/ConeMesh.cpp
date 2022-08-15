#include "Core/MeshType.h"
#include "ConeMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Component/Mesh/ConeMeshComponent.h"
#include "../Core/Construction/MacroConstruction.h"

GConeMesh::GConeMesh()
{
	//构造模型组件  对象构建模型(非生成)
	SetMeshComponent(ConstructionObject<CConeMeshComponent>());
}

void GConeMesh::Init()
{
	Super::Init();//执行父类
}


void GConeMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//执行父类
}

void GConeMesh::CreateMesh(
	float InRadius,  //圆锥体底部半径  因为圆锥体顶部是单个顶点 ，所以只有底部有半径
	float InHeight,  //圆锥体高度
	uint32_t InAxialSubdivision,//轴细分
	uint32_t InHeightSubdivision) //高度细分
{

	//使用构造函数来构造mesh
	CREATE_RENDER_DATA(CConeMeshComponent, InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
	//SetMeshComponent(GetMeshManage()->CreateConeMeshComponent(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision));
}