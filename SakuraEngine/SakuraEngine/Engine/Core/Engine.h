#pragma once//防止重复包含
#include "CoreObject/CoreMinimalObject.h"

#if defined(_WIN32)
#include "WinMainCommandParameters.h"
#endif //判断是否为win平台 这里是1

class CEngine : public CCoreMinimalObject //Cengine创建后继承来自CoreMinimalObject.h的CCoreMinimalObject
{
public: //公共 开放
	CEngine();

	virtual int PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif //预初始化 准备初始化之前的工作 这里是1
	) = 0; 
	virtual int Init(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif //预初始化 准备初始化之前的工作 这里是1
	) = 0; //引擎的初始化
	virtual int PostInit() = 0;//初始化之后的收尾

	virtual void Tick(float DeltaTime) {}; //Tick不断的刷新渲染引擎

	virtual int PreExit() = 0; //预退出
	virtual int Exit() = 0; //退出
	virtual int PostExit() = 0; //退出后的内存释放，也就是收尾
};