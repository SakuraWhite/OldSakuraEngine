//��Ϊ����Ĺ���ʹ��

#pragma once//��ֹ�ظ�����
#include "Core/Engine.h" //����������

class FEngineFactory //�����ࡰFEngineFactory��
{
public://��������
	FEngineFactory(); //������
	static CEngine *CreateEngine(); //��̬�������� ָ��ΪEngine.h�е�CEngine
};