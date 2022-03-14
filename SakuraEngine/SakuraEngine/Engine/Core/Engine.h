#pragma once//��ֹ�ظ�����
#include "CoreObject/CoreMinimalObject.h"

#if defined(_WIN32)
#include "WinMainCommandParameters.h"
#endif //�ж��Ƿ�Ϊwinƽ̨ ������1

class CEngine : public CCoreMinimalObject //Cengine������̳�����CoreMinimalObject.h��CCoreMinimalObject
{
public: //���� ����
	CEngine();

	virtual int PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif //Ԥ��ʼ�� ׼����ʼ��֮ǰ�Ĺ��� ������1
	) = 0; 
	virtual int Init(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif //Ԥ��ʼ�� ׼����ʼ��֮ǰ�Ĺ��� ������1
	) = 0; //����ĳ�ʼ��
	virtual int PostInit() = 0;//��ʼ��֮�����β

	virtual void Tick(float DeltaTime) {}; //Tick���ϵ�ˢ����Ⱦ����

	virtual int PreExit() = 0; //Ԥ�˳�
	virtual int Exit() = 0; //�˳�
	virtual int PostExit() = 0; //�˳�����ڴ��ͷţ�Ҳ������β
};