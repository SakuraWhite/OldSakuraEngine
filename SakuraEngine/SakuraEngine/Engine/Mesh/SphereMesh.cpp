#include "Core/MeshType.h"
#include "SphereMesh.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/SphereMeshComponent.h"
GSphereMesh::GSphereMesh()
{
	//����ģ�����  ���󹹽�ģ��(������)
	SetMeshComponent(ConstructionObject<CSphereMeshComponent>());
}
void GSphereMesh::Init()
{
	Super::Init();//ִ�и���
}


void GSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}

void GSphereMesh::CreateMesh(
	float InRadius, //������ǰ뾶
	uint32_t InAxialSubdivision,//���������ϸ��
	uint32_t InHeightSubdivision,//����ĸ߶�ϸ��
	bool bReverse) //�Ƿ�ת����
{
	//ʹ�ù��캯��������mesh
	CREATE_RENDER_DATA(CSphereMeshComponent, InRadius, InAxialSubdivision, InHeightSubdivision, bReverse);

	//SetMeshComponent(GetMeshManage()->CreateSphereMeshComponent(InRadius, InAxialSubdivision, InHeightSubdivision));

}
