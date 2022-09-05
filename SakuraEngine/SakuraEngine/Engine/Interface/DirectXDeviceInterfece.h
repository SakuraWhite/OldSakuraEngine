#pragma  once 
#include "../core/Engine.h"

//前置声明
 #if defined(_WIN32)
class CWindowsEngine;
 #else
class CEngine;
#endif

class CLightManage;
class CMeshManage;
class CWorld;



//提供渲染内容的接口 面向当前的类
class IDirectXDeviceInterfece
{
public:
	//这里是函数
	//开始设置主视口渲染目标
	void StartSetMainViewportRenderTarget();
	//结束设置主视口渲染目标
	void EndSetMainViewportRenderTarget();
	//清除主视口交换链画布
	void ClearMainSwapChainCanvas();


public:
	ComPtr<ID3D12Fence> GetFence();//获取围栏 围栏接口
	ComPtr<ID3D12Device> GetD3dDevice();//获取D3D驱动 驱动接口
	CLightManage* GetLightManage();//获取灯光数据信息接口
	CMeshManage* GetMeshManage();//获取模型数据信息接口
	CWorld* GetWorld();//获得世界信息接口

	//获取当前队列
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();//拿到在WindowsEngine.h里用于同步CPU和一个或多个GPU的对象
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	ID3D12DescriptorHeap* GetRTVHeap();//获取渲染目标视口堆
	ID3D12DescriptorHeap* GetDSVHeap();//获取深度模板描述堆
	
	UINT GetDescriptorHandleIncrementSizeByDSV();//获取描述符句柄增量大小  深度模板视图  偏移用
	UINT GetDescriptorHandleIncrementSizeByRTV();//渲染目标视图  偏移用
	UINT GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();//获取描述符句柄增量大小 CBV SRV UAV  偏移用

	UINT64 GetCurrentFenceIndex();//获取当前围栏的Index
	HWND GetMianWindowsHandle();//获取当前的Windows的句柄

 #if defined(_WIN32)	 //做运行环境的区分
 	CWindowsEngine* GetEngine();
 #else
 	CEngine* GetEngine();  //如果是win32就走默认的
 #endif


};

//提供渲染内容的接口 面向当前的结构体
struct IDirectXDeviceInterfece_Struct 
{
public:
	ComPtr<ID3D12Fence> GetFence();//获取围栏 围栏接口
	ComPtr<ID3D12Device> GetD3dDevice();//获取D3D驱动 驱动接口
	CLightManage* GetLightManage();//获取灯光数据信息接口
	CMeshManage* GetMeshManage();//获取模型数据信息接口
	CWorld* GetWorld();//获得世界信息接口

	//获取当前队列
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();//拿到在WindowsEngine.h里用于同步CPU和一个或多个GPU的对象
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	ID3D12DescriptorHeap* GetRTVHeap();//获取渲染目标视口堆
	ID3D12DescriptorHeap* GetDSVHeap();//获取深度模板描述堆

	UINT GetDescriptorHandleIncrementSizeByDSV();//获取描述符句柄增量大小  深度模板视图  偏移用
	UINT GetDescriptorHandleIncrementSizeByRTV();//渲染目标视图  偏移用
	UINT GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();//获取描述符句柄增量大小 CBV SRV UAV  偏移用

	UINT64 GetCurrentFenceIndex();//获取当前围栏的Index
	HWND GetMianWindowsHandle();//获取当前的Windows的句柄

#if defined(_WIN32)	 //做运行环境的区分
	CWindowsEngine* GetEngine();
#else
	CEngine* GetEngine();  //如果是win32就走默认的
#endif


private:
	//创建类接口的实例
	IDirectXDeviceInterfece Interfece;

};