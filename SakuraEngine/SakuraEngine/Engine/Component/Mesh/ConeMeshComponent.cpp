#include "ConeMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CConeMeshComponent::CConeMeshComponent()
{

}

void CConeMeshComponent::CreateMesh(
	FMeshRenderingData& MeshData, 
	float InRadius, 
	float InHeight, 
	uint32_t InAxialSubdivision, 
	uint32_t InHeightSubdivision)
{
	//�뾶���
	float RadiusInterval = -InRadius / (float)InHeightSubdivision;//�����뾶��ȥ�ײ��뾶��Ȼ����Ը߶�ϸ�֣�������ϵ��µĿ�Ȳ��ֵ
	//�߶ȼ��
	float HeightInterval = InHeight / (float)InHeightSubdivision;//Բ����ĸ߶ȳ���ϸ�֡�
	//����
	float BetaValue = XM_2PI / (float)InAxialSubdivision;//2�����Ե�ǰ����ϸ��


	//������������
		//����Բ׶�������Ķ���
	MeshData.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, 0.5f * InHeight, 0.f), 
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, 1.f, 0.f)));

	//
	uint32_t Index = MeshData.VertexData.size();//�����嶥����ʼ��
	for (uint32_t i = 0; i < InHeightSubdivision; ++i)
	{
		float Y = 0.5f * InHeight - HeightInterval * i; //���Y��  Բ׶��ֻ�еײ�����Բ
		float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			float c = cosf(j * BetaValue);
			float s = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * cosf(j * BetaValue),//����X��
					Y,
					Radius * sinf(j * BetaValue)),//����Z�� 
				XMFLOAT4(Colors::White)));

			//������������
			FVertex& Vertex = MeshData.VertexData[MeshData.VertexData.size() - 1];
			Vertex.UTangent = XMFLOAT3(-s, Y, c);

			float dr = InRadius;
			XMFLOAT3 bitangent(dr * c, -InHeight, dr * s);
			//��������
			XMVECTOR T = XMLoadFloat3(&Vertex.UTangent);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = -XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&Vertex.Normal, N);
		}
	}
	//Բ׶����ײ������ĵ�
	MeshData.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, -0.5f * InHeight, 0.f), 
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, -1.f, 0.f)));

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
	{
		int i = InHeightSubdivision;
		float Y = 0.5f * InHeight - HeightInterval * i;
		float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			float c = cosf(j * BetaValue);
			float s = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * c,//x
					Y,//y
					Radius * s), //z
				XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f)));
		}
	}

	//�����ϼ�
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;//�ϼ�����ʼ�������ǵ�ǰ�Ķ�������ȥ1
	BaseIndex = SouthBaseIndex - VertexCircleNum;//�ϼ���Index������͵��� ��ǰ���ϼ���ʼ���ȥ�����ת���һȦ�����ټ�1������1��Ϊ����Index��������������
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//��������ʱ�� ��ʱ�뷨��������
	{

		MeshData.IndexData.push_back(SouthBaseIndex);//��һ��һ����0 
		MeshData.IndexData.push_back(BaseIndex + Index);//2
		MeshData.IndexData.push_back(BaseIndex + Index + 1); //1    0-2-1��ɵ�������
	}

}

//������ϣkey
void CConeMeshComponent::BuildKey(size_t& OutHashKey, float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 2;
	OutHashKey += FloatHash(InRadius);
	OutHashKey += FloatHash(InHeight);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
}

