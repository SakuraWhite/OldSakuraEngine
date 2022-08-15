#pragma once//��ֹ�ظ�����
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

///////////////////////////////
//���������� 

//ǰ������
struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GCamera;
class GActorObject;

//C��ͷ��Camera  C�Ǻ�����˼ �̳���CCoreMinimalObject��Fviewport
class CWorld :public CCoreMinimalObject 
{
public:
	CWorld();

	GCamera* GetCamera() { return Camera; }

	template<class T>
	T* CreateActorObject()
	{
		T* InArray = new T();
		ActorObjects.push_back(InArray);

		return InArray;
	}

protected:
	CVARIABLE()
	GCamera* Camera;//���ʵ��

	//�洢���ǵ�Actors
	CVARIABLE()
		vector<GActorObject*> ActorObjects;

};