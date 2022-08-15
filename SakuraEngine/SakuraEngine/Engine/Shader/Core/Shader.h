#pragma once//��ֹ�ظ�����
#include "../../EngineMinimal.h"//����������ͷ�ļ�


class FShader //Shader�� ��������Ԫ�أ�������ɫ����������ɫ��
{
public:
	//��¶������ָ��
	LPVOID GetBufferPointer() const;
	SIZE_T GetBufferSize() const;


	void BuildShaders(				 //�ӿ� ͨ���ӿ���ȥ����Shader����
		const wstring& InFinleName,	 //��ǰShader�����ڴ��̵��Ǹ�λ��
		const string& InEntryFunName,//Shader�����Ľ���㣬 �൱�ں�����Mian����
		const string& InShadersVersion,//Shader�İ汾
		const D3D_SHADER_MACRO* InShaderMacro = NULL);//����  ��
private:
	ComPtr<ID3DBlob> ShaderCode; //Shader������
};