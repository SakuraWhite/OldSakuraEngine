#include "SphereMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CSphereMeshComponent::CSphereMeshComponent()
{

}

void CSphereMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse)
{

	//球体轴向的计算βθ
	float ThetaValue = XM_2PI / InHeightSubdivision;//这里是计算的是β 高度轴
	float BetaValue = XM_PI / InAxialSubdivision; //这里计算的是θ 轴心
	//计算起始点
	MeshData.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, InRadius, 0.f),		//顶点半径
		XMFLOAT4(Colors::White),		//赋予颜色
		bReverse ? XMFLOAT3(0.0f, -1.0f, 0.0f) : XMFLOAT3(0.0f, 1.0f, 0.0f),	//顶点法线  垂直于Y轴 是否反转法线
		XMFLOAT2(0.5f, 0.f)));		////添加顶部纹理UV坐标


	//开始计算构建球体顶点
	for (uint32_t i = 1; i < InAxialSubdivision; ++i)
	{
		float Beta = i * BetaValue;  //构建轴 让面旋转360°  每旋转一周就往下走一层
		//来生成球体的高度面
		for (uint32_t j = 0; j <= InHeightSubdivision; ++j)//判断高度轴
		{
			float Theta = j * ThetaValue; //构建高度轴 让其竖着旋转180° 
			//球面坐标转换为笛卡尔坐标
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InRadius * sinf(Beta) * cosf(Theta),//构建X轴 x = r * sinθ * cosβ r是球半径InRadius
					InRadius * cosf(Beta), //构建Y轴 y = r * cosβ
					InRadius * sinf(Beta) * sinf(Theta)),//构建Z轴 z = r * sinβ * sinθ
				XMFLOAT4(Colors::White))); //赋予颜色

			int TopIndex = MeshData.VertexData.size() - 1; //获取顶点
			FVertex& InVertex = MeshData.VertexData[TopIndex];//提取顶点

			//储存位置/法线
			XMVECTOR Pos = XMLoadFloat3(&InVertex.Position);//首先转一下结构 
			XMStoreFloat3(&InVertex.Normal, XMVector3Normalize(Pos));//在顶点生成法线

			//计算切线 U方向的切线
			InVertex.UTangent.x = -InRadius * sinf(Beta) * sinf(Theta); //切线的X轴 等于模型的Z轴的反方向
			InVertex.UTangent.y = 0.f;//切线没有高度紧贴于表面 所以Y轴等于0
			InVertex.UTangent.z = InRadius * sinf(Beta) * cosf(Theta); //切线的Z轴等于模型的X轴

			//构建UV
			InVertex.TexCoord.x = Theta / XM_2PI; 
			InVertex.TexCoord.y = Beta / XM_PI;

			//储存切线
			XMVECTOR Tangent = XMLoadFloat3(&InVertex.UTangent);//转换结构
			XMStoreFloat3(&InVertex.UTangent, XMVector3Normalize(Tangent));//再顶点生成切线


		}
	}

	//计算球体底部终止点
	MeshData.VertexData.push_back(FVertex(
		XMFLOAT3(0.f, -InRadius, 0.f),   //底部半径
		XMFLOAT4(Colors::White),		 //赋予颜色
		bReverse ? XMFLOAT3(0.0f, 1.0f, 0.0f) : XMFLOAT3(0.0f, -1.0f, 0.0f),	 //底部的顶点法线  垂直于-Y轴    判断是否反转法线
		XMFLOAT2(0.f, 0.5f)));			 //添加底部纹理UV坐标


	//绘制北极
	for (uint32_t Index = 0; Index <= InAxialSubdivision; ++Index)//这里是逆时针 逆时针法线是正的
	{
		if (bReverse)//是否反转法线
		{
			MeshData.IndexData.push_back(0);//第一个一定是0 
			MeshData.IndexData.push_back(Index);
			MeshData.IndexData.push_back(Index + 1);
		}
		else
		{
			MeshData.IndexData.push_back(0);//第一个一定是0 
			MeshData.IndexData.push_back(Index + 1);//2
			MeshData.IndexData.push_back(Index);//1    0-2-1组成的三角面
		}

	}


	float BaseIndex = 1;//初始顶点 1
	float VertexCircleNum = InAxialSubdivision + 1;//旋转一圈的顶点数量


	 //绘制球中间的环面
	for (size_t i = 0; i < InHeightSubdivision - 1; ++i) //球体的高度轴减2是减去南北极
	{
		for (size_t j = 0; j < InAxialSubdivision; ++j)//开始绘制环圈
		{
			
			DrawQuadrilateral(MeshData,//输入模型数据
				GetQuadrilateralDrawPointTypeA(j, i, InAxialSubdivision, 1),//球中间环面算法宏
				bReverse);//是否反转法线

			////绘制四边形
			////三角形1
			//MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			//MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			//MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			////三角形2
			//MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			//MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);

		}

	}




	//绘制南极  南极的起始点在我们当前的顶点数减去1
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;//南极的起始点在我们当前的顶点数减去1
	BaseIndex = SouthBaseIndex - VertexCircleNum;//南极的Index基础点就等于 当前的南极起始点减去最后旋转轴的一圈顶点再加1。最后加1是为了求Index索引而不是数量
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//这里是逆时针 逆时针法线是正的
	{
		if (bReverse)
		{
			MeshData.IndexData.push_back(BaseIndex + Index);
			MeshData.IndexData.push_back(SouthBaseIndex);
			MeshData.IndexData.push_back(BaseIndex + Index + 1);
		}
		else
		{
			MeshData.IndexData.push_back(SouthBaseIndex);//第一个一定是0 
			MeshData.IndexData.push_back(BaseIndex + Index);//2
			MeshData.IndexData.push_back(BaseIndex + Index + 1);//1    0-2-1组成的三角面
		}

	}


	//纠正
	//MeshData.IndexData[SouthBaseIndex - 2];
	//MeshData.IndexData[SouthBaseIndex - 3];

}


//构建哈希key
void CSphereMeshComponent::BuildKey(size_t& OutHashKey, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse)
{
	std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 8;
	OutHashKey += FloatHash(InRadius);

	OutHashKey += IntHash._Do_hash(InAxialSubdivision);
	OutHashKey += IntHash._Do_hash(InHeightSubdivision);
}

