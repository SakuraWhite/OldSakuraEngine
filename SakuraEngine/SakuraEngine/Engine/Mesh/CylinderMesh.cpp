#include "Core/MeshType.h"
#include "CylinderMesh.h"

void CCylinderMesh::Init()
{
	Super::Init();
}

void CCylinderMesh::BuildMesh(const FMeshRenderingData* InRenderIngData)
{
	Super::BuildMesh(InRenderIngData);
}


void CCylinderMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void CCylinderMesh::CreateMesh(FMeshRenderingData& MeshData, float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	//�뾶���:����Ӷ������ײ��İ뾶��.�����Բ����,�Ǿ�Ϊ0��Ϊ�˼�����խ�¿�����Ͽ���խ�����
	float RadiusInterval = (InTopRadius - InBottomRadius) / InHeightSubdivision;//�����뾶��ȥ�ײ��뾶��Ȼ����Ը߶�ϸ�֣�������ϵ��µĿ�Ȳ��ֵ
	//�߶ȼ��:����Բ�������ϸ�ֵļ����
	float HeightInterval = InHeight / InHeightSubdivision; //Բ����ĸ߶ȳ���ϸ�֡�
	//����
	float Betavalue = XM_2PI / (float)InAxialSubdivision;//2�����Ե�ǰ����ϸ��
	//��������Բ���嶥��
	for (uint32_t i = 0; i < InHeightSubdivision; ++i )//�ȹ����߶�  ʹ�õ��Ǹ߶�ϸ��
	{
		float Y = (0.5f * InHeight) - HeightInterval * i;//����Y��
		float Radius = InTopRadius + i * RadiusInterval;//����뾶

		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)//�����뾶  ʹ�õ�������
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * cosf(j * Betavalue),//����X��  r * COS�� * i  �뾶����COS�Ļ��ȳ���i
					Y,
					Radius * sinf(j * Betavalue)),//����Z�� cos(��/2 - ��) * i  = R * sin�� * i    ԭ����COS��2��֮����ȥ���ȣ���Ϊ���Ǻ����Ĺ�ʽ�ɵ�ͬ�ڰ뾶����sin�Ļ���
				XMFLOAT4(Colors::Red)));
		}
	}

	//����ģ����
	float VertexCircleNum = InAxialSubdivision + 1;//��תһȦ�Ķ�������
	//����Բ������Χ
	for (uint32_t i = 0; i < InHeightSubdivision; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			// ����Զ�������
			//������1
			//MeshData.IndexData.push_back(i * VertexCircleNum + j);
			//MeshData.IndexData.push_back((i + 1 )* VertexCircleNum + j);
			//MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j+1);

			////������2
			//MeshData.IndexData.push_back(i * VertexCircleNum + j);
			//MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			//MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);
			
			//�����ı���
			// ���߳����Լ�
			//������1
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);// (i + 1) * n + j + 1
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j); //(i + 1) * n + j
			MeshData.IndexData.push_back(i * VertexCircleNum + j);  //i * n + j
			//����������2
			MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);//i * n + j + 1
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);// (i + 1) * n + j + 1
			MeshData.IndexData.push_back(i * VertexCircleNum + j);//i * n + j

		}
	}


	//������������
	{
		uint32_t Index = MeshData.VertexData.size();//Բ���嶥����ʼ��
		float Y = 0.5f * InHeight; //���Y��
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InTopRadius * cosf(i * Betavalue),//����X��  r * COS�� * i   �뾶����COS�Ļ��ȳ���i
					Y,
					InTopRadius * sinf(i * Betavalue)),//����Z�� cos(��/2 - ��) * i = R * sin��  * i   ԭ����COS��2��֮����ȥ���ȣ���Ϊ���Ǻ����Ĺ�ʽ�ɵ�ͬ�ڰ뾶����sin�Ļ��ȳ���i
				XMFLOAT4(Colors::Red)));
		}
		//Բ���嶥�������ĵ�
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f),XMFLOAT4(Colors::Red)));
	
		//���ƶ�����
		float Cpoint = MeshData.VertexData.size() - 1;//���ƶ���Բ�����ĵ�
		for (uint32_t i = 0; i < InAxialSubdivision; ++i)
		{
			//����������
			MeshData.IndexData.push_back(Cpoint);
			MeshData.IndexData.push_back(Index + i + 1);
			MeshData.IndexData.push_back(Index + i);

		}
	}
	//�����ײ�
	{

		uint32_t Index = MeshData.VertexData.size();//Բ����ײ���ʼ��
		float Y = -0.5f * InHeight; //���Y�� �ײ��Ǹ���
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InBottomRadius * cosf(i * Betavalue),//����X��  r * COS�� * i   �뾶����COS�Ļ��ȳ���i
					Y,
					InBottomRadius * sinf(i * Betavalue)),//����Z�� cos(��/2 - ��) * i = R * sin��  * i   ԭ����COS��2��֮����ȥ���ȣ���Ϊ���Ǻ����Ĺ�ʽ�ɵ�ͬ�ڰ뾶����sin�Ļ��ȳ���i
				XMFLOAT4(Colors::Red)));
		}


		//Բ���嶥�������ĵ�
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f), XMFLOAT4(Colors::Red)));

		//���ƶ�����
		float Cpoint = MeshData.VertexData.size() - 1;//���ƶ���Բ�����ĵ�
		for (uint32_t i = 0; i < InAxialSubdivision; ++i)
		{
			//����������
			MeshData.IndexData.push_back(Cpoint);
			MeshData.IndexData.push_back(Index + i);
			MeshData.IndexData.push_back(Index + i + 1);

		}
	}
}


