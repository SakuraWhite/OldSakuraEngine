#pragma once
#include "Core/ShellMeshComponent.h"

class CConeMeshComponent :public CShellMeshComponent
{
public:
	CConeMeshComponent();

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	//¹¹½¨¹þÏ£key
	void BuildKey(size_t& OutHashKey,
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};
