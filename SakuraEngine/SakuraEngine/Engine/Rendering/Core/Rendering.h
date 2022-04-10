#pragma once//防止重复包含
#include "../../Core/Engine.h"
#include "../../Core/CoreObject/GuidInterface.h"
#if defined(_WIN32)   //根据引擎来采用头文件
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif


//记录提供渲染相关的内容的接口
class  IRenderingInterface   //:public IGuidInterface //继承GUID
{
	friend class CDirectXRenderingEngine;//让Engine可以直接访问
public:
	IRenderingInterface();  //构造
	virtual ~IRenderingInterface();	//析构函数

	virtual void Init(); //渲染初始化

	virtual void PreDraw(float DeltaTime); //预渲染
	virtual void Draw(float DeltaTime); //渲染相关 进行时
	virtual void PostDraw(float DeltaTime);//交付渲染 poatDraw



	/*
private:
	static vector<IRenderingInterface*> RenderingInterface;//静态函数
	*/
};
