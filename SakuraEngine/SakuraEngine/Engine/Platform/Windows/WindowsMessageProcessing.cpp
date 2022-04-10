#include "WindowsMessageProcessing.h"
#include "../../Component/Input/Input.h"
#include <windowsx.h> //ʹ��Windows��API

//����ˢ�µ���Ϣ����
LRESULT CALLBACK EngineWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	
	case WM_RBUTTONDOWN:  //"RBUTTONDOWN"����˼��������Ҽ�����
		MouseDownDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));   //�������֮��ִ�д����е�ָ���  "GET_X_LPARAM"��windows��API���Ի�ȡ��X��Ȼ����Y��
		return 0;
	
	case WM_RBUTTONUP:  //"RUTTONUP"����˼��������Ҽ��ɿ�
		MouseUpDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	
	case WM_MOUSEMOVE:  //"MOUSEMOVE"����˼���������ƶ�
		MouseMoveDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEWHEEL:  //"MOUSEWHEEL"����˼�������Ĺ���
		//										����Ҳ����X���Y���					����short���͵Ĺ�����ֵ
		MousesWheelsDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (short)HIWORD(wParam));
		return 0;
	}

	//����windows�Լ��������
	return DefWindowProc(hwnd, msg, wParam, lParam);
}