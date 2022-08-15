#pragma once

#include "Core/Mesh.h"

class GTorusMesh :public GMesh
{
	typedef GMesh Super;
public:
	GTorusMesh();

	virtual void Init();

	virtual void Draw(float DeltaTime);

	void CreateMesh(
		float InRadius, 
		float InSectionRadius, 
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};