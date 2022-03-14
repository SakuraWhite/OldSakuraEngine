//作为引擎的工厂使用

#pragma once//防止重复包含
#include "Core/Engine.h" //声明包含库

class FEngineFactory //创建类“FEngineFactory”
{
public://公共开放
	FEngineFactory(); //构造体
	static CEngine *CreateEngine(); //静态创建引擎 指针为Engine.h中的CEngine
};