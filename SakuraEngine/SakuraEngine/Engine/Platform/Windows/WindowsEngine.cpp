#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Rendering.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Core/World.h"
#include "../../Rendering/Enigne/DurectX/DirectX12RenderingEngine.h"
//生成Mash头文件
#include "../../Mesh/BoxMesh.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Mesh/CylinderMesh.h"
#include "../../Mesh/ConeMesh.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Mesh/CustomMesh.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"//包含消息处理

CWindowsEngine::CWindowsEngine()
	:RenderingEngine(new CDirectX12RenderingEngine())
{

}


CWindowsEngine::~CWindowsEngine()
{
	delete RenderingEngine;
}



int CWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	//日志系统初始化
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	//预先处理命令



	Engine_Log("Engine pre initialization complete.");

	//渲染引擎初始化
	RenderingEngine->PreInit(InParameters);


	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	//处理视口
	InitWindows(InParameters);

	//初始化之前需要指定一个句柄
	RenderingEngine->SetMianWindowsHandle(MianWindowsHandle);
	//RenderingEngine开始初始化
	RenderingEngine->Init(InParameters);
	//初始化世界
	CWorld *World  = CreateObject<CWorld>(new CWorld());

	Engine_Log("Engine initialization complete.");
	
	return 0;
}

int CWindowsEngine::PostInit()
{

	
	Engine_Log("Engine post initialization complete.");

	RenderingEngine->PostInit();
	
	//键鼠控制相机初始化
	for (auto& Tmp : GObjects)
	{
		Tmp->BeginInit();
	}

	return 0;



}

void CWindowsEngine::Tick(float DeltaTime)
{
	//输入操作
	for (auto& Tmp : GObjects)
	{
		if (Tmp->IsTick()) //判断是否在tick
		{
			Tmp->Tick(DeltaTime);
		}
		
	}

	RenderingEngine->Tick(DeltaTime);

	
}

int CWindowsEngine::PreExit()
{
	RenderingEngine->PreExit();

	Engine_Log("Engine pre exit complete.");
	return 0;
}

int CWindowsEngine::Exit()
{
	RenderingEngine->Exit();

	Engine_Log("Engine exit complete.");
	return 0;
}

int CWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	RenderingEngine->PostExit();


	Engine_Log("Engine post exit complete.");
	return 0;
}






bool CWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{

	//创建 注册窗口
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);//该对象实际占用多大内存
	WindowsClass.cbClsExtra = 0;//是否需要额外空间
	WindowsClass.cbWndExtra = 0;//是否需要额外内存
	WindowsClass.hbrBackground = nullptr; //如果有设置那就是GDI差出
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//设置一个箭头光标
	WindowsClass.hIcon = nullptr; //应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = NULL; //应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance;//窗口实例
	WindowsClass.lpszClassName = L"SakuraEngine"; //窗口名字
	WindowsClass.lpszMenuName = nullptr;//设置菜单名字
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW; //怎么绘制窗口，垂直和水平重绘
	WindowsClass.lpfnWndProc = EngineWindowProc; //消息处理函数

	//注册窗口 
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom) //如果创建失败，打印消息
	{
		Engine_Log_Error("Register windows class Fail.");
		MessageBox(NULL, L"Register windows class Fail.", L"Error", MB_OK);//显示注册失败然后跳出小框
	}
	//定制窗口大小
	RECT Rect = {0,0,FEngineRenderConfig::GetRenderConfig()->ScrrenWidth,FEngineRenderConfig::GetRenderConfig()->ScrrenHight};

	
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);//&Rect视口设定 WS_OVERLAPPEDWINDOW窗口风格  NULL没有菜单栏
	//计算在我们这个显示器的分辨率下的引擎窗口大小
	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;

	MianWindowsHandle = CreateWindowEx(
		NULL,						//窗口额外风格
		L"SakuraEngine",			//窗口名称
		L"SAKURA Engine",			//显示在窗口标题栏的名称
		WS_OVERLAPPEDWINDOW,		//窗口风格
		100, 100,					//窗口坐标
		WindowWidth, WindowHight,	//上面已经算好的窗口宽高
		NULL,						//副窗口的句柄
		nullptr,					//菜单句柄
		InParameters.HInstance,		//窗口实例
		NULL);						//额外参数传入
	if (!MianWindowsHandle) //判断窗口创建是否失败了
	{
		Engine_Log_Error("CreateWindow Failed.");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//显示窗口
	ShowWindow(MianWindowsHandle, SW_SHOW);
	//显示窗口后刷新一下
	UpdateWindow(MianWindowsHandle);


	Engine_Log("InitWindows complete.");

	return true;
}
#endif