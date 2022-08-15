#pragma once//��ֹ�ظ�����

#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../Construction/ObjectConstruction.h"

//�ṩC��ͷ�ĺ��Ķ���
class CCoreMinimalObject   :public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject();

	//���뵽Camera.h
	virtual void BeginInit() {};
	virtual void Tick(float DeltaTime) {};



	bool IsTick()const { return bTick; } //�ж��ǲ�����ҪTick
protected:
	bool bTick;
};

extern vector<CCoreMinimalObject*> GObjects;//ȫ�ֶ���