#pragma once
#include "../../Mesh/SphereMesh.h"


//  ��տ���		  �̳е�����ĺ��Ķ���
class GSky :public GSphereMesh
{
	typedef GSphereMesh Super;

public:
	GSky();

public:
	virtual void Tick(float DeltaTime);

};