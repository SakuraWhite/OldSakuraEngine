//ö��  ��������������ʹ��

#pragma  once
#include "../../EngineMinimal.h"

enum EPressState
{
	//�������µ�״̬ �ɿ���������
	Press, //����
	Release , //�ɿ�
};



struct  FInputKey  //�ṹ������ö��
{
	FInputKey();


	string KeyName;//�������������  
	EPressState PressState;   //����״̬

};