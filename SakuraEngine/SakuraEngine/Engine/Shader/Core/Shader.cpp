#include "Shader.h"


LPVOID FShader::GetBufferPointer() const
{
	return ShaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize() const
{
	return ShaderCode->GetBufferSize();
}

void FShader::BuildShaders(const wstring& InFinleName,const string& InEntryFunName,const string& InShadersVersion)//接口 通过接口区去编译Shader代码
{
	//创建二进制报错 ShaderError
	ComPtr<ID3DBlob> ErrorShaderMsg;
	HRESULT R = D3DCompileFromFile(					//DX12的API
		InFinleName.c_str(), //转化为宽字符
		NULL,	 //聚集定义阵列的指标
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //include如何去处理 也就是Shader如何去处理当前所包含的文件
		InEntryFunName.c_str(), //定义入口名字
		InShadersVersion.c_str(),//定义目标   也就是Shader版本
#if _DEBUG	//关于调试的输入参数	 所以这里判断当前是不是属于DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION  //禁用优化 防止把Shader语言编译为汇编

#else //如果不是DEBUG阶段 那就输出0即可
0
#endif
	,0, //如何去编译效果的
	&ShaderCode,//字节代码
	&ErrorShaderMsg);//二进制报错

	if (ErrorShaderMsg)   //如果上面Sahder二进制报错 那么打印这个报错
	{
		Engine_Log_Error("%s", (char*)ErrorShaderMsg->GetBufferPointer());
	}
	//失败就会崩溃
	ANALYSIS_HRESULT(R);
}
