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
	ComPtr<ID3D12Fence> GetFence();//获取围栏 围栏接口
	ComPtr<ID3D12Device> GetD3dDevice();//获取D3D驱动 驱动接口
	CLightManage* GetLightManage();//获取灯光数据信息接口
	CMeshManage* GetMeshManage();//获取模型数据信息接口
	CWorld* GetWorld();//获得世界信息接口

	//获取当前队列
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();//拿到在WindowsEngine.h里用于同步CPU和一个或多个GPU的对象
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	
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