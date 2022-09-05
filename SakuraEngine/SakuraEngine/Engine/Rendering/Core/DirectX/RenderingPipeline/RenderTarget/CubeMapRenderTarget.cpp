#include "CubeMapRenderTarget.h"
#include "../../../../../Debug/EngineDebug.h"

FCubeMapRenderTarget::FCubeMapRenderTarget()
	: Width(256)   //��ʼ����ȾĿ���ӿڵĿ�
	, Height(256) //��ʼ����ȾĿ���ӿڵĸ�
	, Format(DXGI_FORMAT_R8G8B8A8_UNORM)//��ʼ���洢��ȾĿ���ӿڵ���ͼ��ʽ
{
	//������ȾĿ�� �ӿ������Ϣ
	ResetViewport(Width, Height);
	//�����ӿڲü������Ϣ
	ResetScissorRect(Width, Height);

	//����6��CPU��ȾĿ���ӿ�  ��������6����������ӿ�
	CPURenderTargetView.resize(6);

}

void FCubeMapRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
{
	//��ʼ����ʱ����У�

	Width = InWidth;  //��ȾĿ���ӿڵĿ�ֵ
	Height = InHeight; //��ȾĿ���ӿڵĸ߸�ֵ
	Format = InFormat; //��ȾĿ���ӿڵ���ͼ��ʽ��ֵ

	//�����ӿ���Ϣ �ü���Ϣ
	ResetViewport(Width, Height);
	ResetScissorRect(Width, Height);

	//��ʼ����ʱ�� ���й�����ȾĿ����ͼ
	BuildRenderTargetMap();

	//���й���shader��Դ��ͼ ������
	BuildSRVDescriptors();
	//���й�����ȾĿ����ͼ ������
	BuildRTVDescriptors();
}

void FCubeMapRenderTarget::ResetViewport(UINT InWidth, UINT InHeight)
{
	Viewport =
	{
		0.0f,//TopLeftX ���Ͻ�Xλ��
		0.0f,//TopLeftY ���Ͻ�Yλ��
		(float)InWidth,//Width ��
		(float)InHeight,//Height ��
		0.0f,//MinDepth  ��С���
		1.0f//MaxDepth  ������
	};
}

void FCubeMapRenderTarget::ResetScissorRect(UINT InWidth, UINT InHeight)
{
	ScissorRect =
	{
		0,//left ����ʼ��
		0,//top  ������ʼ��
		(LONG)InWidth,//right  ���
		(LONG)InHeight//bottom �߶�
	};
}

void FCubeMapRenderTarget::ResetRenderTarget(UINT InWidth, UINT InHeight)
{
	if (InWidth != Width || InHeight != Height) //�ж�����Ŀ�����趨�Ŀ���Ƿ�һ��
	{
		Width = InWidth;		//���¸�ֵ
		Height = InHeight;		//���¸�ֵ

		
		BuildRenderTargetMap();	//���¹�����ȾĿ����ͼ

		BuildSRVDescriptors();	//���¹���CPU��ɫ����Դ��ͼ
		BuildRTVDescriptors();	//���¹���CPU��ȾĿ����ͼ
	}
}

void FCubeMapRenderTarget::BuildRenderTargetMap()
{
	//����������Դ����
	D3D12_RESOURCE_DESC ResourceDesc;
	memset(&ResourceDesc, 0, sizeof(ResourceDesc));//����Դ�����еĶ��������

	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; //ָ����ǰ��Դ�ĸ�ʽ ��ǰ��Դ��ʲô���ӵ���Դ ������2D��ͼ
	ResourceDesc.Alignment = 0;    //���뷽ʽ
	ResourceDesc.Width = Width;    //ָ����Դ�Ŀ�
	ResourceDesc.Height = Height;  //ָ����Դ�ĸ�
	ResourceDesc.DepthOrArraySize = 6; //��Դ�������Ĵ�С �����3D��Դ�������
	ResourceDesc.MipLevels = 1;		//Mipmap�㼶
	ResourceDesc.Format = Format;	//ָ����ͼ��Դ��ʽ
	ResourceDesc.SampleDesc.Count = 1;	//������������
	ResourceDesc.SampleDesc.Quality = 0;	//������������
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; //��Դ���ַ�ʽ ����ָ������δ֪ģʽ ���ݴ������Դ�����Զ�����
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; //��־ ��Դ�Ƿ���Ա���������Ϊ��ȾĿ��ͱ�ת������

	//����Ĭ�϶�
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(  //ʹ��D3D����ָ�봴��һ��Resource��Դ
		&BufferProperties,   //����Ĭ�ϵ�һ������ Ĭ�϶�
		D3D12_HEAP_FLAG_NONE, //�ѱ�ǩ  ��������
		&ResourceDesc,       //������Դ����
		D3D12_RESOURCE_STATE_COMMON,//��Դ�ĳ�ʼ״̬ ������COMMON
		NULL,	 //
		IID_PPV_ARGS(RenderTargetMap.GetAddressOf())));//��Դָ��

}

void FCubeMapRenderTarget::BuildSRVDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};//��ɫ����Դ��ͼ����

	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//Ĭ����ɫ�����ӳ��  ����ͼƬRGB����ʱ���Խ������� ����RG���� ����GB������
	SRVDesc.Format = Format;//������ǰshader��ͼ��ʽ��Ϣ
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;//����������Դ��ά�� ��������������ͼ�����ʽ

	SRVDesc.TextureCube.MostDetailedMip = 0; //��ϸ��Ϣ
	SRVDesc.TextureCube.MipLevels = 1;   //mipmap�㼶
	SRVDesc.TextureCube.ResourceMinLODClamp = 0.f; //��ǰ����Դ��СLODǯ��


	GetD3dDevice()->CreateShaderResourceView(  //ͨ��D3D������������ɫ����Դ��ͼ
		RenderTargetMap.Get(), //������ȾĿ�����ɵ�MAP
		&SRVDesc,			//������ɫ����Դ��ͼ����
		CPUShaderResourceView);//����CPU��ɫ����Դ��ͼ
}

void FCubeMapRenderTarget::BuildRTVDescriptors()
{
	for (size_t i = 0; i < 6; i++)  //���� ��Ϊ����Ҫ��Ⱦģ��6������Ļ�����ͼ
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = { };//��ȾĿ����ͼ����

		RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;//����������Դ��ά�� ������2D��ͼ�����ʽ
		RTVDesc.Format = Format;	//������ǰshader��ͼ��ʽ��Ϣ
		RTVDesc.Texture2DArray.MipSlice = 0;	//Mipmap��Ƭ
		RTVDesc.Texture2DArray.PlaneSlice = 0;	//ƽ����Ƭ
		//MipSlice + ArraySlice * MipLevels  ��Դ�����㷨(��Ƭ) MipSlice PlaneSlice
		RTVDesc.Texture2DArray.FirstArraySlice = i;		//������Ƭ
		RTVDesc.Texture2DArray.ArraySize = 1;	//�����С  ����ÿ��RTV��ֻ��һ����ͼ

		GetD3dDevice()->CreateRenderTargetView(   //ͨ��D3D������������ȾĿ����ͼ
			RenderTargetMap.Get(),    //������ȾĿ�����ɵ�MAP
			&RTVDesc,			//������ȾĿ����ͼ����
			CPURenderTargetView[i]); //����CPU��ȾĿ����ͼ
	}

}
