#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Rendering.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Core/World.h"
#include "../../Rendering/Enigne/DurectX/DirectX12RenderingEngine.h"
//����Mashͷ�ļ�
#include "../../Mesh/BoxMesh.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Mesh/CylinderMesh.h"
#include "../../Mesh/ConeMesh.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Mesh/CustomMesh.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"//������Ϣ����

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
	//��־ϵͳ��ʼ��
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	//Ԥ�ȴ�������



	Engine_Log("Engine pre initialization complete.");

	//��Ⱦ�����ʼ��
	RenderingEngine->PreInit(InParameters);


	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	//�����ӿ�
	InitWindows(InParameters);

	//��ʼ��֮ǰ��Ҫָ��һ�����
	RenderingEngine->SetMianWindowsHandle(MianWindowsHandle);
	//RenderingEngine��ʼ��ʼ��
	RenderingEngine->Init(InParameters);
	//��ʼ������
	CWorld *World  = CreateObject<CWorld>(new CWorld());

	Engine_Log("Engine initialization complete.");
	
	return 0;
}

int CWindowsEngine::PostInit()
{

	
	Engine_Log("Engine post initialization complete.");

	RenderingEngine->PostInit();
	
	//������������ʼ��
	for (auto& Tmp : GObjects)
	{
		Tmp->BeginInit();
	}

	return 0;



}

void CWindowsEngine::Tick(float DeltaTime)
{
	//�������
	for (auto& Tmp : GObjects)
	{
		if (Tmp->IsTick()) //�ж��Ƿ���tick
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

	//���� ע�ᴰ��
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);//�ö���ʵ��ռ�ö���ڴ�
	WindowsClass.cbClsExtra = 0;//�Ƿ���Ҫ����ռ�
	WindowsClass.cbWndExtra = 0;//�Ƿ���Ҫ�����ڴ�
	WindowsClass.hbrBackground = nullptr; //����������Ǿ���GDI���
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//����һ����ͷ���
	WindowsClass.hIcon = nullptr; //Ӧ�ó�����ڴ�������ʾ��ͼ��
	WindowsClass.hIconSm = NULL; //Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance;//����ʵ��
	WindowsClass.lpszClassName = L"SakuraEngine"; //��������
	WindowsClass.lpszMenuName = nullptr;//���ò˵�����
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW; //��ô���ƴ��ڣ���ֱ��ˮƽ�ػ�
	WindowsClass.lpfnWndProc = EngineWindowProc; //��Ϣ������

	//ע�ᴰ�� 
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom) //�������ʧ�ܣ���ӡ��Ϣ
	{
		Engine_Log_Error("Register windows class Fail.");
		MessageBox(NULL, L"Register windows class Fail.", L"Error", MB_OK);//��ʾע��ʧ��Ȼ������С��
	}
	//���ƴ��ڴ�С
	RECT Rect = {0,0,FEngineRenderConfig::GetRenderConfig()->ScrrenWidth,FEngineRenderConfig::GetRenderConfig()->ScrrenHight};

	
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);//&Rect�ӿ��趨 WS_OVERLAPPEDWINDOW���ڷ��  NULLû�в˵���
	//���������������ʾ���ķֱ����µ����洰�ڴ�С
	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;

	MianWindowsHandle = CreateWindowEx(
		NULL,						//���ڶ�����
		L"SakuraEngine",			//��������
		L"SAKURA Engine",			//��ʾ�ڴ��ڱ�����������
		WS_OVERLAPPEDWINDOW,		//���ڷ��
		100, 100,					//��������
		WindowWidth, WindowHight,	//�����Ѿ���õĴ��ڿ��
		NULL,						//�����ڵľ��
		nullptr,					//�˵����
		InParameters.HInstance,		//����ʵ��
		NULL);						//�����������
	if (!MianWindowsHandle) //�жϴ��ڴ����Ƿ�ʧ����
	{
		Engine_Log_Error("CreateWindow Failed.");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//��ʾ����
	ShowWindow(MianWindowsHandle, SW_SHOW);
	//��ʾ���ں�ˢ��һ��
	UpdateWindow(MianWindowsHandle);


	Engine_Log("InitWindows complete.");

	return true;
}
#endif