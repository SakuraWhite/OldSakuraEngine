#pragma once//防止重复包含
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

///////////////////////////////
//世界主界面 

//前置声明
struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class CCamera;

//C开头的Camera  C是核心意思 继承自CCoreMinimalObject与Fviewport
class CWorld :public CCoreMinimalObject 
{
public:
	CWorld();

	CVARIABLE()
	CCamera* Camera;//相机实例

};