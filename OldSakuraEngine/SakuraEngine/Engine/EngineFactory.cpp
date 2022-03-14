#include "EngineFactory.h"//包含EngineFactory.h文件
#if defined(_WIN32)
#include "Platform/Windows/WindowsEngine.h"//包含"WindowsEngine.h"文件库
#endif


FEngineFactory::FEngineFactory() //将“EngineFactory.h”中的“FEngineFactory”类在此文件中变成成员变量
{

}

CEngine* FEngineFactory::CreateEngine()
{
#if defined(_WIN32)
	return new CWindowsEngine();
#elif defined(__linux__)//如果是Linux则走Linux相关内容

#endif //DEBUG

}
