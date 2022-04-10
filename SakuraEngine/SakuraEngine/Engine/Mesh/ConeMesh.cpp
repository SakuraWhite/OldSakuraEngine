#include "Core/MeshType.h"
#include "ConeMesh.h"

void GConeMesh::Init()
{
	Super::Init();//ִ�и���
}

void GConeMesh::BuildMesh(const FMeshRenderingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//ִ�и���

}

void GConeMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}

void GConeMesh::CreateMesh(FMeshRenderingData& MeshData,//������ǰMesh�ṹ����
	float InRadius,  //Բ׶��ײ��뾶  ��ΪԲ׶�嶥���ǵ������� ������ֻ�еײ��а뾶
	float InHeight,  //Բ׶��߶�
	uint32_t InAxialSubdivision,//��ϸ��
	uint32_t InHeightSubdivision) //�߶�ϸ��
{

	//�뾶���
	float RadiusInterVal = -InRadius / (float)InHeightSubdivision;//�����뾶��ȥ�ײ��뾶��Ȼ����Ը߶�ϸ�֣�������ϵ��µĿ�Ȳ��ֵ
	//�߶ȼ��
	float HeightInterVal = InHeight / (float)InHeightSubdivision;//Բ����ĸ߶ȳ���ϸ�֡�
	//����
	float Betavalue = XM_2PI / (float)InAxialSubdivision;//2�����Ե�ǰ����ϸ��


	//������������
		//����Բ׶�������Ķ���
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, 0.5f * InHeight, 0.f), XMFLOAT4(Colors::White)));

		uint32_t Index = MeshData.VertexData.size();//�����嶥����ʼ��
		for (uint32_t i = 0; i < InHeightSubdivision; ++i)
		{
			float Y = 0.5f * InHeight - HeightInterVal * i; //���Y��  Բ׶��ֻ�еײ�����Բ
			float Radius =  i * RadiusInterVal;
			for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
			{
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						Radius * cosf(j * Betavalue),//����X��
						Y,
						Radius * sinf(j * Betavalue)),//����Z�� 
					XMFLOAT4(Colors::Red)));
			}
		}
	//Բ׶����ײ������ĵ�
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, -0.5f * InHeight, 0.f), XMFLOAT4(Colors::White)));

	//���ƶ����� Index
	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		//����������
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);

	}
	//����Բ׶�ײ�
	float BaseIndex = 1;//��ʼ���� 1
	float VertexCircleNum = InAxialSubdivision + 1;//��תһȦ�Ķ�������
	//Բ׶����
	for (uint32_t i = 0; i < InHeightSubdivision - 1; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			//�����ı���
			//������1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//������2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}


	//�����ϼ�  �ϼ�����ʼ�������ǵ�ǰ�Ķ�������ȥ1
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;//�ϼ�����ʼ�������ǵ�ǰ�Ķ�������ȥ1
	BaseIndex = SouthBaseIndex - VertexCircleNum;//�ϼ���Index������͵��� ��ǰ���ϼ���ʼ���ȥ�����ת���һȦ�����ټ�1������1��Ϊ����Index��������������
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//��������ʱ�� ��ʱ�뷨��������
	{

		MeshData.IndexData.push_back(SouthBaseIndex);//��һ��һ����0 
		MeshData.IndexData.push_back(BaseIndex + Index);//2
		MeshData.IndexData.push_back(BaseIndex + Index + 1); //1    0-2-1��ɵ�������
	}

}