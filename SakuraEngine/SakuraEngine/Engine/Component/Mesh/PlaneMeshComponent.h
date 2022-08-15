#pragma once
#include "Core/ShellMeshComponent.h"

class CPlaneMeshComponent :public CShellMeshComponent
{
public:
	CPlaneMeshComponent();
	
	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);

	//¹¹½¨¹þÏ£key
	void BuildKey(size_t& OutHashKey, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};
