#include "InputComponent.h"
#include "Input/Input.h"

void CInputComponent::BeginInit()
{
	//做鼠标输入的绑定
	MouseDownDelegate.AddFunction(this, &CInputComponent::OnMouseButtonDown);
	MouseUpDelegate.AddFunction(this, &CInputComponent::OnMouseButtonUp);
	MouseMoveDelegate.AddFunction(this, &CInputComponent::OnMouseMove);
	MousesWheelsDelegate.AddFunction(this, &CInputComponent::OnMouseWheel);
}

void CInputComponent::Tick(float DeltaTime)
{
	if (CaptureKeyboardInforDelegate.IsBound()) //判断是否绑定了
	{
		FInputKey InputKey;
		if (GetAsyncKeyState('W') & 0x8000)//利用windows的API来判断输入W键 然后与0x8000做一个位运算
		{
			InputKey.KeyName = "W";
		}
		else if (GetAsyncKeyState('S') & 0x8000)//如果我们摁下的是S

		{
			InputKey.KeyName = "S";
		}
		else if (GetAsyncKeyState('A') & 0x8000)//如果我们摁下的是A
		{
			InputKey.KeyName = "A";
		}
		else if (GetAsyncKeyState('D') & 0x8000)//如果我们摁下的是D
		{
			InputKey.KeyName = "D";
		}
		else if (GetAsyncKeyState('Q') & 0x8000)//如果我们摁下的是Q
		{
			InputKey.KeyName = "Q";
		}
		else if (GetAsyncKeyState('E') & 0x8000)//如果我们摁下的是E
		{
			InputKey.KeyName = "E";
		}
		else  //如果不是上方的摁键
		{
			return;// 那么直接返回
		}


		CaptureKeyboardInforDelegate.Execute(InputKey); //确定绑定了才会执行
	}
}

void CInputComponent::OnMouseButtonDown(int X, int Y)
{
	if (OnMouseButtonDownDelegate.IsBound())//判断是否已经绑定
	{
		OnMouseButtonDownDelegate.Execute((int)X, (int)Y);//绑定之后开始执行
	}
}

void CInputComponent::OnMouseButtonUp(int X, int Y)
{
	if (OnMouseButtonUpDelegate.IsBound())//判断是否已经绑定
	{
		OnMouseButtonUpDelegate.Execute((int)X, (int)Y);//绑定之后开始执行
	}
}

void CInputComponent::OnMouseMove(int X, int Y)
{
	if (OnMouseMoveDelegate.IsBound())//判断是否已经绑定
	{
		OnMouseMoveDelegate.Execute((int)X, (int)Y);//绑定之后开始执行
	}
}

void CInputComponent::OnMouseWheel(int X, int Y, float InDelta)
{
	if (OnMouseWheelDelegate.IsBound())
	{
		OnMouseWheelDelegate.Execute((int)X, (int)Y, (float)InDelta);
	}
}
