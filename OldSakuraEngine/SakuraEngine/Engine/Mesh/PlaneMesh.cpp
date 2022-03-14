#include "Core/MeshType.h"
#include "PlaneMesh.h"

void CPlaneMesh::Init()
{
	Super::Init();//执行父类
}

void CPlaneMesh::BuildMesh(const FMeshRendingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//执行父类

}

void CPlaneMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//执行父类
}

CPlaneMesh* CPlaneMesh::CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	FMeshRendingData MeshData;//创建当前Mesh结构数据
	auto SubdivideValue =  [ & ] ( float InValue, uint32_t  InSubdivideValue)->float  //判断细分值 防止归零导致出错
	{
		if (InSubdivideValue <= 1)
		{
			return InValue;
		}

		return InValue / ((float) InSubdivideValue - 1);
	};

	//计算模型中心点
	float CHeight = 0.5f * InHeight;//计算平面高度中心点
	float CWidth = 0.5f * InWidth;//计算平面宽度中心点
	//计算模型的细分
	float HeightSubdivideValie = SubdivideValue(InHeight,InHeightSubdivide );//平面的高度细分计算
	float WidthSubdivideValie = SubdivideValue(InWidth, InWidthSubdivide);//平面的宽度细分计算

	//构建顶点
	for (uint32_t i = 0; i < InHeightSubdivide; ++i)
	{
		float Z = CHeight - i * HeightSubdivideValie;//计算Z轴

		for (uint32_t j = 0; j < InWidthSubdivide; ++j)
		{
			float X = CWidth - j * WidthSubdivideValie;//计算Z轴 决定绘制方向
			MeshData.VertexDeta.push_back(FVertex(
				XMFLOAT3(
					X, //x
					0.f, //y
					Z),//z
				XMFLOAT4(Colors::Red)));

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
				MeshData.IndexDeta.push_back(i * InWidthSubdivide + j);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j);
			//三角形2
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j + 1);
			*/
			//三角形1
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j);
			//三角形2
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexDeta.push_back((i + 1) * InWidthSubdivide + j);


		}
	}



	CPlaneMesh* PlaneMesh =new CPlaneMesh;
	PlaneMesh->BuildMesh(&MeshData);//构建完的数据Bulid一下

	PlaneMesh->Init();//确定构建完之后初始化

	return PlaneMesh;
}
