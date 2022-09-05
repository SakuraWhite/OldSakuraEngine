#pragma once 
#include "../../EngineMinimal.h"


namespace ShaderType //Shader���Ϳռ�
{
	struct FShaderMacro //Shader��
	{
		std::string Name; //����
		std::string Definition;//����
	};

	//�Ƿ����ת��D3D��shader��ṹ�Ŀ���
	bool ToD3DShaderMacro(const vector<FShaderMacro>& InShaderMacro, vector<D3D_SHADER_MACRO>& OutD3DMacro);
}