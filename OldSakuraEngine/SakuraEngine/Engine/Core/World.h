#pragma once//��ֹ�ظ�����
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

///////////////////////////////
//���������� 

//ǰ������
struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class CCamera;

//C��ͷ��Camera  C�Ǻ�����˼ �̳���CCoreMinimalObject��Fviewport
class CWorld :public CCoreMinimalObject 
{
public:
	CWorld();

	CVARIABLE()
	CCamera* Camera;//���ʵ��

};