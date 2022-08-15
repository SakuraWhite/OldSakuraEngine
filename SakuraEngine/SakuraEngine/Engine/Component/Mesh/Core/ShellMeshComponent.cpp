#pragma once
#include "ShellMeshComponent.h"
#include "../../../Mesh/Core/MeshType.h"

CShellMeshComponent::CShellMeshComponent()
{

}

void CShellMeshComponent::DrawQuadrilateral(FMeshRenderingData& MeshData, const fvector_4id& InDrawPoint, bool bReversal)
{
	if (!bReversal)
	{
		//������1
		MeshData.IndexData.push_back(InDrawPoint.z);
		MeshData.IndexData.push_back(InDrawPoint.y);
		MeshData.IndexData.push_back(InDrawPoint.x);

		//������2
		MeshData.IndexData.push_back(InDrawPoint.w);
		MeshData.IndexData.push_back(InDrawPoint.z);
		MeshData.IndexData.push_back(InDrawPoint.x);
	}
	else
	{
		//������1
		MeshData.IndexData.push_back(InDrawPoint.x);
		MeshData.IndexData.push_back(InDrawPoint.y);
		MeshData.IndexData.push_back(InDrawPoint.z);

		//������2
		MeshData.IndexData.push_back(InDrawPoint.x);
		MeshData.IndexData.push_back(InDrawPoint.z);
		MeshData.IndexData.push_back(InDrawPoint.w);
	}
}

fvector_4id CShellMeshComponent::GetQuadrilateralDrawPointTypeA(int InRowsand, int InColumns, int GroupLayer, int Offset)
{
	//���м价���㷨��
	return { InColumns * GroupLayer + InRowsand,					 //A
			(InColumns + 1) * GroupLayer + InRowsand,			 //B
			(InColumns + 1) * GroupLayer + InRowsand + Offset,	 //C
			InColumns * GroupLayer + InRowsand + Offset };		 //D
}