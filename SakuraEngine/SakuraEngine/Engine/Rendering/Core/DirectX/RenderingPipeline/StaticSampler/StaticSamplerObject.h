#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"

//静态采样器对象
struct FStaticSamplerObject :public IDirectXDeviceInterfece_Struct
{
	FStaticSamplerObject();
	//构建静态采样
	void BuildStaticSampler();

	CD3DX12_STATIC_SAMPLER_DESC* GetData();//获取到静态采样中的数据
	int GetSize()const;//获取索引

private:
	//构建静态采样方式
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> SamplerDescs;//采样描述
};