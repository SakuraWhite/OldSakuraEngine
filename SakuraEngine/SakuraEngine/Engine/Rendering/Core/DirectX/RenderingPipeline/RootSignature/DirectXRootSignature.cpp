#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{


}

void FDirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
	//��ǩ������  //��ǩ��������Shader���� ��ǩ�����԰�����ɫ���ṩ��Ⱦ��ˮ�������������Դ
	//������6����ǩ���ֱ��� ��������ռ�cbv�������ӿ�ͶӰcbv����������cbv�������ƹ�cbv�����������SRV���� ��������ͼSRV����
	CD3DX12_ROOT_PARAMETER RootParam[7];


	////object�����CBV������
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;//������������CBV�ı�
	////Param��ʼ��
	//DescriptorRangeObjCBV.Init(
	//	D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //ָ����ΧΪCBV   CBV=������������ͼ
	//	1, 0);//�ڶ����������ж��ٸ�������  �����������ǻ�����һ����ɫ���ļĴ��� ������0��
	//
	//
	////ViewportͶӰ�����CBV������
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;//������������CBV�ı�
	////Param��ʼ��
	//DescriptorRangeViewportCBV.Init(
	//	D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //ָ����ΧΪCBV   CBV=������������ͼ
	//	1, 1);//�ڶ����������ж��ٸ�������  �����������ǻ�����һ����ɫ���ļĴ��� ������1��
	//
	////Light�ƹ��CBV������
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeLightCBV;//������������CBV�ı�
	////Param��ʼ��
	//DescriptorRangeLightCBV.Init(
	//	D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //ָ����ΧΪCBV   CBV=������������ͼ
	//	1, 2);//�ڶ����������ж��ٸ�������  �����������ǻ�����һ����ɫ���ļĴ��� ������2��

	//Texture�����SRV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;//������������SRV�ı�  
	//Param��ʼ��
	DescriptorRangeTextureSRV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV, //ָ����ΧΪSRV   SRV=shaderRendering��ͼ
		InTextureNum, 1);//�ڶ����������ж��ٸ�������  �����������ǻ�����һ����ɫ���ļĴ��� ������1��
	//��������ͼ�����SRV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV;//������������SRV�ı�  
	//Param��ʼ��
	DescriptorRangeCubeMapSRV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//�ڶ����������ж��ٸ�������  �����������ǻ�����һ����ɫ���ļĴ��� ������0��


	//����������װ������������
	RootParam[0].InitAsConstantBufferView(0);//object�����CBV
	RootParam[1].InitAsConstantBufferView(1);//ViewportͶӰ�����CBV
	RootParam[2].InitAsConstantBufferView(2);//Light�ƹ��CBV
	RootParam[3].InitAsConstantBufferView(3);//Fog���CBV
	//����
	RootParam[4].InitAsShaderResourceView(0, 1);//��ʼ��Ϊ��ɫ����Դ��ͼ  ��һ���ǼĴ���λ�� �ڶ����ǼĴ����ռ�

	//2D��ͼ
	RootParam[5].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);//Texture�����SRV  ���һ��ϵ��Ϊ����
	//CUBE��������ͼ
	RootParam[6].InitAsDescriptorTable(1, &DescriptorRangeCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);

	//������̬��������
	StaticSamplerObject.BuildStaticSampler();



	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		7,//ָ���ж��ٸ�����
		RootParam,//ָ���Ĳ���
		StaticSamplerObject.GetSize(),//��̬��������
		StaticSamplerObject.GetData(),//��̬����ʵ��(���� PTR)
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);//��ǩ���Ĳ���ѡ��

	//����
	//���������л�
	ComPtr<ID3DBlob> SerializeRootSignature;
	ComPtr<ID3DBlob> ErrorBlob;//���л�����
	//���л���ǩ��
	D3D12SerializeRootSignature(
		&RootSignatureDesc, //������
		D3D_ROOT_SIGNATURE_VERSION_1,//���汾
		SerializeRootSignature.GetAddressOf(),//ָ�����������л�������
		ErrorBlob.GetAddressOf());//ָ�������Ʊ���

	//�жϱ����ǩ���Ƿ��ǿյ�
	if (ErrorBlob)
	{
		Engine_Log_Error("%s", (char*)ErrorBlob->GetBufferPointer());//��������ǿյ� ��ô��ӡ����
	}

	//ͨ������������ǩ��
	GetD3dDevice()->CreateRootSignature(
		0,//����Ե���GPU���в���
		SerializeRootSignature->GetBufferPointer(),//ָ�����л��Ļ�ȡBuffer��ָ��
		SerializeRootSignature->GetBufferSize(),////ָ�����л��Ļ�ȡBuffer��Size
		IID_PPV_ARGS(&RootSignature));//������ǩ��

}

void FDirectXRootSignature::PreDraw(float DeltaTime)
{

	//���ø�ǩ��
	GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());//��ǩ����ÿ֡����Ҫ���õ�
}

void FDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDirectXRootSignature::PostDraw(float DeltaTime)
{
}
