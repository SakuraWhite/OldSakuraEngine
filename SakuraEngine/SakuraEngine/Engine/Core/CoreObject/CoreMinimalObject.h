#pragma once//��ֹ�ظ�����

#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"


//�ṩC��ͷ�ĺ��Ķ���
class CCoreMinimalObject   :public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	//���뵽Camera.h
	virtual void BeginInit() {};
	virtual void Tick(float DeltaTime) {};

	template<class T> //����ģ��
	T* CreateObject(CCoreMinimalObject *NewObject)
	{
		return dynamic_cast<T*>(NewObject);
	}

	bool IsTick()const { return bTick; } //�ж��ǲ�����ҪTick
protected:
	bool bTick;
};

extern vector<CCoreMinimalObject*> GObjects;//ȫ�ֶ���