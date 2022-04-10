#include "InputComponent.h"
#include "Input/Input.h"

void CInputComponent::BeginInit()
{
	//���������İ�
	MouseDownDelegate.AddFunction(this, &CInputComponent::OnMouseButtonDown);
	MouseUpDelegate.AddFunction(this, &CInputComponent::OnMouseButtonUp);
	MouseMoveDelegate.AddFunction(this, &CInputComponent::OnMouseMove);
	MousesWheelsDelegate.AddFunction(this, &CInputComponent::OnMouseWheel);
}

void CInputComponent::Tick(float DeltaTime)
{
	if (CaptureKeyboardInforDelegate.IsBound()) //�ж��Ƿ����
	{
		FInputKey InputKey;
		if (GetAsyncKeyState('W') & 0x8000)//����windows��API���ж�����W�� Ȼ����0x8000��һ��λ����
		{
			InputKey.KeyName = "W";
		}
		else if (GetAsyncKeyState('S') & 0x8000)//����������µ���S

		{
			InputKey.KeyName = "S";
		}
		else if (GetAsyncKeyState('A') & 0x8000)//����������µ���A
		{
			InputKey.KeyName = "A";
		}
		else if (GetAsyncKeyState('D') & 0x8000)//����������µ���D
		{
			InputKey.KeyName = "D";
		}
		else if (GetAsyncKeyState('Q') & 0x8000)//����������µ���Q
		{
			InputKey.KeyName = "Q";
		}
		else if (GetAsyncKeyState('E') & 0x8000)//����������µ���E
		{
			InputKey.KeyName = "E";
		}
		else  //��������Ϸ�������
		{
			return;// ��ôֱ�ӷ���
		}


		CaptureKeyboardInforDelegate.Execute(InputKey); //ȷ�����˲Ż�ִ��
	}
}

void CInputComponent::OnMouseButtonDown(int X, int Y)
{
	if (OnMouseButtonDownDelegate.IsBound())//�ж��Ƿ��Ѿ���
	{
		OnMouseButtonDownDelegate.Execute((int)X, (int)Y);//��֮��ʼִ��
	}
}

void CInputComponent::OnMouseButtonUp(int X, int Y)
{
	if (OnMouseButtonUpDelegate.IsBound())//�ж��Ƿ��Ѿ���
	{
		OnMouseButtonUpDelegate.Execute((int)X, (int)Y);//��֮��ʼִ��
	}
}

void CInputComponent::OnMouseMove(int X, int Y)
{
	if (OnMouseMoveDelegate.IsBound())//�ж��Ƿ��Ѿ���
	{
		OnMouseMoveDelegate.Execute((int)X, (int)Y);//��֮��ʼִ��
	}
}

void CInputComponent::OnMouseWheel(int X, int Y, float InDelta)
{
	if (OnMouseWheelDelegate.IsBound())
	{
		OnMouseWheelDelegate.Execute((int)X, (int)Y, (float)InDelta);
	}
}
