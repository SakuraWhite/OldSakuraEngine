#include "ShaderType.h"

bool ShaderType::ToD3DShaderMacro(const vector<FShaderMacro>& InShaderMacro, vector<D3D_SHADER_MACRO>& OutD3DMacro)
{
	for (auto& Tmp : InShaderMacro)//遍历shader宏
	{
		D3D_SHADER_MACRO ShaderMacro =   //D3DSahder宏定义
		{
			Tmp.Name.c_str(), //名字
			Tmp.Definition.c_str(),//定义
		};

		OutD3DMacro.push_back(ShaderMacro);//回退shader宏

	}

	if (OutD3DMacro.size() > 0) //判断size  如果宏里面的值大于0 则给空值
	{
		D3D_SHADER_MACRO ShaderMacro =
		{
			NULL,
			NULL,
		};

		OutD3DMacro.push_back(ShaderMacro);
	}

	return OutD3DMacro.size();
}
