//控制玩家视口或者相机  传入指令使用的  控制输入输出
#pragma once//防止重复包含
#include "core/Component.h"
#include "Input/InputType.h"

//硬核C++里封装好的代理
DEFINITION_SIMPLE_SINGLE_DELEGATE(
	FCaptureKeyboardInforDelegate,//键盘的输入
	void,	//返回一个Void类型
	const FInputKey &);      //在Input文件夹中定义的摁键枚举 枚举默认是摁下键位状态

class CInputComponent :public CComponent
{
public:
	CVARIABLE()
	FCaptureKeyboardInforDelegate CaptureKeyboardInforDelegate;//键盘的输入

public:
	//初始化与输入进行的接口
	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

};