#pragma once//防止重复包含

#if defined(_WIN32)
#include "../../Core/Engine.h"//网上跳两级文件夹找到Core文件夹包含“Engine.h”文件
#include "../../Rendering//Enigne/Core/RenderingEngine.h"

class CDirectXRenderingEngine;

class CWindowsEngine :public CEngine //CWindowsEngine继承自"Engine.h"中的CEngine
{
	friend class IRenderingInterface; //创建一个接口可以让Rendering.h访问WindowsEngine.h里的所有函数

public://公开的 初始化相关
	CWindowsEngine();
	~CWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);//预初始化 准备初始化之前的工作
	
	virtual int Init(FWinMainCommandParameters InParameters); //引擎的初始化
	virtual int PostInit();//初始化之后的收尾

	
	virtual void Tick(float DeltaTime);//Tick不断的刷新渲染引擎

	virtual int PreExit(); //预退出
	virtual int Exit(); //退出
	virtual int PostExit(); //退出后的内存释放，也就是收尾

	CDirectXRenderingEngine* GetRenderingEngine() { return RenderingEngine; }

private: //私有的 初始化窗口相关
	bool InitWindows(FWinMainCommandParameters InParameters);  //初始化windows窗口

protected: //受保护的 
	HWND MianWindowsHandle;//主Windows句柄

protected:
	CDirectXRenderingEngine* RenderingEngine;
};

#endif
