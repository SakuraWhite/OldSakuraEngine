//摄像机
#pragma once//防止重复包含
#include "Viewport/Viewport.h"
#include "../Actor/Core/ActorObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterfece.h"


//前置声明
enum ECmeraType;
struct FInputKey;
class CInputComponent;

//C开头的Camera  C是核心意思
class GCamera :
	public GActorObject//继承自GActorObject
	,public FViewport//继承自Fviewport
	,public IDirectXDeviceInterfece//继承IDirectXDeviceInterfece 拿到视口句柄
{

	
	CVARIABLE()  //反射宏
	CInputComponent* InputComponent;//输入输出控制
public:   //构造
	GCamera();
	//初始化
	virtual void BeginInit();
	//输入进行
	virtual void Tick(float DeltaTime);
	//输出绑定
	virtual void ExecuteKeyboard(const FInputKey& InputKey);

	//接口 用作构建屏幕空间矩阵  
	virtual void BuildVewMatrix(float DeltaTime);

public:
	//鼠标输入的接口  
	virtual void OnMouseButtonDown(int X, int Y);
	virtual void OnMouseButtonUp(int X, int Y);
	virtual void OnMouseMove(int X, int Y);
	virtual void OnMouseWheel(int X, int Y, float InDelta);//滚轮

	//键盘的接口 控制前进后退 左右移动
	virtual void MoveForward(float InValue);//前进后退
	virtual void MoveRight(float InValue);//前进后退
protected:
	//绕着Y轴旋转
	void RotateAroundYAxis(float InRotateDegrees);
	//绕着Z轴旋转
	void RotateAroundZAxis(float InRotateDegrees);
public:

	//使用FORCEINLINE来暴漏一些参数接口 作为键盘或者鼠标输入的相应   FORCEINLINE是属于windows平台定义的内容 需要包含关与windows平台
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }

protected:
	POINT LastMousePosition;//鼠标点击的上一个位置  用来做鼠标的拖动

	// 鼠标右键 摁下拖拽的时候会触发此Bool
	bool bLeftMouseDown;

	//鼠标的配置  临时
	float MouseSensitivity;//鼠标灵敏度

	//相机模式选择枚举
	ECmeraType CmeraType;

	//球面坐标
	float Radius;//半径
	float A;//球面坐标中的θ角度 Theta 
	float B;//球面坐标中的φ角度
};