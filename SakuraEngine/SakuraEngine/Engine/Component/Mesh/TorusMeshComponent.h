#pragma once
#include "Core/ShellMeshComponent.h"

class CTorusMeshComponent :public CShellMeshComponent
{
public:
	CTorusMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
	//构建哈希key	 提出哈希key
	void BuildKey(size_t &OutHashKey, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};
