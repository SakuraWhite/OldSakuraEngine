#include "InputComponent.h"

void CInputComponent::BeginInit()
{
}

void CInputComponent::Tick(float DeltaTime)
{
	if (CaptureKeyboardInforDelegate.IsBound()) //�ж��Ƿ����
	{
		FInputKey InputKey;
		CaptureKeyboardInforDelegate.Execute(InputKey); //ȷ�����˲Ż�ִ��
	}
}
