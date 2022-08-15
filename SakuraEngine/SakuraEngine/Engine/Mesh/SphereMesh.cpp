#include "Core/MeshType.h"
#include "SphereMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/SphereMeshComponent.h"
GSphereMesh::GSphereMesh()
{
	//构造模型组件  对象构建模型(非生成)
	SetMeshComponent(ConstructionObject<CSphereMeshComponent>());
}
void GSphereMesh::Init()
{
	Super::Init();//执行父类
}


void GSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//执行父类
}

void GSphereMesh::CreateMesh(
	float InRadius, //球体的是半径
	uint32_t InAxialSubdivision,//球体的轴向细分
	uint32_t InHeightSubdivision,//球体的高度细分
	bool bReverse) //是否反转法线
{
	//使用构造函数来构造mesh
	CREATE_RENDER_DATA(CSphereMeshComponent, InRadius, InAxialSubdivision, InHeightSubdivision, bReverse);

	//SetMeshComponent(GetMeshManage()->CreateSphereMeshComponent(InRadius, InAxialSubdivision, InHeightSubdivision));

}
