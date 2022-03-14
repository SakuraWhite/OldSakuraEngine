#include "EngineMinimal.h" //调用库
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

//为了美观做了一个初始化

//初始化函数
int Init(CEngine* InEngine, HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(_WIN32)
	FWinMainCommandParameters winMainParameters(hInstance, prevInstance, cmdLine, showCmd);
#endif

	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
		winMainParameters

#endif//判断是否为win平台 
	);//首先进行预初始化
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre initialization error,check and initialization problem.", ReturnValue); //如果预初始化失败，打印出错日志
		return ReturnValue;
	}

	ReturnValue = InEngine->Init(

#if defined(_WIN32)
		winMainParameters
#endif//判断是否为win平台 
	);//进行初始化
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initialization error,please check the initialization problem.", ReturnValue); //如果初始化失败，打印出错日志
		return ReturnValue;
	}

	ReturnValue = InEngine->PostInit();//进行初始化之后的收尾
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post initialization error,please check the initialization problem.", ReturnValue); //如果初始化结束后失败，打印出错日志
		return ReturnValue;
	}

	return ReturnValue;
}

//渲染函数
void Tick(CEngine* InEngine)
{
	float DeltaTime = 0.03f;
	InEngine->Tick(DeltaTime);//引擎进行时
	//Sleep(30);
}

//退出函数
int Exit(CEngine* InEngine)
{
	int ReturnValue = InEngine->PreExit();//预退出
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre exit failed.", ReturnValue); //预退出失败打印日志
		return ReturnValue;
	}

	ReturnValue = InEngine->Exit();//退出
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit failed.", ReturnValue); //退出失败打印日志
		return ReturnValue;
	}

	ReturnValue = InEngine->PostExit();//退出之后的收尾
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post exit failed.", ReturnValue); //退出收尾失败打印日志
		return ReturnValue;
	}

	return ReturnValue;
}

CEngine* Engine = NULL;

// hInstance 系统自己的实例
//prevInstance 上次系统启动的实例
//cmdLine 可以传递命令
//showCmd 可以显示多少条cmd
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)//使用int 窗口的时候记得设置一下
{
	int ReturnValue = 0;

	Engine = FEngineFactory::CreateEngine();

	if (Engine)
	{

		//初始化
		Init(Engine, hInstance, prevInstance, cmdLine, showCmd);
		
		MSG EngineMsg = { 0 };//消息捕捉

		//主要渲染出图
		while (EngineMsg.message != WM_QUIT) //如果传入信息是我们要退出窗口 则直接退出
		{
			//第一个传入的EngineMsg是我们的消息结构
			//第二个传入的0是被指定范围内的第一个消息
			//第三个传入的是被指定范围内的最后一个消息
			//第四个传入
			//最后一个就是当前的消息如何被处理这里是“PM_REMOVE”意思是如果我们的消息不是退出，那么我们将“EngineMsg”从队列里移除
			/*PM_NOREMOVE 消息不从队列里除掉
			* PM_REMOVE   消息从队列里删除掉
			* PM_NOYIELD  此标志使系统不释放等待调用程序空闲的线程
			* 
			* PM_QS_INPUT 处理鼠标和键盘消息
			* PM_QS_PAINT 处理画图信息
			* PM_QS_POSTMESSAGE 处理 所有被寄送的消息，包括计时器与热键。
			* PM_QS_SENDMESSAGE 处理所有发送消息
			*/
			if (PeekMessage(&EngineMsg,0,0,0,PM_REMOVE))//判断传入的消息 PeekMessage是检测消息队列里面的消息,并且把消息放入指定结构中
			{
				TranslateMessage(&EngineMsg); //消息翻译 把无符号的消息翻译为字符串
				DispatchMessage(&EngineMsg);  //调配信息，把信息发送给窗口
			}
			else
			{
				Tick(Engine);
			}
			
		}

		//退出
		ReturnValue = Exit(Engine);

	}
	else
	{
		ReturnValue = 1;
	}
	Engine_Log("[%i]The engine has exited.", ReturnValue); //引擎退出去之后打印日志
	return ReturnValue;
}