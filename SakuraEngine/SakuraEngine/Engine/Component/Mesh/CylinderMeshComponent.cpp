#include "CylinderMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CCylinderMeshComponent::CCylinderMeshComponent()
{

}

void CCylinderMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	//半径间隔:计算从顶部到底部的半径差.如果是圆柱形,那就为0。为了计算上窄下宽或者上宽下窄的情况
	float RadiusInterval = (InTopRadius - InBottomRadius) / InHeightSubdivision;//顶部半径减去底部半径，然后除以高度细分，计算从上到下的宽度差的值
	//高度间隔:计算圆柱体横向细分的间隔。
	float HeightInterval = InHeight / InHeightSubdivision; //圆柱体的高度除以细分。
	//弧度
	float BetaValue = XM_2PI / (float)InAxialSubdivision;//2Π除以当前的轴细分
	//遍历计算圆柱体顶点
	for (uint32_t i = 0; i < InHeightSubdivision + 1; ++i)//先构建高度  使用的是高度细分
	{
		float Y = (0.5f * InHeight) - HeightInterval * i;//计算Y轴
		float Radius = InTopRadius + i * RadiusInterval;//计算半径

		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)//构建半径  使用的是轴向
		{
			float BetaValueCos = cosf(j * BetaValue);
			float BetaValueSin = sinf(j * BetaValue);
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * cosf(j * BetaValue),//计算X轴  r * COSβ * i  半径乘以COS的弧度乘以i
					Y,
					Radius * sinf(j * BetaValue)),//计算Z轴 cos(Π/2 - β) * i  = R * sinβ * i    原本是COS的2分之Π减去弧度，因为三角函数的公式可等同于半径乘以sin的弧度
				XMFLOAT4(Colors::White)));
		
			//取出顶点数据
			FVertex& MyVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];
			//切线
			MyVertex.UTangent = XMFLOAT3(-BetaValueSin, 0.0f, BetaValueCos);

			float dr = InBottomRadius - InTopRadius;
			XMFLOAT3 Bitangent(dr * BetaValueCos, -InHeight, dr * BetaValueSin);
			//构建TBN法线
			XMVECTOR T = XMLoadFloat3(&MyVertex.UTangent);
			XMVECTOR B = XMLoadFloat3(&Bitangent);
			XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			XMStoreFloat3(&MyVertex.Normal, N);
		
		}
	}

	//绘制模型面
	float VertexCircleNum = InAxialSubdivision;//旋转一圈的顶点数量
	//绘制圆柱体腰围
	for (uint32_t i = 0; i < InHeightSubdivision + 1; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			// 法线远离摄像机
			//三角形1
			//MeshData.IndexData.push_back(i * VertexCircleNum + j);
			//MeshData.IndexData.push_back((i + 1 )* VertexCircleNum + j);
			//MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j+1);

			////三角形2
			//MeshData.IndexData.push_back(i * VertexCircleNum + j);
			//MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);
			//MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);

			//绘制四边形
			// 法线朝向自己
			//三角形1
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);// (i + 1) * n + j + 1
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j); //(i + 1) * n + j
			MeshData.IndexData.push_back(i * VertexCircleNum + j);  //i * n + j
			//绘制三角形2
			MeshData.IndexData.push_back(i * VertexCircleNum + j + 1);//i * n + j + 1
			MeshData.IndexData.push_back((i + 1) * VertexCircleNum + j + 1);// (i + 1) * n + j + 1
			MeshData.IndexData.push_back(i * VertexCircleNum + j);//i * n + j

		}
	}


	//构建顶部顶点
	if (1)
	{
		uint32_t Index = MeshData.VertexData.size();//圆柱体顶部起始点
		float Y = 0.5f * InHeight; //求出Y轴
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InTopRadius * cosf(i * BetaValue),//计算X轴  r * COSβ * i   半径乘以COS的弧度乘以i
					Y,
					InTopRadius * sinf(i * BetaValue)),//计算Z轴 cos(Π/2 - β) * i = R * sinβ  * i   原本是COS的2分之Π减去弧度，因为三角函数的公式可等同于半径乘以sin的弧度乘以i
				XMFLOAT4(Colors::White),
				XMFLOAT3(0.f, 1.f, 0.f)));
		}

			//圆柱体顶部的中心点
			MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f), XMFLOAT4(Colors::White),XMFLOAT3(0.f, 1.f, 0.f)));
			
			//绘制顶部面
			float Cpoint = MeshData.VertexData.size() - 1;//绘制顶部圆心中心点
			for (uint32_t i = 0; i < InAxialSubdivision; ++i)
			{
				//绘制三角形
				MeshData.IndexData.push_back(Cpoint);
				MeshData.IndexData.push_back(Index + i + 1);
				MeshData.IndexData.push_back(Index + i);
			}
	}


	//构建底部
	if (1)
	{
		uint32_t Index = MeshData.VertexData.size();//圆柱体底部起始点
		float Y = -0.5f * InHeight; //求出Y轴 底部是负数
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InBottomRadius * cosf(i * BetaValue),//计算X轴  r * COSβ * i   半径乘以COS的弧度乘以i
					Y,
					InBottomRadius * sinf(i * BetaValue)),//计算Z轴 cos(Π/2 - β) * i = R * sinβ  * i   原本是COS的2分之Π减去弧度，因为三角函数的公式可等同于半径乘以sin的弧度乘以i
				XMFLOAT4(Colors::Red)));
		}


		//圆柱体顶部的中心点
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f), XMFLOAT4(Colors::White)));

		//绘制顶部面
		float Cpoint = MeshData.VertexData.size() - 1;//绘制顶部圆心中心点
		for (uint32_t i = 0; i < InAxialSubdivision; ++i)
		{
			//绘制三角形
			MeshData.IndexData.push_back(Cpoint);
			MeshData.IndexData.push_back(Index + i);
			MeshData.IndexData.push_back(Index + i + 1);
		}
	}
}


//构建哈希key
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
