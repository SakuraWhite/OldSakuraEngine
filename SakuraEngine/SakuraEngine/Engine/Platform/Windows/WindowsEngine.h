#pragma once//��ֹ�ظ�����

#if defined(_WIN32)
#include "../../Core/Engine.h"//�����������ļ����ҵ�Core�ļ��а�����Engine.h���ļ�
#include "../../Rendering//Enigne/Core/RenderingEngine.h"

class CDirectXRenderingEngine;

class CWindowsEngine :public CEngine //CWindowsEngine�̳���"Engine.h"�е�CEngine
{
	friend class IRenderingInterface; //����һ���ӿڿ�����Rendering.h����WindowsEngine.h������к���

public://������ ��ʼ�����
	CWindowsEngine();
	~CWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);//Ԥ��ʼ�� ׼����ʼ��֮ǰ�Ĺ���
	
	virtual int Init(FWinMainCommandParameters InParameters); //����ĳ�ʼ��
	virtual int PostInit();//��ʼ��֮�����β

	
	virtual void Tick(float DeltaTime);//Tick���ϵ�ˢ����Ⱦ����

	virtual int PreExit(); //Ԥ�˳�
	virtual int Exit(); //�˳�
	virtual int PostExit(); //�˳�����ڴ��ͷţ�Ҳ������β

	CDirectXRenderingEngine* GetRenderingEngine() { return RenderingEngine; }

private: //˽�е� ��ʼ���������
	bool InitWindows(FWinMainCommandParameters InParameters);  //��ʼ��windows����

protected: //�ܱ����� 
	HWND MianWindowsHandle;//��Windows���

protected:
	CDirectXRenderingEngine* RenderingEngine;
};

#endif
