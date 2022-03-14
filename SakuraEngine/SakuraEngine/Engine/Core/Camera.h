//�����
#pragma once//��ֹ�ظ�����
#include "Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"



//ǰ������
struct FInputKey;
class CTransformationComponent;
class CInputComponent;

//C��ͷ��Camera  C�Ǻ�����˼ �̳���CCoreMinimalObject��Fviewport
class CCamera :public CCoreMinimalObject,public FViewport
{
	CVARIABLE()  //�����
	CTransformationComponent* TransformationComponent;//����ϣ��������ܹ��ƶ� ����Ҫ�������
	
	CVARIABLE()  //�����
	CInputComponent* InputComponent;//�����������
public:   //����
	CCamera();
	//��ʼ��
	virtual void BeginInit();
	//�������
	virtual void Tick(float DeltaTime);
	//�����
	virtual void ExecuteKeyboard(const FInputKey& InputKey);

public:

	//ʹ��FORCEINLINE����©һЩ�����ӿ� ��Ϊ���̻�������������Ӧ   FORCEINLINE������windowsƽ̨��������� ��Ҫ��������windowsƽ̨
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
	FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }

};