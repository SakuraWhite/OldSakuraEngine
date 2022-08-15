#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Component/TransformationComponent.h"
#include "Material/Material.h"
#include "../../Component/Mesh/Core/MeshComponent.h"

GMesh::GMesh()
	:GActorObject()
{
}
void GMesh::Init()
{

}



void GMesh::PreDraw(float DeltaTime)
{

}


void GMesh::Draw(float DeltaTime)
{

}


void GMesh::PostDraw(float DeltaTime)
{

}

void GMesh::SetPosition(const XMFLOAT3& InNewPosition)
{
	Super::SetPosition(InNewPosition);

	//��Ϊ��δ�����������ʱ��ôд
	GetMeshComponent()->SetPosition(InNewPosition);
}

void GMesh::SetRotation(const fvector_3d& InRotation)
{
	Super::SetRotation(InRotation);

	//��Ϊ��δ�����������ʱ��ôд
	GetMeshComponent()->SetRotation(InRotation);
}

void GMesh::SetScale(const fvector_3d& InNewScale)
{
	Super::SetScale(InNewScale);

	//��Ϊ��δ�����������ʱ��ôд
	GetMeshComponent()->SetScale(InNewScale);
}

UINT GMesh::GetMaterialNum() const
{
	return MeshComponent->GetMaterialNum();
}


vector<CMaterial*>* GMesh::GetMaterials()
{
	return MeshComponent->GetMaterials();
}

void GMesh::SetMeshComponent(CMeshComponent* InMeshComponent)
{
	MeshComponent = InMeshComponent;
}

void GMesh::SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType)
{
	MeshComponent->SetMeshRenderLayerType(InRenderLayerType);//ͨ��mesh���ȥ���mesh��Ⱦ�㼶����
}
