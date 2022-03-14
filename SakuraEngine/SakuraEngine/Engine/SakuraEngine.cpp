#include "EngineMinimal.h" //���ÿ�
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

//Ϊ����������һ����ʼ��

//��ʼ������
int Init(CEngine* InEngine, HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(_WIN32)
	FWinMainCommandParameters winMainParameters(hInstance, prevInstance, cmdLine, showCmd);
#endif

	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
		winMainParameters

#endif//�ж��Ƿ�Ϊwinƽ̨ 
	);//���Ƚ���Ԥ��ʼ��
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre initialization error,check and initialization problem.", ReturnValue); //���Ԥ��ʼ��ʧ�ܣ���ӡ������־
		return ReturnValue;
	}

	ReturnValue = InEngine->Init(

#if defined(_WIN32)
		winMainParameters
#endif//�ж��Ƿ�Ϊwinƽ̨ 
	);//���г�ʼ��
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initialization error,please check the initialization problem.", ReturnValue); //�����ʼ��ʧ�ܣ���ӡ������־
		return ReturnValue;
	}

	ReturnValue = InEngine->PostInit();//���г�ʼ��֮�����β
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post initialization error,please check the initialization problem.", ReturnValue); //�����ʼ��������ʧ�ܣ���ӡ������־
		return ReturnValue;
	}

	return ReturnValue;
}

//��Ⱦ����
void Tick(CEngine* InEngine)
{
	float DeltaTime = 0.03f;
	InEngine->Tick(DeltaTime);//�������ʱ
	//Sleep(30);
}

//�˳�����
int Exit(CEngine* InEngine)
{
	int ReturnValue = InEngine->PreExit();//Ԥ�˳�
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre exit failed.", ReturnValue); //Ԥ�˳�ʧ�ܴ�ӡ��־
		return ReturnValue;
	}

	ReturnValue = InEngine->Exit();//�˳�
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit failed.", ReturnValue); //�˳�ʧ�ܴ�ӡ��־
		return ReturnValue;
	}

	ReturnValue = InEngine->PostExit();//�˳�֮�����β
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post exit failed.", ReturnValue); //�˳���βʧ�ܴ�ӡ��־
		return ReturnValue;
	}

	return ReturnValue;
}

CEngine* Engine = NULL;

// hInstance ϵͳ�Լ���ʵ��
//prevInstance �ϴ�ϵͳ������ʵ��
//cmdLine ���Դ�������
//showCmd ������ʾ������cmd
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)//ʹ��int ���ڵ�ʱ��ǵ�����һ��
{
	int ReturnValue = 0;

	Engine = FEngineFactory::CreateEngine();

	if (Engine)
	{

		//��ʼ��
		Init(Engine, hInstance, prevInstance, cmdLine, showCmd);
		
		MSG EngineMsg = { 0 };//��Ϣ��׽

		//��Ҫ��Ⱦ��ͼ
		while (EngineMsg.message != WM_QUIT) //���������Ϣ������Ҫ�˳����� ��ֱ���˳�
		{
			//��һ�������EngineMsg�����ǵ���Ϣ�ṹ
			//�ڶ��������0�Ǳ�ָ����Χ�ڵĵ�һ����Ϣ
			//������������Ǳ�ָ����Χ�ڵ����һ����Ϣ
			//���ĸ�����
			//���һ�����ǵ�ǰ����Ϣ��α����������ǡ�PM_REMOVE����˼��������ǵ���Ϣ�����˳�����ô���ǽ���EngineMsg���Ӷ������Ƴ�
			/*PM_NOREMOVE ��Ϣ���Ӷ��������
			* PM_REMOVE   ��Ϣ�Ӷ�����ɾ����
			* PM_NOYIELD  �˱�־ʹϵͳ���ͷŵȴ����ó�����е��߳�
			* 
			* PM_QS_INPUT �������ͼ�����Ϣ
			* PM_QS_PAINT ����ͼ��Ϣ
			* PM_QS_POSTMESSAGE ���� ���б����͵���Ϣ��������ʱ�����ȼ���
			* PM_QS_SENDMESSAGE �������з�����Ϣ
			*/
			if (PeekMessage(&EngineMsg,0,0,0,PM_REMOVE))//�жϴ������Ϣ PeekMessage�Ǽ����Ϣ�����������Ϣ,���Ұ���Ϣ����ָ���ṹ��
			{
				TranslateMessage(&EngineMsg); //��Ϣ���� ���޷��ŵ���Ϣ����Ϊ�ַ���
				DispatchMessage(&EngineMsg);  //������Ϣ������Ϣ���͸�����
			}
			else
			{
				Tick(Engine);
			}
			
		}

		//�˳�
		ReturnValue = Exit(Engine);

	}
	else
	{
		ReturnValue = 1;
	}
	Engine_Log("[%i]The engine has exited.", ReturnValue); //�����˳�ȥ֮���ӡ��־
	return ReturnValue;
}