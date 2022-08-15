#include "StaticSamplerObject.h"


FStaticSamplerObject::FStaticSamplerObject()
{
}

void FStaticSamplerObject::BuildStaticSampler()
{

	//Ĭ�Ͼ�̬��������  �����
	SamplerDescs.push_back(					
		CD3DX12_STATIC_SAMPLER_DESC(0,		//������żĴ��� 
			D3D12_FILTER_MIN_MAG_MIP_POINT));//����mipmap�Ĺ��� 

	//�������Ծ�̬����
	SamplerDescs.push_back(				//������̬������
		CD3DX12_STATIC_SAMPLER_DESC(1,	//����1�żĴ���
			D3D12_FILTER_ANISOTROPIC,	//���ø������Թ���
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, //��ַU����ģʽ Ĭ��
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, //��ַv����ģʽ Ĭ��
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, //��ַM����ģʽ Ĭ��
			0,	//mipmap�ȼ�
			8));//���������Բ����ȼ�


}

CD3DX12_STATIC_SAMPLER_DESC* FStaticSamplerObject::GetData()
{
	//���ز���������
	return SamplerDescs.data();
}

int FStaticSamplerObject::GetSize() const
{
	//���ز���������
	return SamplerDescs.size();
}
