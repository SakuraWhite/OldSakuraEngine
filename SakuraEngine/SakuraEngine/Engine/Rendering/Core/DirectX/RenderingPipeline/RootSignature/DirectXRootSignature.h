#pragma once//防止重复包含
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../StaticSampler/StaticSamplerObject.h"

//记录提供渲染相关的内容的接口    
struct  FDirectXRootSignature :public IDirectXDeviceInterfece_Struct //继承自DirectXDeviceInterfece
{	//根签名

	FDirectXRootSignature();

	void BuildRootSignature(UINT InTextureNum = 1);//创建根签名  传入贴图数量 如果没有贴图则占位1

	void PreDraw(float DeltaTime);//在模型里使用Rendeing.h的预渲染接口 
	void Draw(float DeltaTime);//在模型里使用Rendeing.h的渲染接口  进行时
	void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw

	ID3D12RootSignature* GetRootSignature() { return RootSignature.Get(); }    //获取根签名

private:
	//根签名
	ComPtr<ID3D12RootSignature>  RootSignature;
	//静态采样器对象
	FStaticSamplerObject StaticSamplerObject;
};