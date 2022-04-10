//��������ӿڻ������  ����ָ��ʹ�õ�  �����������
#pragma once//��ֹ�ظ�����
#include "core/Component.h"
#include "Input/InputType.h"

//Ӳ��C++���װ�õĴ��� ��������
DEFINITION_SIMPLE_SINGLE_DELEGATE(
	FCaptureKeyboardInforDelegate,//���̵�����
	void,	//����һ��Void����
	const FInputKey &);      //��Input�ļ����ж��������ö�� ö��Ĭ�������¼�λ״̬

//ͬ�� ���� ���������
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void, int, int);
// ���������ִ���
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseWheelDelegate, void, int, int, float);


class CInputComponent :public CComponent
{
public:
	CVARIABLE()
		FCaptureKeyboardInforDelegate CaptureKeyboardInforDelegate;//���̵�����

	//�󶨵�������
	CVARIABLE()
		FCaptureOnMouseDelegate OnMouseButtonDownDelegate;
	CVARIABLE()
		FCaptureOnMouseDelegate OnMouseButtonUpDelegate;
	CVARIABLE()
		FCaptureOnMouseDelegate OnMouseMoveDelegate;
	CVARIABLE()
		FCaptureOnMouseWheelDelegate OnMouseWheelDelegate;//�����ִ����


public:
	//��ʼ����������еĽӿ�
	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	//�������Ľӿ�
	virtual void OnMouseButtonDown(int X, int Y);
	virtual void OnMouseButtonUp(int X, int Y);
	virtual void OnMouseMove(int X, int Y);
	virtual void OnMouseWheel(int X, int Y, float InDelta);//����������ӿ�
};