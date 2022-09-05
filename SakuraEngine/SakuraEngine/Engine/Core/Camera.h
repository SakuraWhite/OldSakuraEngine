//�����
#pragma once//��ֹ�ظ�����
#include "Viewport/Viewport.h"
#include "Viewport/ClientViewport.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterfece.h"


//ǰ������
enum ECmeraType;
struct FInputKey;
class CInputComponent;

//C��ͷ��Camera  C�Ǻ�����˼
class GCamera :
	public GClientViewport //�ͻ����ӿ�
	,public IDirectXDeviceInterfece//�̳�IDirectXDeviceInterfece �õ��ӿھ��
{
	typedef GClientViewport Super; //����ͻ����ӿ�
	
	CVARIABLE()  //�����
	CInputComponent* InputComponent;//�����������
public:   //����
	GCamera();
	//��ʼ��
	virtual void BeginInit();
	//�������
	virtual void Tick(float DeltaTime);
	//�����
	virtual void ExecuteKeyboard(const FInputKey& InputKey);

	//�ӿ� ����������Ļ�ռ����  
	virtual void BuildViewMatrix(float DeltaTime);

public:
	//�������Ľӿ�  
	virtual void OnMouseButtonDown(int X, int Y);
	virtual void OnMouseButtonUp(int X, int Y);
	virtual void OnMouseMove(int X, int Y);
	virtual void OnMouseWheel(int X, int Y, float InDelta);//����

	//���̵Ľӿ� ����ǰ������ �����ƶ�
	virtual void MoveForward(float InValue);//ǰ������
	virtual void MoveRight(float InValue);//ǰ������
protected:
	//����X����ת
	void RotateAroundXAxis(float InRotateDegrees);
	//����Y����ת
	void RotateAroundYAxis(float InRotateDegrees);
public:

	//ʹ��FORCEINLINE����©һЩ�����ӿ� ��Ϊ���̻�������������Ӧ   FORCEINLINE������windowsƽ̨��������� ��Ҫ��������windowsƽ̨
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }

protected:
	POINT LastMousePosition;//���������һ��λ��  �����������϶�

	// ����Ҽ� ������ק��ʱ��ᴥ����Bool
	bool bLeftMouseDown;

	//��������  ��ʱ
	float MouseSensitivity;//���������

	//���ģʽѡ��ö��
	ECmeraType CmeraType;

	//��������
	float Radius;//�뾶
	float A;//���������еĦȽǶ� Theta 
	float B;//���������еĦսǶ�
};