#pragma once //防止重复包含
#include "../../Core/RenderingEngine.h"

class CMeshManage;
class CDirectXRenderingEngine :public CRenderingEngine
{
	friend class IRenderingInterface;//创建一个接口

public://公开的 初始化相关
	CDirectXRenderingEngine();
	~CDirectXRenderingEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);//预初始化 准备初始化之前的工作

	virtual int Init(FWinMainCommandParameters InParameters);//引擎的初始化
	virtual int PostInit();//初始化之后的收尾

	virtual void Tick(float DeltaTime);//Tick不断的刷新渲染引擎

	virtual int PreExit();//预退出
	virtual int Exit();//退出
	virtual int PostExit();//退出后的内存释放，也就是收尾
public:
	ID3D12Resource* GetCurrentSwapBuff() const; //快速获取SwapBuff   api
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;//快速获取SwapBuffer视图位置的api
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;//快速获取深度位置的api
public://公开的 接口相关 供光栅化使用
	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }//制作接口 纹理格式 后缓冲区
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; } //获取深度模板
	UINT GetDXGISampleCount()const;//获取采样数量 
	UINT GetDXGISampleQuality()const;//获取采样的质量
protected://受保护的 这里是做一些GPU围栏
	void WaitGPUCommandQueueComplete();

	bool InitDirect3D();//初始化Direct3D
	void PostInitDirect3D();//初始化Direct3D接口

protected:
	CMeshManage* MeshManage;
protected:
	UINT64 CurrentFenceIndex;//当前的围栏的指数，给GPU与CPU同步使用的
	int CurrentSwapBuffIndex;//当前Swap BUFF的Index 

	ComPtr<IDXGIFactory4> DXGIFactory;//创建 DirectX 图形基础结构 (DXGI) 对象
	ComPtr<ID3D12Device> D3dDevice;//创建命令分配器、命令列表、命令队列、Fence、资源、管道状态对象、堆、根签名、采样器和许多资源视图
	ComPtr<ID3D12Fence> Fence;//一个用于同步 CPU 和一个或多个 GPU 的对象。
	//创建命令队列、命令分配器、命令列表实例
	ComPtr<ID3D12CommandQueue> CommandQueue;//队列
	ComPtr<ID3D12CommandAllocator> CommandAllocator; //存储
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;//命令列表
	//交换链实例
	ComPtr<IDXGISwapChain> SwapChain;

	//描述符对象和堆
	ComPtr<ID3D12DescriptorHeap> RTVHeap;
	ComPtr<ID3D12DescriptorHeap> DSVHeap;

	vector<ComPtr<ID3D12Resource>> SwapChainBuffer;
	ComPtr<ID3D12Resource> DepthStencilBuffer;

	//和屏幕的视口有关
	D3D12_VIEWPORT ViewprotInfo;
	D3D12_RECT ViewprotRect;
protected://受保护的  抗锯齿 纹理 深度等相关
	UINT M4XQualityLevels;//主Windows句柄
	bool bMSAA4XEnabled;//多重采样质量
	DXGI_FORMAT BackBufferFormat;//4XMSAA是否开启
	DXGI_FORMAT DepthStencilFormat;//纹理格式 后缓冲区
	UINT RTVDescriptorSize;//深度模板格式
};