#pragma once//防止重复包含

#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../Construction/ObjectConstruction.h"

//提供C开头的核心对象
class CCoreMinimalObject   :public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	//传入到Camera.h
	virtual void BeginInit() {};
	virtual void Tick(float DeltaTime) {};



	bool IsTick()const { return bTick; } //判断是不是需要Tick
protected:
	bool bTick;
};

extern vector<CCoreMinimalObject*> GObjects;//全局对象