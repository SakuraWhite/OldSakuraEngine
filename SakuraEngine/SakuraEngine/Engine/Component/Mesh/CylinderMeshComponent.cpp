#include "CylinderMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CCylinderMeshComponent::CCylinderMeshComponent()
{

}

void CCylinderMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	//�뾶���:����Ӷ������ײ��İ뾶��.�����Բ����,�Ǿ�Ϊ0��Ϊ�˼�����խ�¿�����Ͽ���խ�����
	float RadiusInterval = (InTopRadius - InBottomRadius) / InHeightSubdivision;//�����뾶��ȥ�ײ��뾶��Ȼ����Ը߶�ϸ�֣�������ϵ��µĿ�Ȳ��ֵ
	//�߶ȼ��:����Բ�������ϸ�ֵļ����
	float HeightInterval = InHeight / InHeightSubdivision; //Բ����ĸ߶ȳ���ϸ�֡�
	//����
	float BetaValue = XM_2PI / (float)InAxialSubdivision;//2�����Ե�ǰ����ϸ��
	//��������Բ���嶥��
	for (uint32_t i = 0; i < InHeightSubdivision + 1; ++i)//�ȹ����߶�  ʹ�õ��Ǹ߶�ϸ��
	{
		float Y = (0.5f * InHeight) - HeightInterval * i;//����Y��
		float Radius = InTopRadius + i * RadiusInterval;//����뾶

		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)//�����뾶  ʹ�õ�������
		{
			float BetaValueCos = cosf(j * BetaValue);
			float BetaValueSin = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * cosf(j * BetaValue),//����X��  r * COS�� * i  �뾶����COS�Ļ��ȳ���i
					Y,
					Radius * sinf(j * BetaValue)),//����Z�� cos(��/2 - ��) * i  = R * sin�� * i    ԭ����COS��2��֮����ȥ���ȣ���Ϊ���Ǻ����Ĺ�ʽ�ɵ�ͬ�ڰ뾶����sin�Ļ���
				XMFLOAT4(Colors::White)));
		
			//ȡ����������
			FVertex& MyVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];
			//����
			MyVertex.UTangent = XMFLOAT3(-BetaValueSin, 0.0f, BetaValueCos);

			float dr = InBottomRadius - InTopRadius;
			XMFLOAT3 Bitangent(dr * BetaValueCos, -InHeight, dr * BetaValueSin);
			//����TBN����
			XMVECTOR T = XMLoadFloat3(&MyVertex.UTangent);
			XMVECTOR B = XMLoadFloat3(&Bitangent);
			XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&MyVertex.Normal, N);
		
		}
	}

	//����ģ����
	float VertexCircleNum = InAxialSubdivision;//��תһȦ�Ķ�������
	//����Բ������Χ
	for (uint32_t i = 0; i < InHeightSubdivision + 1; ++i)
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
	if (1)
	{
		uint32_t Index = MeshData.VertexData.size();//Բ���嶥����ʼ��
		float Y = 0.5f * InHeight; //���Y��
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InTopRadius * cosf(i * BetaValue),//����X��  r * COS�� * i   �뾶����COS�Ļ��ȳ���i
					Y,
					InTopRadius * sinf(i * BetaValue)),//����Z�� cos(��/2 - ��) * i = R * sin��  * i   ԭ����COS��2��֮����ȥ���ȣ���Ϊ���Ǻ����Ĺ�ʽ�ɵ�ͬ�ڰ뾶����sin�Ļ��ȳ���i
				XMFLOAT4(Colors::White),
				XMFLOAT3(0.f, 1.f, 0.f)));
		}

			//Բ���嶥�������ĵ�
			MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f), XMFLOAT4(Colors::White),XMFLOAT3(0.f, 1.f, 0.f)));
			
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
	if (1)
	{
		uint32_t Index = MeshData.VertexData.size();//Բ����ײ���ʼ��
		float Y = -0.5f * InHeight; //���Y�� �ײ��Ǹ���
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InBottomRadius * cosf(i * BetaValue),//����X��  r * COS�� * i   �뾶����COS�Ļ��ȳ���i
					Y,
					InBottomRadius * sinf(i * BetaValue)),//����Z�� cos(��/2 - ��) * i = R * sin��  * i   ԭ����COS��2��֮����ȥ���ȣ���Ϊ���Ǻ����Ĺ�ʽ�ɵ�ͬ�ڰ뾶����sin�Ļ��ȳ���i
				XMFLOAT4(Colors::Red)));
		}


		//Բ���嶥�������ĵ�
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f), XMFLOAT4(Colors::White)));

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


//������ϣkey
void CCylinderMeshComponent::BuildKey(size_t& OutHashKey, float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 4;
	OutHashKey += FloatHash(InHeight);
	OutHashKey += FloatHash(InBottomRadius);
	OutHashKey += FloatHash(InTopRadius);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
}
