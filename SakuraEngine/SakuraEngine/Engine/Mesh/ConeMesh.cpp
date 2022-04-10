#include "Core/MeshType.h"
#include "ConeMesh.h"

void GConeMesh::Init()
{
	Super::Init();//执行父类
}

void GConeMesh::BuildMesh(const FMeshRenderingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//执行父类

}

void GConeMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//执行父类
}

void GConeMesh::CreateMesh(FMeshRenderingData& MeshData,//创建当前Mesh结构数据
	float InRadius,  //圆锥体底部半径  因为圆锥体顶部是单个顶点 ，所以只有底部有半径
	float InHeight,  //圆锥体高度
	uint32_t InAxialSubdivision,//轴细分
	uint32_t InHeightSubdivision) //高度细分
{

	//半径间隔
	float RadiusInterVal = -InRadius / (float)InHeightSubdivision;//顶部半径减去底部半径，然后除以高度细分，计算从上到下的宽度差的值
	//高度间隔
	float HeightInterVal = InHeight / (float)InHeightSubdivision;//圆柱体的高度除以细分。
	//弧度
	float Betavalue = XM_2PI / (float)InAxialSubdivision;//2Π除以当前的轴细分


	//构建顶部顶点
		//构建圆锥顶部中心顶点
		MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, 0.5f * InHeight, 0.f), XMFLOAT4(Colors::White)));

		uint32_t Index = MeshData.VertexData.size();//顶点体顶部起始点
		for (uint32_t i = 0; i < InHeightSubdivision; ++i)
		{
			float Y = 0.5f * InHeight - HeightInterVal * i; //求出Y轴  圆锥体只有底部才有圆
			float Radius =  i * RadiusInterVal;
			for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
			{
				MeshData.VertexData.push_back(FVertex(
					XMFLOAT3(
						Radius * cosf(j * Betavalue),//计算X轴
						Y,
						Radius * sinf(j * Betavalue)),//计算Z轴 
					XMFLOAT4(Colors::Red)));
			}
		}
	//圆锥体体底部的中心点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, -0.5f * InHeight, 0.f), XMFLOAT4(Colors::White)));

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
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;//南极的起始点在我们当前的顶点数减去1
	BaseIndex = SouthBaseIndex - VertexCircleNum;//南极的Index基础点就等于 当前的南极起始点减去最后旋转轴的一圈顶点再加1。最后加1是为了求Index索引而不是数量
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//这里是逆时针 逆时针法线是正的
	{

		MeshData.IndexData.push_back(SouthBaseIndex);//第一个一定是0 
		MeshData.IndexData.push_back(BaseIndex + Index);//2
		MeshData.IndexData.push_back(BaseIndex + Index + 1); //1    0-2-1组成的三角面
	}

}