#include "InputComponent.h"

void CInputComponent::BeginInit()
{
}

void CInputComponent::Tick(float DeltaTime)
{
	if (CaptureKeyboardInforDelegate.IsBound()) //判断是否绑定了
	{
		FInputKey InputKey;
		CaptureKeyboardInforDelegate.Execute(InputKey); //确定绑定了才会执行
	}
}
