//��Ⱦ������RenderingEngine  
#pragma once 
#include "../../../Core/Engine.h"

class CRenderingEngine : public CEngine//�̳���CEngine
{
public:
	void SetMianWindowsHandle(HWND InNewMianWindowsHandle);

protected:
	HWND MianWindowsHandle;
};