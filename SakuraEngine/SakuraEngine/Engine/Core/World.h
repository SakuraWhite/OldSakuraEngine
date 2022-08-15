#pragma once//防止重复包含
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

///////////////////////////////
//世界主界面 

//前置声明
struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GCamera;
class GActorObject;

//C开头的Camera  C是核心意思 继承自CCoreMinimalObject与Fviewport
class CWorld :public CCoreMinimalObject 
{
public:
	CWorld();

	GCamera* GetCamera() { return Camera; }

	template<class T>
	T* CreateActorObject()
	{
		T* InArray = new T();
		ActorObjects.push_back(InArray);

		return InArray;
	}

protected:
	CVARIABLE()
	GCamera* Camera;//相机实例

	//存储我们的Actors
	CVARIABLE()
		vector<GActorObject*> ActorObjects;

};