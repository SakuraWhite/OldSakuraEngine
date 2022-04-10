#pragma once
#include "../../Core/Engine.h"


//多播 代理 专门用来接收键盘鼠标的输入输出

//定义代理					使用的是硬核C++教程封装好的多播函数	void返回值是空的 第一个int是X轴， 第二个int是Y轴
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesDelegate, void, int ,int);
//这里是鼠标滚轮代理
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesWheelsDelegate, void, int, int, float);



extern FCaptureOnMousesWheelsDelegate MousesWheelsDelegate;//声明鼠标滚轮指令
extern FCaptureOnMousesDelegate MouseDownDelegate;//声明鼠标点击指令
extern FCaptureOnMousesDelegate MouseUpDelegate; //声明鼠标松开指令
extern FCaptureOnMousesDelegate MouseMoveDelegate;//声明鼠标移动指令

