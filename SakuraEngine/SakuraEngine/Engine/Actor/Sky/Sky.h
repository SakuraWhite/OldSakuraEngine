#pragma once
#include "../../Mesh/SphereMesh.h"


//  天空控制		  继承的是球的核心对象
class GSky :public GSphereMesh
{
	typedef GSphereMesh Super;

public:
	GSky();

public:
	virtual void Tick(float DeltaTime);

};