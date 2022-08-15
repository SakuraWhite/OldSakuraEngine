#include "ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../Math/EngineMath.h"
#include "../../Core/Viewport/ViewportTransformation.h"
//Mesh头文件
#include "../../Component/Mesh/BoxMeshComponent.h"
#include "../../Component/Mesh/ConeMeshComponent.h"
#include "../../Component/Mesh/CustomMeshComponent.h"
#include "../../Component/Mesh/CylinderMeshComponent.h"
#include "../../Component/Mesh/PlaneMeshComponent.h"
#include "../../Component/Mesh/SphereMeshComponent.h"
#include "../../Component/Mesh/PyramidMeshComponent.h"
#include "../../Component/Mesh/PipeMeshComponent.h"
#include "../../Component/Mesh/TorusMeshComponent.h"
//Construction
#include "../../Core/Construction/MeshConstruction.h"

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

CMeshComponent* CMeshManage::CreateSphereMeshComponent(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse)
{
	return MeshConstruction::CreateMeshComponent<CSphereMeshComponent>(this, InRadius, InAxialSubdivision, InHeightSubdivision, bReverse);
}

CMeshComponent* CMeshManage::CreatePyramidMeshComponent(EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize)
{
	return MeshConstruction::CreateMeshComponent<CPyramidMeshComponent>(this, InPyramidNumberSidesType, InHeightSubdivide, InSize);
}

CMeshComponent* CMeshManage::CreatePipeMeshComponent(float InTopRadius, float InBottomRadius, float InHeight, float InThickness, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return MeshConstruction::CreateMeshComponent<CPipeMeshComponent>(this, InTopRadius, InBottomRadius, InHeight, InThickness, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreateTorusMeshComponent(float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return MeshConstruction::CreateMeshComponent<CTorusMeshComponent>(this, InRadius, InSectionRadius, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreateMeshComponent(string& InPath)
{
	return MeshConstruction::CreateMeshComponent<CCustomMeshComponent>(this, InPath);
}

CMeshComponent* CMeshManage::CreateBoxMeshComponent(float InHeight, float InWidth, float InDepth)
{
	return MeshConstruction::CreateMeshComponent<CBoxMeshComponent>(this, InHeight, InWidth, InDepth);
}

CMeshComponent* CMeshManage::CreateConeMeshComponent(float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return MeshConstruction::CreateMeshComponent<CConeMeshComponent>(this, InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreateCylinderMeshComponent(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return MeshConstruction::CreateMeshComponent<CCylinderMeshComponent>(this, InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManage::CreatePlaneMeshComponent(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	return MeshConstruction::CreateMeshComponent<CPlaneMeshComponent>(this, InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

