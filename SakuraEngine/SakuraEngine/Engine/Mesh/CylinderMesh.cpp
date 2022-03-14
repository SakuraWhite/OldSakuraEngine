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
	//半径间隔:计算从顶部到底部的半径差.如果是圆柱形,那就为0。为了计算上窄下宽或者上宽下窄的情况
	float RadiusInterval = (InTopRadius - InBottomRadius) / InHeightSubdivision;//顶部半径减去底部半径，然后除以高度细分，计算从上到下的宽度差的值
	//高度间隔:计算圆柱体横向细分的间隔。
	float HeightInterval = InHeight / InHeightSubdivision; //圆柱体的高度除以细分。
	//弧度
	float Betavalue = XM_2PI / (float)InAxialSubdivision;//2Π除以当前的轴细分
	//遍历计算圆柱体顶点
	for (uint32_t i = 0; i < InHeightSubdivision; ++i )//先构建高度  使用的是高度细分
	{
		float Y = (0.5f * InHeight) - HeightInterval * i;//计算Y轴
		float Radius = InTopRadius + i * RadiusInterval;//计算半径

		for (uint32_t j = 0; j <= InAxialSubdivision; ++j)//构建半径  使用的是轴向
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					Radius * cosf(j * Betavalue),//计算X轴  r * COSβ * i  半径乘以COS的弧度乘以i
					Y,
					Radius * sinf(j * Betavalue)),//计算Z轴 cos(Π/2 - β) * i  = R * sinβ * i    原本是COS的2分之Π减去弧度，因为三角函数的公式可等同于半径乘以sin的弧度
				XMFLOAT4(Colors::Red)));
		}
	}

	//绘制模型面
	float VertexCircleNum = InAxialSubdivision + 1;//旋转一圈的顶点数量
	//绘制圆柱体腰围
	for (uint32_t i = 0; i < InHeightSubdivision; ++i)
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
	{
		uint32_t Index = MeshData.VertexData.size();//圆柱体顶部起始点
		float Y = 0.5f * InHeight; //求出Y轴
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InTopRadius * cosf(i * Betavalue),//计算X轴  r * COSβ * i   半径乘以COS的弧度乘以i
					Y,
					InTopRadius * sinf(i * Betavalue)),//计算Z轴 cos(Π/2 - β) * i = R * sinβ  * i   原本是COS的2分之Π减去弧度，因为三角函数的公式可等同于半径乘以sin的弧度乘以i
				XMFLOAT4(Colors::Red)));
		}
		//圆柱体顶部的中心点
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f),XMFLOAT4(Colors::Red)));
	
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
	{

		uint32_t Index = MeshData.VertexData.size();//圆柱体底部起始点
		float Y = -0.5f * InHeight; //求出Y轴 底部是负数
		for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
		{
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InBottomRadius * cosf(i * Betavalue),//计算X轴  r * COSβ * i   半径乘以COS的弧度乘以i
					Y,
					InBottomRadius * sinf(i * Betavalue)),//计算Z轴 cos(Π/2 - β) * i = R * sinβ  * i   原本是COS的2分之Π减去弧度，因为三角函数的公式可等同于半径乘以sin的弧度乘以i
				XMFLOAT4(Colors::Red)));
		}


		//圆柱体顶部的中心点
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, Y, 0.f), XMFLOAT4(Colors::Red)));

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


