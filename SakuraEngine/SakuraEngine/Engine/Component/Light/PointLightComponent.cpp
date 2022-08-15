#include "PointLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"
#include "../../Component/Mesh/Core/MeshComponent.h"
#include "../../Mesh/Core/Material/Material.h"

CPointLightComponent::CPointLightComponent()
	:Super() //ִ�и���
{	
	//��ȡģ����Դ(�������ļ����ж�ȡ)
	string MeshPath = "../SakuraEngine/Asset/PointMesh.obj";
	//ͨ��LightMesh��Ϣ ����һ���Զ����ģ��������ƽ�й���(�Դ�ģ��)
	SetLightMesh(GetMeshManage()->CreateMeshComponent(MeshPath));

	//����̫����(�Դ���mesh)Ϊ�߿�ģʽ��ʾ
	if (GetLightMesh())
	{
		
		if (CMaterial *InMaterial =  (*GetLightMesh()->GetMaterials())[0])//��ȡ��0�Ų���
		{
			InMaterial->SetMaterialType(EMaterialType::BaseColor);//���ò�������
			InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);//�����߿�ģʽ��ʾ״̬
			InMaterial->SetBaseColor(fvector_4d(1.0f, 0.6f, 1.0f, 1.0f));//������ɫ
		}
	}

	LightType = ELightType::PointLight	;//��ʼ���ƹ�����Ϊ���Դ
}
