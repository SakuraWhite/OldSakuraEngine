#pragma once//防止重复包含


#if defined(_WIN32) //判断是否是windows平台 如果定义了win32，那么则是win平台
#include "../EngineMinimal.h"//包含头文件
class FWinMainCommandParameters //创建类
{
public:
	FWinMainCommandParameters(HINSTANCE InhInstance, HINSTANCE InprevInstance, PSTR IncmdLine, int InshowCmd);//传递信息需要包含头文件
	
	HINSTANCE HInstance;//保存参数 与上面传递来的信息做区分，首字母大写
	HINSTANCE PrevInstance;//同上
	PSTR CmdLine;//同上
	int ShowCmd;//同上
};
#elif 0

#endif // 0

