#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{


}

void FDirectXRootSignature::BuildRootSignature()
{
	//��ǩ������  //��ǩ��������Shader���� ��ǩ�����԰�����ɫ���ṩ��Ⱦ��ˮ�������������Դ
	CD3DX12_ROOT_PARAMETER RootParam[2];//������������ǩ������һ��������ռ���� һ�����ӿ�ͶӰ����


	//�����CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;//������������CBV�ı�
	//Param��ʼ��
	DescriptorRangeObjCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //ָ����ΧΪCBV   CBV=������������ͼ
		1, 0);//�ڶ����������ж��ٸ�������  �����������ǻ�����һ����ɫ���ļĴ��� ������0��


	//ͶӰ�����CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;//������������CBV�ı�
	//Param��ʼ��
	DescriptorRangeViewportCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //ָ����ΧΪCBV   CBV=������������ͼ
		1, 1);//�ڶ����������ж��ٸ�������  �����������ǻ�����һ����ɫ���ļĴ��� ������1��

	RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeObjCBV);//��һ�������ǵ�ǰ�ķ�Χ �ڶ���ָ����Ӧ������
	RootParam[1].InitAsDescriptorTable(1, &DescriptorRangeViewportCBV);//��һ�������ǵ�ǰ�ķ�Χ �ڶ���ָ����Ӧ������

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		2,//ָ���ж��ٸ�����
		RootParam,//ָ���Ĳ���
		0,//�ж��ٸ���̬����
		nullptr,//ָ����̬����ʵ��
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
