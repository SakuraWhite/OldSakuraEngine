#pragma once//防止重复包含

#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"


//提供C开头的核心对象
class CCoreMinimalObject   :public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	//传入到Camera.h
	virtual void BeginInit() {};
	virtual void Tick(float DeltaTime) {};

	template<class T> //制作模板
	T* CreateObject(CCoreMinimalObject *NewObject)
	{
		return dynamic_cast<T*>(NewObject);
	}

	bool IsTick()const { return bTick; } //判断是不是需要Tick
protected:
	bool bTick;
};

extern vector<CCoreMinimalObject*> GObjects;//全局对象