//��������ӿڻ������  ����ָ��ʹ�õ�  �����������
#pragma once//��ֹ�ظ�����
#include "core/Component.h"
#include "Input/InputType.h"

//Ӳ��C++���װ�õĴ���
DEFINITION_SIMPLE_SINGLE_DELEGATE(
	FCaptureKeyboardInforDelegate,//���̵�����
	void,	//����һ��Void����
	const FInputKey &);      //��Input�ļ����ж��������ö�� ö��Ĭ�������¼�λ״̬

class CInputComponent :public CComponent
{
public:
	CVARIABLE()
	FCaptureKeyboardInforDelegate CaptureKeyboardInforDelegate;//���̵�����

public:
	//��ʼ����������еĽӿ�
	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

};