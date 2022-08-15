#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"

//��̬����������
struct FStaticSamplerObject :public IDirectXDeviceInterfece_Struct
{
	FStaticSamplerObject();
	//������̬����
	void BuildStaticSampler();

	CD3DX12_STATIC_SAMPLER_DESC* GetData();//��ȡ����̬�����е�����
	int GetSize()const;//��ȡ����

private:
	//������̬������ʽ
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> SamplerDescs;//��������
};