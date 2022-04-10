#include "ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../Math/EngineMath.h"
#include "../../Core/Viewport/ViewportTransformation.h"
//Mesh头文件
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CustomMesh.h"
#include "../CylinderMesh.h"
#include "../SphereMesh.h"
#include "../PlaneMesh.h"
#include "MeshManage.h"

CMeshManage::CMeshManage()
{

}

void CMeshManage::Init()
{

}

void CMeshManage::BuildMesh()
{
	RenderingPipeline.BuildPipeline();//创建管线
}

void CMeshManage::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	RenderingPipeline.UpdateCalculations(DeltaTime, ViewportInfo);

}

void CMeshManage::PostDraw(float DeltaTime)
{
	//从管线开始渲染
	RenderingPipeline.PostDraw(DeltaTime);
}

void CMeshManage::Draw(float DeltaTime)
{
	//从管线开始渲染
	RenderingPipeline.Draw(DeltaTime);

}

void CMeshManage::PreDraw(float DeltaTime)
{
	//从管线开始渲染
	RenderingPipeline.PreDraw(DeltaTime);

}

GMesh* CMeshManage::CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<GSphereMesh>(InRadius, InAxialSubdivision, InHeightSubdivision);
}

GMesh* CMeshManage::CreateMesh(string& InPath)
{
	return CreateMesh<GCustomMesh>(InPath);
}

GMesh* CMeshManage::CreateBoxMesh(float InHeight, float InWidth, float InDepth)
{
	return CreateMesh<GBoxMesh>(InHeight, InWidth, InDepth);
}

GMesh* CMeshManage::CreateConeMesh(float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<GConeMesh>(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

GMesh* CMeshManage::CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<GCylinderMesh>(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

GMesh* CMeshManage::CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	return CreateMesh<GPlaneMesh>(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

template<class T, typename ...ParamTypes> //使用模板来指定生成对象
T* CMeshManage::CreateMesh(ParamTypes && ...Params) 
{
	T* MyMesh = new T();

	//提取模型资源
	FMeshRenderingData MeshData;
	MyMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

	MyMesh->BeginInit();

	//构建mesh
	RenderingPipeline.BuildMesh(MyMesh, MeshData);

	MyMesh->Init();

	return MyMesh;
}