#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "Core/RenderTarget.h"

//提供渲染内容的接口
class FCubeMapRenderTarget 
	:public FRenderTarget //继承渲染目标
{
	friend class FDynamicCubeMap; //动态立方体贴图 有源
public:
	FCubeMapRenderTarget();

	//初始化渲染目标的宽 高以及存储渲染目标视口的贴图格式
	void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat);

	//重置渲染目标 视口相关信息
	void ResetViewport(UINT InWidth, UINT InHeight);
	//重置视口裁剪相关信息
	void ResetScissorRect(UINT InWidth, UINT InHeight);

public:
	//重置渲染目标 渲染目标的宽高自适应
	void ResetRenderTarget(UINT InWidth, UINT InHeight);

private:
	//构建渲染目标贴图信息
	void BuildRenderTargetMap();
	//构建SRV描述符  着色器资源图
	void BuildSRVDescriptors();
	//构建RTV描述符  渲染目标视图
	void BuildRTVDescriptors();

public:
	//获取渲染目标
	FORCEINLINE ID3D12Resource* GetRenderTarget() const { return RenderTargetMap.Get(); }
	//获取渲染目标视口相关信息
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	//获取渲染目标裁剪相关信息
	FORCEINLINE D3D12_RECT GetScissorRect()const { return ScissorRect; }


private:
	UINT Width; //定义渲染目标的宽
	UINT Height;//定义渲染目标的高
	DXGI_FORMAT Format;//渲染目标之后存储的RGB格式

	D3D12_VIEWPORT Viewport;//渲染目标 视口相关信息
	D3D12_RECT ScissorRect;//渲染目标 视口裁剪相关信息

	//放数据的地方
	ComPtr<ID3D12Resource> RenderTargetMap;//渲染目标贴图  存储渲染目标视口的贴图

	//SRV 着色器资源视图
	CD3DX12_CPU_DESCRIPTOR_HANDLE CPUShaderResourceView;//CPU着色器资源视图
	CD3DX12_GPU_DESCRIPTOR_HANDLE GPUShaderResourceView;//GPU着色器资源视图

	//RTV 渲染目标视图
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> CPURenderTargetView;//CPU渲染目标视图
	//CD3DX12_GPU_DESCRIPTOR_HANDLE GPURenderTargetView[6];//GPU渲染目标视图
};