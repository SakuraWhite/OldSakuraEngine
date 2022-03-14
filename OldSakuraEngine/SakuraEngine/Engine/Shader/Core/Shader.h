#pragma once//防止重复包含
#include "../../EngineMinimal.h"//包含基础的头文件


class FShader //Shader类 包含两大元素：顶点着色器，像素着色器
{
public:
	//暴露二进制指针
	LPVOID  GetBufferPointer();
	SIZE_T GetBufferSize();


	void BuildShaders(				 //接口 通过接口区去编译Shader代码
		const wstring& InFinleName,	 //当前Shader代码在磁盘的那个位置
		const string& InEntryFunName,//Shader函数的进入点， 相当于函数的Mian语言
		const string& InShadersVersion);//Shader的版本
private:
	ComPtr<ID3DBlob> ShaderCode; //Shader代码区
};