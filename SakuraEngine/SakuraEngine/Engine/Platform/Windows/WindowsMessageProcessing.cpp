#include "WindowsMessageProcessing.h"
#include "../../Component/Input/Input.h"
#include <windowsx.h> //使用Windows的API

//不断刷新的消息队列
LRESULT CALLBACK EngineWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	
	case WM_RBUTTONDOWN:  //"RBUTTONDOWN"的意思就是鼠标右键摁下
		MouseDownDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));   //鼠标摁下之后执行代理中的指令即可  "GET_X_LPARAM"是windows的API可以获取到X轴然后是Y轴
		return 0;
	
	case WM_RBUTTONUP:  //"RUTTONUP"的意思就是鼠标右键松开
		MouseUpDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	
	case WM_MOUSEMOVE:  //"MOUSEMOVE"的意思就是鼠标的移动
		MouseMoveDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEWHEEL:  //"MOUSEWHEEL"的意思就是鼠标的滚轮
		//										滚轮也是有X轴和Y轴的					传入short类型的滚轮数值
		MousesWheelsDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (short)HIWORD(wParam));
		return 0;
	}

	//交给windows自己处理完成
	return DefWindowProc(hwnd, msg, wParam, lParam);
}