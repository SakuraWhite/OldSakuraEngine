#pragma once//��ֹ�ظ�����
#include "../../EngineMinimal.h"//����������ͷ�ļ�


class FShader //Shader�� ��������Ԫ�أ�������ɫ����������ɫ��
{
public:
	//��¶������ָ��
	LPVOID  GetBufferPointer();
	SIZE_T GetBufferSize();


	void BuildShaders(				 //�ӿ� ͨ���ӿ���ȥ����Shader����
		const wstring& InFinleName,	 //��ǰShader�����ڴ��̵��Ǹ�λ��
		const string& InEntryFunName,//Shader�����Ľ���㣬 �൱�ں�����Mian����
		const string& InShadersVersion);//Shader�İ汾
private:
	ComPtr<ID3DBlob> ShaderCode; //Shader������
};