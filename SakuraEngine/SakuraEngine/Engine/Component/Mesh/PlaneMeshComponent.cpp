#include "PlaneMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CPlaneMeshComponent::CPlaneMeshComponent()
{

}

void CPlaneMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{

	auto SubdivideValue = [&](float InValue, uint32_t  InSubdivideValue)->float  //判断细分值 防止归零导致出错
	{
		if (InSubdivideValue <= 1)
		{
			return InValue;
		}

		return InValue / ((float)InSubdivideValue - 1);
	};

	//计算模型中心点
	float CHeight = 0.5f * InHeight;//计算平面高度中心点
	float CWidth = 0.5f * InWidth;//计算平面宽度中心点
	//计算模型的细分
	float HeightSubdivideValie = SubdivideValue(InHeight, InHeightSubdivide);//平面的高度细分计算
	float WidthSubdivideValie = SubdivideValue(InWidth, InWidthSubdivide);//平面的宽度细分计算
	//计算模型横向纵向细分的平均
	float HorizontalAverageSubdivision = 1.f / ((float)InWidthSubdivide - 1.f);//水平细分的值 为UV做准备
	float VerticalAverageSubdivision = 1.f / ((float)InHeightSubdivide - 1.f);//垂直细分的值 为UV做准备


	//构建顶点
	for (uint32_t i = 0; i < InHeightSubdivide; ++i)
	{
		float Z = CHeight - i * HeightSubdivideValie;//计算Z轴

		for (uint32_t j = 0; j < InWidthSubdivide; ++j)
		{
			float X = CWidth - j * WidthSubdivideValie;//计算Z轴 决定绘制方向
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					X, //x
					0.f, //y
					Z),//z					
				XMFLOAT4(Colors::Gray), //顶点颜色
				XMFLOAT3(0.f, 1.f, 0.f),//法线
				XMFLOAT2(VerticalAverageSubdivision * i, HorizontalAverageSubdivision * j)));//根据输入的水平垂直细分数值来自动展开UV



		}
	}


	//绘制面Index
	for (uint32_t i = 0; i < InHeightSubdivide - 1; ++i)
	{
		for (uint32_t j = 0; j < InWidthSubdivide - 1; ++j)
		{
			//绘制四边形
			/*法线反转
			//三角形1
				MeshData.IndexData.push_back(i * InWidthSubdivide + j);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
			//三角形2
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j + 1);
			*/
			//三角形1
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j);
			//三角形2
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
		}
	}
}


//构建哈希key
void CPlaneMeshComponent::BuildKey(size_t& OutHashKey, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	std::hash<float> FloatHash;
	std::hash<int> IntHash;

	OutHashKey = 6;
	OutHashKey += FloatHash(InHeight);
	OutHashKey += FloatHash(InWidth);

	OutHashKey += IntHash._Do_hash(InHeightSubdivide);
	OutHashKey += IntHash._Do_hash(InWidthSubdivide);
}