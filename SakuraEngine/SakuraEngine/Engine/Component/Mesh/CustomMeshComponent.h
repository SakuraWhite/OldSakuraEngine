#pragma once
#include "Core/ShellMeshComponent.h"

class CCustomMeshComponent :public CShellMeshComponent
{
public:
	CCustomMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, string& InPath);

	static bool LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData);

	//¹¹½¨¹þÏ£key
	void BuildKey(size_t& OutHashKey, std::string& InPath);
};
