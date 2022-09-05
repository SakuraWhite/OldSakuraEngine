#pragma once 
#include "../../EngineMinimal.h"


namespace ShaderType //Shader类型空间
{
	struct FShaderMacro //Shader宏
	{
		std::string Name; //名称
		std::string Definition;//定义
	};

	//是否可以转成D3D的shader宏结构的开关
	bool ToD3DShaderMacro(const vector<FShaderMacro>& InShaderMacro, vector<D3D_SHADER_MACRO>& OutD3DMacro);
}