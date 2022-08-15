#include "SphereMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CSphereMeshComponent::CSphereMeshComponent()
{

}

void CSphereMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse)
{

	//��������ļ���¦�
	float ThetaValue = XM_2PI / InHeightSubdivision;//�����Ǽ�����Ǧ� �߶���
	float BetaValue = XM_PI / InAxialSubdivision; //���������Ǧ� ����
	//������ʼ��
	MeshData.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, InRadius, 0.f),		//����뾶
		XMFLOAT4(Colors::White),		//������ɫ
		bReverse ? XMFLOAT3(0.0f, -1.0f, 0.0f) : XMFLOAT3(0.0f, 1.0f, 0.0f),	//���㷨��  ��ֱ��Y�� �Ƿ�ת����
		XMFLOAT2(0.5f, 0.f)));		////��Ӷ�������UV����


	//��ʼ���㹹�����嶥��
	for (uint32_t i = 1; i < InAxialSubdivision; ++i)
	{
		float Beta = i * BetaValue;  //������ ������ת360��  ÿ��תһ�ܾ�������һ��
		//����������ĸ߶���
		for (uint32_t j = 0; j <= InHeightSubdivision; ++j)//�жϸ߶���
		{
			float Theta = j * ThetaValue; //�����߶��� ����������ת180�� 
			//��������ת��Ϊ�ѿ�������
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InRadius * sinf(Beta) * cosf(Theta),//����X�� x = r * sin�� * cos�� r����뾶InRadius
					InRadius * cosf(Beta), //����Y�� y = r * cos��
					InRadius * sinf(Beta) * sinf(Theta)),//����Z�� z = r * sin�� * sin��
				XMFLOAT4(Colors::White))); //������ɫ

			int TopIndex = MeshData.VertexData.size() - 1; //��ȡ����
			FVertex& InVertex = MeshData.VertexData[TopIndex];//��ȡ����

			//����λ��/����
			XMVECTOR Pos = XMLoadFloat3(&InVertex.Position);//����תһ�½ṹ 
			XMStoreFloat3(&InVertex.Normal, XMVector3Normalize(Pos));//�ڶ������ɷ���

			//�������� U���������
			InVertex.UTangent.x = -InRadius * sinf(Beta) * sinf(Theta); //���ߵ�X�� ����ģ�͵�Z��ķ�����
			InVertex.UTangent.y = 0.f;//����û�и߶Ƚ����ڱ��� ����Y�����0
			InVertex.UTangent.z = InRadius * sinf(Beta) * cosf(Theta); //���ߵ�Z�����ģ�͵�X��

			//����UV
			InVertex.TexCoord.x = Theta / XM_2PI; 
			InVertex.TexCoord.y = Beta / XM_PI;

			//��������
			XMVECTOR Tangent = XMLoadFloat3(&InVertex.UTangent);//ת���ṹ
			XMStoreFloat3(&InVertex.UTangent, XMVector3Normalize(Tangent));//�ٶ�����������


		}
	}

	//��������ײ���ֹ��
	MeshData.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, -InRadius, 0.f),   //�ײ��뾶
		XMFLOAT4(Colors::White),		 //������ɫ
		bReverse ? XMFLOAT3(0.0f, 1.0f, 0.0f) : XMFLOAT3(0.0f, -1.0f, 0.0f),	 //�ײ��Ķ��㷨��  ��ֱ��-Y��    �ж��Ƿ�ת����
		XMFLOAT2(0.f, 0.5f)));			 //��ӵײ�����UV����


	//���Ʊ���
	for (uint32_t Index = 0; Index <= InAxialSubdivision; ++Index)//��������ʱ�� ��ʱ�뷨��������
	{
		if (bReverse)//�Ƿ�ת����
		{
			MeshData.IndexData.push_back(0);//��һ��һ����0 
			MeshData.IndexData.push_back(Index);
			MeshData.IndexData.push_back(Index + 1);
		}
		else
		{
			MeshData.IndexData.push_back(0);//��һ��һ����0 
			MeshData.IndexData.push_back(Index + 1);//2
			MeshData.IndexData.push_back(Index);//1    0-2-1��ɵ�������
		}

	}


	float BaseIndex = 1;//��ʼ���� 1
	float VertexCircleNum = InAxialSubdivision + 1;//��תһȦ�Ķ�������


	 //�������м�Ļ���
	for (size_t i = 0; i < InHeightSubdivision - 1; ++i) //����ĸ߶����2�Ǽ�ȥ�ϱ���
	{
		for (size_t j = 0; j < InAxialSubdivision; ++j)//��ʼ���ƻ�Ȧ
		{
			
			DrawQuadrilateral(MeshData,//����ģ������
				GetQuadrilateralDrawPointTypeA(j, i, InAxialSubdivision, 1),//���м价���㷨��
				bReverse);//�Ƿ�ת����

			////�����ı���
			////������1
			//MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			//MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			//MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			////������2
			//MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			//MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);

		}

	}




	//�����ϼ�  �ϼ�����ʼ�������ǵ�ǰ�Ķ�������ȥ1
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;//�ϼ�����ʼ�������ǵ�ǰ�Ķ�������ȥ1
	BaseIndex = SouthBaseIndex - VertexCircleNum;//�ϼ���Index������͵��� ��ǰ���ϼ���ʼ���ȥ�����ת���һȦ�����ټ�1������1��Ϊ����Index��������������
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//��������ʱ�� ��ʱ�뷨��������
	{
		if (bReverse)
		{
			MeshData.IndexData.push_back(BaseIndex + Index);
			MeshData.IndexData.push_back(SouthBaseIndex);
			MeshData.IndexData.push_back(BaseIndex + Index + 1);
		}
		else
		{
			MeshData.IndexData.push_back(SouthBaseIndex);//��һ��һ����0 
			MeshData.IndexData.push_back(BaseIndex + Index);//2
			MeshData.IndexData.push_back(BaseIndex + Index + 1);//1    0-2-1��ɵ�������
		}

	}


	//����
	//MeshData.IndexData[SouthBaseIndex - 2];
	//MeshData.IndexData[SouthBaseIndex - 3];

}


//������ϣkey
void CSphereMeshComponent::BuildKey(size_t& OutHashKey, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse)
{
	std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 8;
	OutHashKey += FloatHash(InRadius);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
}

