#pragma once//防止重复包含
#include "Camera.h"
#include "CameraType.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"


GCamera::GCamera()
	: Super()//定义反射
{
	InputComponent = CreateObject<CInputComponent>(new CInputComponent());
	//配置灵敏度
	MouseSensitivity = 1.f;

	//相机初始化的时候默认为相机漫游模式
	CmeraType = ECmeraType::CameraRoaming;

	//球面坐标初始化
	Radius = 10.f;     //球面坐标半径
	A = XM_PI;   //球面坐标的θ 初始化为Π = 180°
	B = XM_PI;   //球面坐标的φ 初始化为Π = 180°
}

void GCamera::BeginInit()
{
	
	ViewportInit();//执行初始化  初始化投影矩阵 相机

	//做输入绑定
	InputComponent->CaptureKeyboardInforDelegate.Bind(this, &GCamera::ExecuteKeyboard);
	InputComponent->OnMouseButtonDownDelegate.Bind(this, &GCamera::OnMouseButtonDown);
	InputComponent->OnMouseButtonUpDelegate.Bind(this, &GCamera::OnMouseButtonUp);
	InputComponent->OnMouseMoveDelegate.Bind(this, &GCamera::OnMouseMove);
	InputComponent->OnMouseWheelDelegate.Bind(this, &GCamera::OnMouseWheel);
}

void GCamera::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);//反射ClientViewport.h中的帧执行

}

void GCamera::ExecuteKeyboard(const FInputKey& InputKey)
{
	if (InputKey.KeyName == "W") //判断摁键
	{
		MoveForward(1.f);//如果判定摁键为W 那么传入前后移动为 正1  为前进

		//脏标记
		SetDirty(true);
	}
	else if (InputKey.KeyName == "S")
	{
		MoveForward(-1.f);//如果判定摁键为S 那么传入前后移动为 负1  为后退
		//脏标记
		SetDirty(true);
	}
	else if (InputKey.KeyName == "A")
	{
		MoveRight(-1.f);//如果判定摁键为A 那么传入左右移动为 正1  为向右移动
		//脏标记
		SetDirty(true);
	}
	else if (InputKey.KeyName == "D")
	{
		MoveRight(1.f);//如果判定摁键为A 那么传入左右移动为 负1  为向左移动
		//脏标记
		SetDirty(true);
	}
	else if (InputKey.KeyName == "Q")
	{
		CmeraType = ECmeraType::ObservationObject;//如果我们摁下Q 那么摄像机就会被设定为物体对象观察模式
		//脏标记
		SetDirty(true);
	}
	else if (InputKey.KeyName == "E")
	{
		CmeraType = ECmeraType::CameraRoaming;//如果我们摁下E 那么摄像机就会被设定为相机漫游模式
		//脏标记
		SetDirty(true);
	}
}

void GCamera::BuildViewMatrix(float DeltaTime)
{
	//更新矩阵
	//相机模式的开关 选择世界漫游矩阵还是物体观察模式矩阵
	switch (CmeraType)
	{
		case CameraRoaming: //世界漫游矩阵
		{
			Super::BuildViewMatrix(DeltaTime);//反射ClientViewport.h中的构建视口矩阵
		
			break;
		}	
		case ObservationObject: //物体对象观察模式
		{
			//计算球形坐标 来实现物体对象观察模式


			XMFLOAT3& CameraPos = GetTransformationComponent()->GetPosition();//首先拿到相机的位置
			//拿到相机的位置之后 将相机的XYZ带入球形坐标   
		       
			CameraPos.x = Radius * sinf(B) * cosf(A);//x = r * sinθ * cosφ
			CameraPos.z = Radius * sinf(B) * sinf(A);//z = r * sinθ * sinφ
			CameraPos.y = Radius * cosf(B);//y = r * cosθ
			
			//屏幕视口相关
			XMVECTOR Pos = XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 1.0f); //给定齐次坐标
			XMVECTOR ViewTarget = XMVectorZero();//屏幕能看到的目标  这里是0点，还没有办法拿到模型的坐标
			XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);//头顶指向上方

			XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);//屏幕看到的方向 矩阵
			XMStoreFloat4x4(const_cast<XMFLOAT4X4*>(&ViewMatrix), ViewLookAt);//转换为屏幕空间
			break;
		}

	}
	
}


void GCamera::OnMouseButtonDown(int X, int Y)
{
	//摁下鼠标的时候 鼠标判定为true
	bLeftMouseDown = true;

	//记录上一次鼠标点击的位置
	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetCapture(GetMianWindowsHandle());//摁键捕获

	//脏标记
	SetDirty(true);

}

void GCamera::OnMouseButtonUp(int X, int Y)
{
	//松开鼠标的时候 鼠标判定为false
	bLeftMouseDown = false;


	ReleaseCapture();//松开释放摁键捕获

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	//脏标记
	SetDirty(true);

}

void GCamera::OnMouseMove(int X, int Y)
{
	if (bLeftMouseDown)//判定是否鼠标移动
	{
		//计算弧度       DX12API作用是转成弧度   当前的X值减去鼠标拖动的上一帧的值   鼠标的灵敏度
		float XRadians = XMConvertToRadians((float)(X - LastMousePosition.x) * MouseSensitivity);
		float YRadians = XMConvertToRadians((float)(Y - LastMousePosition.y) * MouseSensitivity);

		switch (CmeraType)//自由漫游模式与物体观察模式开关
		{
			case CameraRoaming: //如果为漫游模式
			{
				RotateAroundXAxis(YRadians);//更新Y轴
				RotateAroundYAxis(XRadians);//更新Z轴
				break;
			}

			case ObservationObject://如果为物体观察模式
			{
				A += (-XRadians);//用θ加上Y弧度
				B += YRadians;//用φ减去X弧度

				B = math_libray::Clamp(B, 0.f, XM_2PI * 2);//限制旋转角度到360

				break;
			}

		}
	}

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	//脏标记
	SetDirty(true);

}

void GCamera::OnMouseWheel(int X, int Y, float InDelta)
{
	if (CmeraType == ECmeraType::ObservationObject)//判定是否属于物体观察模式
	{
		Radius += (-InDelta / 80.f);//缩放

		//限制在一定的范围内
		Radius = math_libray::Clamp(Radius, 7.f, 40.f);
	}

	//脏标记
	SetDirty(true);
}

void GCamera::MoveForward(float InValue)
{
	
	//前后移动
	//判断是否为漫游模式
	if (CmeraType == ECmeraType::CameraRoaming)
	{
		//位置
		XMFLOAT3 AT3Position = GetTransformationComponent()->GetPosition();
		//前向朝向方向
		XMFLOAT3 AT3ForwardVector = GetTransformationComponent()->GetForwardVector();

		//转换结构
		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 0.5f);//按照16字节对其
		//求出前向朝向的位置
		XMVECTOR Forward = XMLoadFloat3(&AT3ForwardVector);
		//位置
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		//按照正面朝向方向进行偏移移动
		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Forward, Position));
		//按照上面计算完成后再去设置我们的位置
		GetTransformationComponent()->SetPosition(AT3Position);
	}
}

void GCamera::MoveRight(float InValue)
{

	//左右移动
	//判断是否为漫游模式
	if (CmeraType == ECmeraType::CameraRoaming)
	{
		//位置
		XMFLOAT3 AT3Position = GetTransformationComponent()->GetPosition();
		//右向朝向方向
		XMFLOAT3 AT3RightVevtor = GetTransformationComponent()->GetRightVector();

		//转换结构
		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 0.5f);//按照16字节对其
		//求出右向朝向的位置
		XMVECTOR Right = XMLoadFloat3(&AT3RightVevtor);
		//位置
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		//按照右面朝向方向进行偏移移动
		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Right, Position));
		//按照上面计算完成后再去设置我们的位置
		GetTransformationComponent()->SetPosition(AT3Position);
	}

}

void GCamera::RotateAroundXAxis(float InRotateDegrees)
{   //绕物体X轴旋转

	//当旋转一个轴的时候 其他的轴应相应的旋转
	//获取相机的正向 朝上 朝右的方向
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
	XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();
	XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();

	//拿到关于X的旋转矩阵  使用摄像机自身的X轴来求出矩阵 X轴是物体朝右方向    指定摄像机自身的Y轴
	XMMATRIX RotationX = XMMatrixRotationAxis(XMLoadFloat3(&GetTransformationComponent()->GetRightVector()), InRotateDegrees);


	//计算各个方向和按照X轴旋转后的最终效果
	//注掉Y轴 防止影响
// 	XMStoreFloat3(//储存浮点数的API		
// 		&TransformationComponent->GetRightVector(), //算出的东西赋值给旋转方向
// 		XMVector3TransformNormal(XMLoadFloat3(&RightVector),//DXAP乘法然后单位化
// 		RotationY));//把相机方向转换类型 然后去乘以拿到的X轴矩阵
	XMStoreFloat3(&GetTransformationComponent()->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationX));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationX));

}

void GCamera::RotateAroundYAxis(float InRotateDegrees)
{	//绕物体Y轴旋转

	//当旋转一个轴的时候 其他的轴应相应的旋转
	//获取相机的正向 朝上 朝右的方向
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
	XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();
	XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();

	//拿到关于Y的旋转矩阵		求出绕Y轴旋转矩阵         Y轴是物体朝上方向
	XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

	//计算各个方向和按照Y轴旋转后的最终效果
	XMStoreFloat3(//储存浮点数的API		
		&GetTransformationComponent()->GetRightVector(), //算出的东西赋值给旋转方向
		XMVector3TransformNormal(XMLoadFloat3(&RightVector),//DXAP乘法然后单位化
			RotationY));//把相机方向转换类型 然后去乘以拿到的Y轴矩阵
	XMStoreFloat3(&GetTransformationComponent()->GetUPVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationY));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationY));
}
