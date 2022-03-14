#include "Core/MeshType.h"
#include "PlaneMesh.h"

void CPlaneMesh::Init()
{
	Super::Init();//ִ�и���
}

void CPlaneMesh::BuildMesh(const FMeshRendingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//ִ�и���

}

void CPlaneMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}

CPlaneMesh* CPlaneMesh::CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	FMeshRendingData MeshData;//������ǰMesh�ṹ����
	auto SubdivideValue =  [ & ] ( float InValue, uint32_t  InSubdivideValue)->float  //�ж�ϸ��ֵ ��ֹ���㵼�³���
	{
		if (InSubdivideValue <= 1)
		{
			return InValue;
		}

		return InValue / ((float) InSubdivideValue - 1);
	};

	//����ģ�����ĵ�
	float CHeight = 0.5f * InHeight;//����ƽ��߶����ĵ�
	float CWidth = 0.5f * InWidth;//����ƽ��������ĵ�
	//����ģ�͵�ϸ��
	float HeightSubdivideValie = SubdivideValue(InHeight,InHeightSubdivide );//ƽ��ĸ߶�ϸ�ּ���
	float WidthSubdivideValie = SubdivideValue(InWidth, InWidthSubdivide);//ƽ��Ŀ���ϸ�ּ���

	//��������
	for (uint32_t i = 0; i < InHeightSubdivide; ++i)
	{
		float Z = CHeight - i * HeightSubdivideValie;//����Z��

		for (uint32_t j = 0; j < InWidthSubdivide; ++j)
		{
			float X = CWidth - j * WidthSubdivideValie;//����Z�� �������Ʒ���
			MeshData.VertexDeta.push_back(FVertex(
				XMFLOAT3(
					X, //x
					0.f, //y
					Z),//z
				XMFLOAT4(Colors::Red)));

		}
	}


	//������Index
	for (uint32_t i = 0; i < InHeightSubdivide - 1; ++i)
	{
		for (uint32_t j = 0; j < InWidthSubdivide - 1; ++j)
		{
			//�����ı���
			/*���߷�ת
			//������1
				MeshData.IndexDeta.push_back(i * InWidthSubdivide + j);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j);
			//������2
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j + 1);
			*/
			//������1
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j);
			//������2
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j);


		}
	}



	CPlaneMesh* PlaneMesh =new CPlaneMesh;
	PlaneMesh->BuildMesh(&MeshData);//�����������Bulidһ��

	PlaneMesh->Init();//ȷ��������֮���ʼ��

	return PlaneMesh;
}