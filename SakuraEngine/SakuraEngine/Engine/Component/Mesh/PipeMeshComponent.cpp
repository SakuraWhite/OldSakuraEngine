#include "PipeMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CPipeMeshComponent::CPipeMeshComponent()
{

}

void CPipeMeshComponent::BuildRadiusPoint(
	FMeshRenderingData& MeshData,
	float HeightInterval,
	float InTopRadius,
	float InBottomRadius,
	float BetaValue,
	float RadiusInterval,
	float InHeight,
	float InThickness,
	uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	//����������Ȧ
	for (uint32_t i = 0; i < InHeightSubdivision + 1; ++i)
	{
		float Y = 0.5f * InHeight - HeightInterval * i;

		//���������Ȧ�뾶
		float OuterRadius = InTopRadius + i * RadiusInterval;
		float InnerRadius = math_libray::Clamp((OuterRadius - 0.1f) - InThickness, 0.f, OuterRadius);
		
		for (size_t j = 0; j <= InAxialSubdivision; ++j)
		{
			float BetaValueCos = cosf(j * BetaValue);
			float BetaValueSin = sinf(j * BetaValue);

			//��Ȧ��
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					OuterRadius * BetaValueCos,//x
					Y,//y
					OuterRadius * BetaValueSin), //z
				XMFLOAT4(Colors::White)));

			{
				FVertex& MyVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

				MyVertex.UTangent = XMFLOAT3(-BetaValueSin, 0.0f, BetaValueCos);

				float D = InBottomRadius - InTopRadius;
				XMFLOAT3 Bitangent(D * BetaValueCos, -InHeight, D * BetaValueSin);

				XMVECTOR T = XMLoadFloat3(&MyVertex.UTangent);
				XMVECTOR B = XMLoadFloat3(&Bitangent);
				XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
				XMStoreFloat3(&MyVertex.Normal, N);
			}

			//������Ȧ
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InnerRadius * BetaValueCos,//x
					Y,//y
					InnerRadius * BetaValueSin), //z
				XMFLOAT4(Colors::White)));

			{
				FVertex& MyVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

				MyVertex.UTangent = XMFLOAT3(-BetaValueSin, 0.0f, BetaValueCos);

				float D = InBottomRadius - InTopRadius;
				XMFLOAT3 Bitangent(D * BetaValueCos, -InHeight, D * BetaValueSin);

				XMVECTOR T = XMLoadFloat3(&MyVertex.UTangent);
				XMVECTOR B = XMLoadFloat3(&Bitangent);
				XMVECTOR N = -XMVector3Normalize(XMVector3Cross(T, B));
				XMStoreFloat3(&MyVertex.Normal, N);
			}
		}
	}
}

void CPipeMeshComponent::CreateMesh(
	FMeshRenderingData& MeshData, 
	float InTopRadius,
	float InBottomRadius, 
	float InHeight,
	float InThickness,
	uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	//�뾶���
	float RadiusInterval = (InTopRadius - InBottomRadius) / InHeightSubdivision;
	//�߶ȼ��
	float HeightInterval = InHeight / InHeightSubdivision;

	float BetaValue = XM_2PI / (float)InAxialSubdivision;

	BuildRadiusPoint(
		MeshData,
		HeightInterval,
		InTopRadius,
		InBottomRadius,
		BetaValue,
		RadiusInterval,
		InHeight,
		InThickness, 
		InAxialSubdivision,
		InHeightSubdivision);

	float VertexCircleNum = InAxialSubdivision;

	//������Χ
	for (uint32_t i = 0; i < InHeightSubdivision + 1; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			int OuterStartPoint = j * 2;
			int InnerStartPoint = j * 2 + 1;

			int VC = VertexCircleNum * 2 ;

			//����Ȧ��
			DrawQuadrilateral(
				MeshData,//��ȡ������Ϣ
				GetQuadrilateralDrawPointTypeA(OuterStartPoint, i,VC,2));//�õ�Ȧ���ĸ���
			
			//����Ȧ��
			DrawQuadrilateral(
				MeshData,//��ȡ������Ϣ
				GetQuadrilateralDrawPointTypeA(InnerStartPoint, i,VC,2),true);//�õ�Ȧ���ĸ���																		
		}
	}

	//��������
	if (1)
	{
		for (uint32_t i = 0; i < 1; ++i)
		{
			float Y = 0.5f * InHeight - HeightInterval * i;

			//���������Ȧ�뾶
			float OuterRadius = InTopRadius + i * RadiusInterval;
			float InnerRadius = math_libray::Clamp((OuterRadius - 0.1f) - InThickness, 0.f, OuterRadius);

			for (size_t j = 0; j <= InAxialSubdivision; ++j)
			{
				float BetaValueCos = cosf(j * BetaValue);
				float BetaValueSin = sinf(j * BetaValue);

				//��Ȧ��
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						OuterRadius * BetaValueCos,//x
						Y,//y
						OuterRadius * BetaValueSin), //z
					XMFLOAT4(Colors::White), XMFLOAT3(0.f,1.f,0.f)));

				//������Ȧ
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						InnerRadius * BetaValueCos,//x
						Y,//y
						InnerRadius * BetaValueSin), //z
					XMFLOAT4(Colors::White), XMFLOAT3(0.f, 1.f, 0.f)));
			}
		}
		int BaseIndex = (InHeightSubdivision + 2) * (InAxialSubdivision ) * 2 + 2;
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			int OuterStartPoint = j * 2;
			int InnerStartPoint = j * 2 + 1;

			fvector_4id DrawPoint= {
						BaseIndex+InnerStartPoint,
						BaseIndex+OuterStartPoint,
						BaseIndex+OuterStartPoint + 2,
						BaseIndex+InnerStartPoint + 2 };

			DrawQuadrilateral(MeshData,DrawPoint);
		}
	}

	//�����ײ�
	if (1)
	{
		{
			float Y = 0.5f * InHeight - HeightInterval * InHeightSubdivision;

			//���������Ȧ�뾶
			float OuterRadius = InTopRadius + InHeightSubdivision * RadiusInterval;
			float InnerRadius = math_libray::Clamp((OuterRadius - 0.1f) - InThickness, 0.f, OuterRadius);

			for (size_t j = 0; j <= InAxialSubdivision; ++j)
			{
				float BetaValueCos = cosf(j * BetaValue);
				float BetaValueSin = sinf(j * BetaValue);

				//��Ȧ��
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						OuterRadius * BetaValueCos,//x
						Y,//y
						OuterRadius * BetaValueSin), //z
					XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f)));

				//������Ȧ
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						InnerRadius * BetaValueCos,//x
						Y,//y
						InnerRadius * BetaValueSin), //z
					XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f)));
			}
		}

		int BaseIndex = (InHeightSubdivision + 3) * InAxialSubdivision * 2 + 4;
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			int OuterStartPoint = j * 2;
			int InnerStartPoint = j * 2 + 1;

			fvector_4id DrawPoint = {
						 BaseIndex+InnerStartPoint,
						 BaseIndex+OuterStartPoint,
						 BaseIndex+OuterStartPoint + 2,
						 BaseIndex+InnerStartPoint + 2 };

			DrawQuadrilateral(MeshData, DrawPoint,true);
		}
	}
}

void CPipeMeshComponent::BuildKey(size_t& OutHashKey, float InTopRadius, float InBottomRadius, float InHeight, float InThickness, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 5;
	OutHashKey += FloatHash(InHeight);
	OutHashKey += FloatHash(InBottomRadius);
	OutHashKey += FloatHash(InTopRadius);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
}
