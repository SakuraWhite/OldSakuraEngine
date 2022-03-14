#pragma once

//防止无限进行递归
#define ANALYSIS_HRESULT(InValue)  \
{\
		HRESULT HandleResult = InValue;\
		if (FAILED(HandleResult))\
		{\
			Engine_Log_Error("Error = %i",(int)HandleResult);\
			assert(0);\
		}\
		else if (SUCCEEDED(HandleResult))\
		{\
			Engine_Log_Success("Success !");\
		}\
}			