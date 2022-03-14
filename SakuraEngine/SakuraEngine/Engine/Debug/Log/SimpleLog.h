#pragma once//防止重复包含


#if defined(_WIN32)
#include "../../../simple_library/public/simple_library.h"//包含老师写好的日志库

#define Engine_Log(format,...)			log_log(format, __VA_ARGS__)				//打印日志
#define Engine_Log_Success(format,...)	log_success(format, __VA_ARGS__)  //只播放成功日志
#define Engine_Log_Error(format,...)	log_error(format, __VA_ARGS__)    //只播放出错日志
#define Engine_Log_Warning(format,...)	log_warning(format, __VA_ARGS__)  //只播放警告

#elif 0 //如果不是Win平台的话 输出其他日志
#define Engine_log(format,...) 
#define Engine_Log_Success(format,...) 
#define Engine_Log_Error(format,...) 
#define Engine_Log_Warning(format,...)

#endif