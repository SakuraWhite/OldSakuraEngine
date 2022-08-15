#pragma once
#include "Core/ShellMeshComponent.h"

class CBoxMeshComponent :public CShellMeshComponent
{
public:
	CBoxMeshComponent();

	//构建立方体模型
	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth);
	//创建哈希key
	void BuildKey(size_t& OutHashKey, float InHeight, float InWidth, float InDepth);
};
