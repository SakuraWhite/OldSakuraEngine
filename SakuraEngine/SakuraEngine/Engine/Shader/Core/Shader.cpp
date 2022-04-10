#include "Shader.h"


LPVOID FShader::GetBufferPointer() const
{
	return ShaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize() const
{
	return ShaderCode->GetBufferSize();
}

void FShader::BuildShaders(const wstring& InFinleName,const string& InEntryFunName,const string& InShadersVersion)//�ӿ� ͨ���ӿ���ȥ����Shader����
{
	//���������Ʊ��� ShaderError
	ComPtr<ID3DBlob> ErrorShaderMsg;
	HRESULT R = D3DCompileFromFile(					//DX12��API
		InFinleName.c_str(), //ת��Ϊ���ַ�
		NULL,	 //�ۼ��������е�ָ��
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //include���ȥ���� Ҳ����Shader���ȥ����ǰ���������ļ�
		InEntryFunName.c_str(), //�����������
		InShadersVersion.c_str(),//����Ŀ��   Ҳ����Shader�汾
#if _DEBUG	//���ڵ��Ե��������	 ���������жϵ�ǰ�ǲ�������DEBUG
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION  //�����Ż� ��ֹ��Shader���Ա���Ϊ���

#else //�������DEBUG�׶� �Ǿ����0����
0
#endif
	,0, //���ȥ����Ч����
	&ShaderCode,//�ֽڴ���
	&ErrorShaderMsg);//�����Ʊ���

	if (ErrorShaderMsg)   //�������Sahder�����Ʊ��� ��ô��ӡ�������
	{
		Engine_Log_Error("%s", (char*)ErrorShaderMsg->GetBufferPointer());
	}
	//ʧ�ܾͻ����
	ANALYSIS_HRESULT(R);
}
