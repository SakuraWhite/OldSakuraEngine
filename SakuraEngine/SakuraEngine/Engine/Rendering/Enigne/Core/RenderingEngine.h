//äÖÈ¾Æ÷ºËÐÄRenderingEngine  
#pragma once 
#include "../../../Core/Engine.h"

class CRenderingEngine : public CEngine//¼Ì³Ð×ÔCEngine
{
public:
	void SetMianWindowsHandle(HWND InNewMianWindowsHandle);

protected:
	HWND MianWindowsHandle;
};