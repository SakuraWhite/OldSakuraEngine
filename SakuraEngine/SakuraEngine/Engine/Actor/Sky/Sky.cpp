#include "Sky.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Component/Mesh/Core/MeshComponentType.h"

GSky::GSky()
	:Super()
{
	SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_BACKGROUND);//��ģ�������ڱ�����

	CreateMesh(2.f, 100, 100, true);//����ģ�Ͱ뾶 ϸ�� Ȼ��ת����
	SetScale(fvector_3d(4000.f));//��������������

	if (CMaterial* InMaterial = (*GetMaterials())[0])
	{
		InMaterial->SetBaseColor(fvector_4d(0.f, 0.f, 0.f, 1.f)); //������ɫ
		InMaterial->SetBaseColor("x1_CubeMap");//������������ͼ
		InMaterial->SetSpecular(fvector_3d(1.f));//���ø߹�
		InMaterial->SetMaterialType(EMaterialType::BaseColor);//���ò�������
	}

}



void GSky::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}
