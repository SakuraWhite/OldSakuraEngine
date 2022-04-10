#pragma once//防止重复包含
#include "../../Core/Engine.h"//网上跳两级文件夹找到Core文件夹包含“Engine.h”文件
//消息队列
LRESULT CALLBACK EngineWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 