#pragma once
#include "../../Core/Engine.h"


//�ಥ ���� ר���������ռ��������������

//�������					ʹ�õ���Ӳ��C++�̷̳�װ�õĶಥ����	void����ֵ�ǿյ� ��һ��int��X�ᣬ �ڶ���int��Y��
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesDelegate, void, int ,int);
//�����������ִ���
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaptureOnMousesWheelsDelegate, void, int, int, float);



extern FCaptureOnMousesWheelsDelegate MousesWheelsDelegate;//����������ָ��
extern FCaptureOnMousesDelegate MouseDownDelegate;//���������ָ��
extern FCaptureOnMousesDelegate MouseUpDelegate; //��������ɿ�ָ��
extern FCaptureOnMousesDelegate MouseMoveDelegate;//��������ƶ�ָ��

