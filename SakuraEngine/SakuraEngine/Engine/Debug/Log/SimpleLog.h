#pragma once//��ֹ�ظ�����


#if defined(_WIN32)
#include "../../../simple_library/public/simple_library.h"//������ʦд�õ���־��

#define Engine_Log(format,...)			log_log(format, __VA_ARGS__)				//��ӡ��־
#define Engine_Log_Success(format,...)	log_success(format, __VA_ARGS__)  //ֻ���ųɹ���־
#define Engine_Log_Error(format,...)	log_error(format, __VA_ARGS__)    //ֻ���ų�����־
#define Engine_Log_Warning(format,...)	log_warning(format, __VA_ARGS__)  //ֻ���ž���

#elif 0 //�������Winƽ̨�Ļ� ���������־
#define Engine_log(format,...) 
#define Engine_Log_Success(format,...) 
#define Engine_Log_Error(format,...) 
#define Engine_Log_Warning(format,...)

#endif