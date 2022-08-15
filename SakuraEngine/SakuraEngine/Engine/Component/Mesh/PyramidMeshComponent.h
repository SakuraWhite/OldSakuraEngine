#pragma once
#include "Core/ShellMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

class CPyramidMeshComponent :public CShellMeshComponent
{
public:
	CPyramidMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide,uint32_t InSize = 1);

	void BuildKey(size_t& OutHashKey, EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize = 1);
};
