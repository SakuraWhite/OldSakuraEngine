#include "StaticSamplerObject.h"


FStaticSamplerObject::FStaticSamplerObject()
{
}

void FStaticSamplerObject::BuildStaticSampler()
{

	//默认静态采样参数  点采样
	SamplerDescs.push_back(					
		CD3DX12_STATIC_SAMPLER_DESC(0,		//定义零号寄存器 
			D3D12_FILTER_MIN_MAG_MIP_POINT));//定义mipmap的过滤 

	//各向异性静态采样
	SamplerDescs.push_back(				//创建静态采样器
		CD3DX12_STATIC_SAMPLER_DESC(1,	//定义1号寄存器
			D3D12_FILTER_ANISOTROPIC,	//采用各向异性过滤
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, //地址U包裹模式 默认
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, //地址v包裹模式 默认
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, //地址M包裹模式 默认
			0,	//mipmap等级
			8));//最大各向异性采样等级


}

CD3DX12_STATIC_SAMPLER_DESC* FStaticSamplerObject::GetData()
{
	//返回采样器数据
	return SamplerDescs.data();
}

int FStaticSamplerObject::GetSize() const
{
	//返回采样器索引
	return SamplerDescs.size();
}
