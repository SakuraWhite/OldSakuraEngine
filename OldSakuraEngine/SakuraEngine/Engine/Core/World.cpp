#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
	//¹¹½¨ÉãÏñ»ú
	Camera = CreateObject<CCamera>(new CCamera());
}
