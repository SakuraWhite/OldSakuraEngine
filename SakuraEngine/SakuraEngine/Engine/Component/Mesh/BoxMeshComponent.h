#pragma once
#include "Core/ShellMeshComponent.h"

class CBoxMeshComponent :public CShellMeshComponent
{
public:
	CBoxMeshComponent();

	//����������ģ��
	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth);
	//������ϣkey
	void BuildKey(size_t& OutHashKey, float InHeight, float InWidth, float InDepth);
};
