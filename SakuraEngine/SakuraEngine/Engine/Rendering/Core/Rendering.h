#pragma once//��ֹ�ظ�����
#include "../../Core/Engine.h"
#include "../../Core/CoreObject/GuidInterface.h"
#if defined(_WIN32)   //��������������ͷ�ļ�
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif


//��¼�ṩ��Ⱦ��ص����ݵĽӿ�
class  IRenderingInterface   //:public IGuidInterface //�̳�GUID
{
	friend class CDirectXRenderingEngine;//��Engine����ֱ�ӷ���
public:
	IRenderingInterface();  //����
	virtual ~IRenderingInterface();	//��������

	virtual void Init(); //��Ⱦ��ʼ��

	virtual void PreDraw(float DeltaTime); //Ԥ��Ⱦ
	virtual void Draw(float DeltaTime); //��Ⱦ��� ����ʱ
	virtual void PostDraw(float DeltaTime);//������Ⱦ poatDraw



	/*
private:
	static vector<IRenderingInterface*> RenderingInterface;//��̬����
	*/
};
