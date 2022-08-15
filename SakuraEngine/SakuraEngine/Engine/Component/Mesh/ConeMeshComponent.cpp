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
	//半径间隔
	float RadiusInterval = -InRadius / (float)InHeightSubdivision;//顶部半径减去底部半径，然后除以高度细分，计算从上到下的宽度差的值
	//高度间隔
	float HeightInterval = InHeight / (float)InHeightSubdivision;//圆柱体的高度除以细分。
	//弧度
	float BetaValue = XM_2PI / (float)InAxialSubdivision;//2Π除以当前的轴细分


	//构建顶部顶点
		//构建圆锥顶部中心顶点
	MeshData.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, 0.5f * InHeight, 0.f), 
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, 1.f, 0.f)));

	//
	uint32_t Index = MeshData.VertexData.size();//顶点体顶部起始点
	for (uint32_t i = 0; i < InHeightSubdivision; ++i)
	{
		float Y = 0.5f * InHeight - HeightInterval * i; //求出Y轴  圆锥体只有底部才有圆
		float Radius = i * RadiusInterval;
		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
		{
			float c = cosf(j * BetaValue);
			float s = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * cosf(j * BetaValue),//计算X轴
					Y,
					Radius * sinf(j * BetaValue)),//计算Z轴 
				XMFLOAT4(Colors::White)));

			//构建数据索引
			FVertex& Vertex = MeshData.VertexData[MeshData.VertexData.size() - 1];
			Vertex.UTangent = XMFLOAT3(-s, Y, c);

			float dr = InRadius;
			XMFLOAT3 bitangent(dr * c, -InHeight, dr * s);
			//构建法线
			XMVECTOR T = XMLoadFloat3(&Vertex.UTangent);
			XMVECTOR B = XMLoadFloat3(&bitangent);
			XMVECTOR N = -XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&Vertex.Normal, N);
		}
	}
	//圆锥体体底部的中心点
	MeshData.VertexData.push_back(
		FVertex(
			XMFLOAT3(0.f, -0.5f * InHeight, 0.f), 
			XMFLOAT4(Colors::White),
			XMFLOAT3(0.f, -1.f, 0.f)));

	//绘制顶部面 Index
	for (uint32_t i = 0; i < InAxialSubdivision; ++i)
	{
		//绘制三角形
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);

	}
	//构建圆锥底部
	float BaseIndex = 1;//初始顶点 1
	float VertexCircleNum = InAxialSubdivision + 1;//旋转一圈的顶点数量
	//圆锥腰部
	for (uint32_t i = 0; i < InHeightSubdivision - 1; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			//绘制四边形
			//三角形1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//三角形2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}


	//绘制南极  南极的起始点在我们当前的顶点数减去1
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

	//绘制南极
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;//南极的起始点在我们当前的顶点数减去1
	BaseIndex = SouthBaseIndex - VertexCircleNum;//南极的Index基础点就等于 当前的南极起始点减去最后旋转轴的一圈顶点再加1。最后加1是为了求Index索引而不是数量
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//这里是逆时针 逆时针法线是正的
	{

		MeshData.IndexData.push_back(SouthBaseIndex);//第一个一定是0 
		MeshData.IndexData.push_back(BaseIndex + Index);//2
		MeshData.IndexData.push_back(BaseIndex + Index + 1); //1    0-2-1组成的三角面
	}

}

//构建哈希key
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

