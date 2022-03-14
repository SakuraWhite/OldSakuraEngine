#include "EngineFactory.h"//����EngineFactory.h�ļ�
#if defined(_WIN32)
#include "Platform/Windows/WindowsEngine.h"//����"WindowsEngine.h"�ļ���
#endif


FEngineFactory::FEngineFactory() //����EngineFactory.h���еġ�FEngineFactory�����ڴ��ļ��б�ɳ�Ա����
{

}

CEngine* FEngineFactory::CreateEngine()
{
#if defined(_WIN32)
	return new CWindowsEngine();
#elif defined(__linux__)//�����Linux����Linux�������

#endif //DEBUG

}
