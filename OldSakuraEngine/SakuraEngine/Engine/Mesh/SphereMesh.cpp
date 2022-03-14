#include "Core/MeshType.h"
#include "SphereMesh.h"

void CSphereMesh::Init()
{
	Super::Init();//执行父类
}

void CSphereMesh::BuildMesh(const FMeshRendingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//执行父类

}

void CSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//执行父类
}

CSphereMesh* CSphereMesh::CreateMesh(
	float InRadius, //球体的是半径
	uint32_t InAxialSubdivision,//球体的轴向细分
	uint32_t InHeightSubdivision) //球体的高度细分
{
	FMeshRendingData MeshData;//创建当前Mesh结构数据

	//球体轴向的计算βθ
	float ThetaValue = XM_2PI / InHeightSubdivision;//这里是计算的是β 高度轴
	float BetaValue = XM_PI / InAxialSubdivision; //这里计算的是θ 轴心
	//计算起始点
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(0.f,InRadius,0.f),XMFLOAT4(Colors::Red))); //赋予颜色


	//开始计算构建球体顶点
	for (uint32_t i = 1; i< InAxialSubdivision; ++i)
	{
		float Beta = i * BetaValue;  //构建轴 让面旋转360°  每旋转一周就往下走一层
		//来生成球体的高度面
		for (uint32_t j = 0; j<= InHeightSubdivision; ++j)//判断高度轴
 		{
			float Theta = j * ThetaValue; //构建高度轴 让其竖着旋转180° 
			//球面坐标转换为笛卡尔坐标
			MeshData.VertexDeta.push_back(FVertex(
				XMFLOAT3(
					InRadius * sinf(Beta) * cosf(Theta),//构建X轴 x = r * sinθ * cosβ r是球半径InRadius
					InRadius * cosf(Beta), //构建Y轴 y = r * cosβ
					InRadius * sinf(Beta) * sinf(Theta)),//构建Z轴 z = r * sinβ * sinθ
				XMFLOAT4(Colors::Red))); //赋予颜色

			int TopIndex = MeshData.VertexDeta.size() - 1; //获取顶点

			//计算法线 用的是DX的API 
			XMVECTOR Pos = XMLoadFloat3(&MeshData.VertexDeta[TopIndex].Position);//首先转一下结构 
			XMStoreFloat3(&MeshData.VertexDeta[TopIndex].Normal, XMVector3Normalize(Pos));//在顶点生成法线
 		}
	} 

	//计算球体底部终止点
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(0.f, -InRadius, 0.f), XMFLOAT4(Colors::Red))); //赋予颜色


	//绘制北极
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//这里是逆时针 逆时针法线是正的
	{
	
		MeshData.IndexDeta.push_back(0);//第一个一定是0 
		MeshData.IndexDeta.push_back(Index +1);//2
		MeshData.IndexDeta.push_back(Index); //1    0-2-1组成的三角面
	}


	float BaseIndex = 1;//初始顶点 1
	float VertexCircleNum = InAxialSubdivision + 1;//旋转一圈的顶点数量
	

	 //绘制球中间的环面
	for (size_t i = 0; i< InHeightSubdivision - 2; ++i) //球体的高度轴减2是减去南北极
	{
		for (size_t j = 0; j < InAxialSubdivision; ++j)//开始绘制环圈
		{
			//绘制四边形
			//三角形1
			MeshData.IndexDeta.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexDeta.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexDeta.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//三角形2
			MeshData.IndexDeta.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexDeta.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexDeta.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);

		}

	}




	//绘制南极  南极的起始点在我们当前的顶点数减去1
	uint32_t SouthBaseIndex =  MeshData.VertexDeta.size() - 1;//南极的起始点在我们当前的顶点数减去1
	BaseIndex = SouthBaseIndex - VertexCircleNum;//南极的Index基础点就等于 当前的南极起始点减去最后旋转轴的一圈顶点再加1。最后加1是为了求Index索引而不是数量
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//这里是逆时针 逆时针法线是正的
	{
	
		MeshData.IndexDeta.push_back(SouthBaseIndex);//第一个一定是0 
		MeshData.IndexDeta.push_back(BaseIndex + Index);//2
		MeshData.IndexDeta.push_back(BaseIndex + Index + 1); //1    0-2-1组成的三角面
	}

	//绘制Mesh
	CSphereMesh* SphereMesh = new CSphereMesh;
	SphereMesh->BuildMesh(&MeshData);

	SphereMesh->Init();//确定构建完之后初始化

	return SphereMesh;
}
