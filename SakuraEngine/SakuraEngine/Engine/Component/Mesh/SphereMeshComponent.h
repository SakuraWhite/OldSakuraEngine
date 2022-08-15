#pragma once
#include "Core/ShellMeshComponent.h"

class CSphereMeshComponent :public CShellMeshComponent
{
public:
	CSphereMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivisionbool, bool bReverse = false);
	//¹¹½¨¹þÏ£key
	void BuildKey(size_t& OutHashKey, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse);
};
