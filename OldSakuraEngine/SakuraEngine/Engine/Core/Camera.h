//摄像机
#pragma once//防止重复包含
#include "Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"



//前置声明
struct FInputKey;
class CTransformationComponent;
class CInputComponent;

//C开头的Camera  C是核心意思 继承自CCoreMinimalObject与Fviewport
class CCamera :public CCoreMinimalObject,public FViewport
{
	CVARIABLE()  //反射宏
	CTransformationComponent* TransformationComponent;//我们希望摄像机能够移动 就需要控制组件
	
	CVARIABLE()  //反射宏
	CInputComponent* InputComponent;//输入输出控制
public:   //构造
	CCamera();
	//初始化
	virtual void BeginInit();
	//输入进行
	virtual void Tick(float DeltaTime);
	//输出绑定
	virtual void ExecuteKeyboard(const FInputKey& InputKey);

public:

	//使用FORCEINLINE来暴漏一些参数接口 作为键盘或者鼠标输入的相应   FORCEINLINE是属于windows平台定义的内容 需要包含关与windows平台
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
	FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }

};