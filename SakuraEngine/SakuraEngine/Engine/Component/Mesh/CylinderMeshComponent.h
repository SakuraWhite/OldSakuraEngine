#pragma once
#include "Core/ShellMeshComponent.h"

class CCylinderMeshComponent :public CShellMeshComponent
{
public:
	CCylinderMeshComponent();

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	//¹¹½¨¹þÏ£key
	void BuildKey(size_t& OutHashKey,
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};
