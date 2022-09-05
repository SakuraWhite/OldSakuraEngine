#pragma once
#include "../../EngineMinimal.h"
#include "../../Interface/DirectXDeviceInterfece.h"

struct FRenderingTexture //��Ⱦ���������ṹ
{
	FRenderingTexture()
		:RenderingTextureID(0)//��ʼ������ID
	{}

	ComPtr<ID3D12Resource> UploadBuffer;//�����ϴ���
	ComPtr<ID3D12Resource> Data;//��������Դ

	wstring Name;//��ͼ����  'xxx.dds'
	wstring Filename;//��ͼ·��
	wstring AssetFilename;//��Դ·��	��ͼ����Դ·�� Texture'Asset/Texture/xxx.xxx'
	wstring SimpleAssetFilename;//�򵥵���Դ·��	ֱ������Դ·�� Asset/Texture/xxx.dds

	UINT RenderingTextureID;//������ͼID

};

//���������������ṹ(��ͼ)  ��Ⱦ������Դ����
class FRenderingTextureResourcesUpdate 
	:public enable_shared_from_this<FRenderingTextureResourcesUpdate>
	, public IDirectXDeviceInterfece//��Ⱦ���ݵĽӿ�

{

public:
	FRenderingTextureResourcesUpdate();


	//������ͼ��Դ				�Ӵ��̵�ĳ��λ�õ�����ͼ��Դ(·��)
	void LoadTextureResources(const wstring& InFilename);
	//����������������							�ѵĽӿ�           ��ַƫ��
	void BuildTextureConstantBuffer(ID3D12DescriptorHeap* InHeap, int Offset = 0);
public:
	//������ǰ�Ĳ��� ������ͼ���Ͳ���(Shader��Դ��ͼ������Ĳ���)
	void BuildParam(); 

	//�������ͽ�������������ͼ				      ����Ĵ������ͼ��Դ
	void ResetTextureByType(std::unique_ptr<FRenderingTexture>* InTexture);

	//��������������Դ��ά��(��������������)  ������2D������������ͼ ����3D��ͼ
	void SetViewDimension(D3D12_SRV_DIMENSION InNewDimension);

public:
	//��ȡ������ͼ����
	FORCEINLINE UINT Size() const { return TexturesMapping.size(); }

	//��ȡ������Ⱦ����·��(�ַ���)  Ψһָ��
	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& InKey);
protected:
	//   ����map		   ��ͼ��ʲô����			����Ĵ������ͼ��Դ			��ͼӳ��
	std::map<std::wstring, std::unique_ptr<FRenderingTexture>> TexturesMapping;

	//����Shader��Դ��ͼ���� ����������ǰ��Դ��ʲô���� ��ʽ��ʲô�ȵ���Ϣ
	D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
};