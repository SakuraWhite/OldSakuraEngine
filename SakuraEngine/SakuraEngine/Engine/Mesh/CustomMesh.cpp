#include "Core/MeshType.h"
#include "CustomMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/CustomMeshComponent.h"

GCustomMesh::GCustomMesh()
{
	//构造模型组件  对象构建模型(非生成)
	SetMeshComponent(ConstructionObject<CCustomMeshComponent>());
}

void GCustomMesh::Init()
{
	Super::Init();//执行父类
}


void GCustomMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//执行父类
}


void GCustomMesh::CreateMesh(string& InPath)
{
	//使用构造函数来构造mesh
	CREATE_RENDER_DATA(CCustomMeshComponent, InPath);
	//SetMeshComponent(GetMeshManage()->CreateMeshComponent(InPath));
}
