#include "RenderingTextureResourcesUpdate.h"


//��װ�ַ��� 
const wchar_t DDS[] = L".dds";
const wchar_t Asset[] = L"/Asset/";
const wchar_t Project[] = L"/Project/";

void FRenderingTextureResourcesUpdate::LoadTextureResources(const wstring& InFilename)
{
	//����ȫ��Ψһָ��
	unique_ptr<FRenderingTexture> MyTexture = std::make_unique<FRenderingTexture>();
	//��¼�ļ���
	MyTexture->Filename = InFilename;


	//��¼��ͼ��
	//����һ���ļ���·������ c:/a/b/c/xxx.jpg Ȼ����ȡ�ļ���xxx.jpg
	wchar_t Filename[1024] = { 0 };//C����
	get_path_clean_filename_w(Filename, MyTexture->Filename.c_str());//ת��C���Ե�string
	wremove_string_start(Filename, DDS);//�Ƴ��ļ�����׺��.dds 

	MyTexture->Name = Filename;//��¼��ͼ����

	//��ȡDDS��ͼ����
	CreateDDSTextureFromFile12(			//����DDS��ͼ(֧��dx12)
		GetD3dDevice().Get(),			//���� ָ��D3D�����豸
		GetGraphicsCommandList().Get(), //�������(�б�)�������ύGPU����
		MyTexture->Filename.c_str(),	//������ǰ�ļ���·���Լ��ļ��� �ǵ�ת������תΪ���ַ�
		MyTexture->Data,				//�ļ����� ��Դ
		MyTexture->UploadBuffer);		//�ϴ�������

	//�Զ��ֲ�ID   ͨ�������е����� ������������ͼID�ж���
	MyTexture->RenderingTextureID = TexturesMapping.size();


	//ƴ���ַ��� "Texture'Asset/Texture/xxx.xxx'"
	//����ͼ����Դ·��������ֵ
	wchar_t AssetFilenameBuff[1024] = { 0 };//ת��Ϊ���ַ�
	{
		wchar_t AssetFilenameBuff1[1024] = { 0 };//��Դ���ֻ�����1
		wchar_t* AssetFilenamePtr = const_cast<wchar_t*>(MyTexture->Filename.c_str());//��ȡ��Դָ��

		int Pos = wfind_string(AssetFilenamePtr, Asset);//Ѱ����Դ�е��ַ��� L"/Asset/"

		//�и��ַ���						��Դָ��				��Դ���ֻ���  ��λ���и�    �ַ�������
		wchar_t* Value = wstring_mid(AssetFilenamePtr, AssetFilenameBuff1, Pos, wcslen(AssetFilenamePtr));//�и������'Asset/Texture/xxx.xxx'

		//��ʼ�滻�ַ���
		wreplace_string_inline(Value, Asset, Project);//���и��ַ�����λ�ÿ�ʼ �滻L"/Asset/"  L"/Project/"
		//�滻�ڶ���
		wreplace_string_inline(Value, DDS, (L"." + MyTexture->Name).c_str());//��L".dds"���ļ������кϲ� ����ֻҪ��XXX.XXX ����XXX.dds

		//��ӡ�ַ���
		wget_printf_s(AssetFilenameBuff, L"Texture'%s'", Value);//���ַ����ṹ��Ϊ"Texture'Asset/Texture/xxx.xxx'" 
	}
	//������ƴ�Ӻõ��ַ�����������Դ��
	MyTexture->AssetFilename = AssetFilenameBuff;

	//����ͼ�����뵽������
	TexturesMapping[MyTexture->Name] = std::move(MyTexture);//��Ϊ��Ψһָ��  ֻ��ͨ��move���ƶ���ͼ��
}

void FRenderingTextureResourcesUpdate::BuildTextureConstantBuffer(ID3D12DescriptorHeap* InHeap, int Offset)
{
	//������ƫ����
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//DX12 CPU���������
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(InHeap->GetCPUDescriptorHandleForHeapStart());
	//���������ƫ��
	Handle.Offset(Offset, DescriptorOffset);


	//����Shader��Դ��ͼ���� ����������ǰ��Դ��ʲô���� ��ʽ��ʲô�ȵ���Ϣ
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc = {};
	ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//Ĭ����ɫ�����ӳ��  ����ͼƬRGB����ʱ���Խ������� ����RG���� ����GB������
	ShaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//������ǰshader��ͼ��ʽ��Ϣ
	ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//����������Դ��ά�� ������2D�����ʽ
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;//��ǰmipmap������ 
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;      //mipmap�Ĳ㼶����
	ShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.f;//mipmap�㼶���� ָ����С��mipmap�㼶 ����Ϊ0���Է�������mipmap�㼶

	for (auto &Tmp:TexturesMapping)//������Դ
	{
		ShaderResourceViewDesc.Format = Tmp.second->Data->GetDesc().Format;//ͨ����Դ���ݻ�ȡ����  ��ͨ��������ȡ��ʽ
		ShaderResourceViewDesc.Texture2D.MipLevels = Tmp.second->Data->GetDesc().MipLevels;//ͨ����Դ���ݻ�ȡ����  ����Դ������ȡmipmap�Ĳ㼶
		GetD3dDevice()->CreateShaderResourceView(//ͨ��D3D��������shader��Դ��ͼ
			Tmp.second->Data.Get(),		//�����Դ����
			&ShaderResourceViewDesc,	//��Դ���� 
			Handle);					//��ַƫ��
		Handle.Offset(1, DescriptorOffset);//ÿ�δ���һ��shader��Դ��ͼ ��ַ����ƫ��1��λ��
	}



}

std::unique_ptr<FRenderingTexture>* FRenderingTextureResourcesUpdate::FindRenderingTexture(const std::string& InKey)
{
	if (!InKey.empty())//�ж�·���Ƿ�Ϊ��  �����Ϊ��
	{

		//��һ��Ѱ��·��
		//��·��ת�����ַ�
		const char* InString = InKey.c_str();
		wchar_t TexturePath[1024] = { 0 };
		char_to_wchar_t(TexturePath, 1024, InString); //ת�����ַ� ������ַ�  ���ַ����� ��ǰ�ַ���

		if (TexturesMapping.find(TexturePath) != TexturesMapping.end())//ͨ��·��kay�ж��Ƿ����ҵ��ṹ    ������ڴ���û���ҵ�
		{
			//����ҵ��ṹ����ͼ·�� �򷵻�  ��Ψһָ�룩
			return &TexturesMapping[TexturePath];//key����·��

		}
		else
		{
			//���û���ҵ��ṹ�е���ͼ·��  ���������һ�ַ�ʽ ����
			for (auto& Tmp : TexturesMapping)//���� �Ƚϵ�Ч
			{
				if (Tmp.second->Filename == TexturePath)//�ж��ļ���
				{
					//����·����
					return &Tmp.second;
				}

				if (Tmp.second->AssetFilename == TexturePath)//�ж���Դ·��
				{
					//����·����
					return &Tmp.second;
				}

				if (Tmp.second->SimpleAssetFilename == TexturePath)//�жϼ򵥵���Դ·��
				{
					//����·����
					return &Tmp.second;
				}
			}
		}
	}

	return NULL;
}
