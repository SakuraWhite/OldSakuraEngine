#include "BoxMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/BoxMeshComponent.h"

GBoxMesh::GBoxMesh()
{

	//构造模型组件  对象构建模型(非生成)
	SetMeshComponent(ConstructionObject<CBoxMeshComponent>());

}

void GBoxMesh::Init()
{
	Super::Init();//执行父类
}



void GBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//执行父类
}


void GBoxMesh::CreateMesh( float InHeight, float InWidth, float InDepth)
{
	//使用构造函数来构造mesh
	CREATE_RENDER_DATA(CBoxMeshComponent, InHeight, InWidth, InDepth);
	//SetMeshComponent(GetMeshManage()->CreateBoxMeshComponent(InHeight, InWidth, InDepth));
}
