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

//C��ͷ��Camera  C�Ǻ�����˼ �̳���CCoreMinimalObject��Fviewport
class CWorld :public CCoreMinimalObject 
{
public:
	CWorld();

	GCamera* GetCamera(){return Camera;}

protected:
	CVARIABLE()
	GCamera* Camera;//���ʵ��

};