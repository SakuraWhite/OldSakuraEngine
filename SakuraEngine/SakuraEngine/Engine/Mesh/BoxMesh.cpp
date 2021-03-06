#include "BoxMesh.h"
#include "Core/MeshType.h"

void GBoxMesh::Init()
{
	Super::Init();//执行父类
}

void GBoxMesh::BuildMesh(const FMeshRenderingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//执行父类

}

void GBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//执行父类
}


void GBoxMesh::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth)
{
	

	//计算模型中心点
	float CHeight = 0.5f * InHeight;
	float CWidth = 0.5f * InWidth;
	float CDepth = 0.5f * InDepth;




	//构建顶点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White)));//
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, -CDepth),XMFLOAT4(Colors::White)));//绘制第二个点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, CHeight, -CDepth),XMFLOAT4(Colors::White)));//绘制第三个点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White)));//绘制第四个点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, CDepth),XMFLOAT4(Colors::White)));//绘制第五个点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, CDepth),XMFLOAT4(Colors::White)));//绘制第六个点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, CHeight, CDepth),XMFLOAT4(Colors::White)));//绘制第七个点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, CDepth),XMFLOAT4(Colors::White)));//绘制第八个点


	//构建顶点索引
	//第一个四边面 前
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(2);//第一个三角面
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(3);//第二个三角面并且和第一个连城一个四边形
	//第二个四边面 后
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(5);//第三个三角面
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(7); MeshData.IndexData.push_back(6);//第四个三角面

	//第三个四边面 左
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(1);//第五个三角面
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(0);//第六个三角面

	//第四个四边面 右
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(6);//第七个三角面
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(7);//第八个三角面

	//第五个四边面 上  
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(6);//第九个三角面
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(2);//第十个三角面

	//第六个四边面 下
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(3);//第十一个三角面
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(7);//第十二个三角面
}
