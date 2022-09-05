#include "DirectXDeviceInterfece.h"
#include "../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"



void IDirectXDeviceInterfece::StartSetMainViewportRenderTarget()
{
	//通过Engine 调取渲染Engine再调出开始设置主视口渲染目标
	GetEngine()->GetRenderingEngine()->StartSetMainViewportRenderTarget();
}

void IDirectXDeviceInterfece::EndSetMainViewportRenderTarget()
{
	//通过Engine 调取渲染Engine再调出结束设置主视口渲染目标
	GetEngine()->GetRenderingEngine()->EndSetMainViewportRenderTarget();
}

void IDirectXDeviceInterfece::ClearMainSwapChainCanvas()
{
	//通过Engine 调取渲染Engine再调出清除主视口交换链画布
	GetEngine()->GetRenderingEngine()->ClearMainSwapChainCanvas();
}

//面向类
ComPtr<ID3D12Fence> IDirectXDeviceInterfece::GetFence()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//拿到RenderingEngine
		{
			return InEngine->GetRenderingEngine()->Fence;//如果拿到渲染引擎则直接使用Fence
		}
	}
	return NULL;

}

ComPtr<ID3D12Device> IDirectXDeviceInterfece::GetD3dDevice()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//拿到RenderingEngine
		{
			return InEngine->GetRenderingEngine()->D3dDevice;//如果拿到渲染引擎则直接使用D3dDevice
		}
	}
	return NULL;
}

CLightManage* IDirectXDeviceInterfece::GetLightManage()
{
	//返回灯光信息  通过渲染引擎进行获取
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GetLightManage();
		}
	}

	return NULL;
}



CMeshManage* IDirectXDeviceInterfece::GetMeshManage()
{
	//返回模型信息
	return GetEngine()->GetMeshManage();
}

CWorld* IDirectXDeviceInterfece::GetWorld()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->GetWorld();
	}

	return NULL;
}


ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterfece::GetGraphicsCommandList()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//拿到RenderingEngine
		{
			return InEngine->GetRenderingEngine()->GraphicsCommandList;//如果拿到渲染引擎则直接使用命令列表
		}
	}
	return NULL;
}


ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterfece::GetCommandAllocator()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//拿到RenderingEngine
		{
			return InEngine->GetRenderingEngine()->CommandAllocator;//如果拿到渲染引擎则直接使用CommandAllocator
		}
	}
	return NULL;
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterfece::GetCommandQueue()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//拿到RenderingEngine
		{
			return InEngine->GetRenderingEngine()->CommandQueue;//如果拿到渲染引擎则直接使用GetCommandQueue
		}
	}
	return NULL;
}

ID3D12DescriptorHeap* IDirectXDeviceInterfece::GetRTVHeap()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//拿到RenderingEngine
		{
			return InEngine->GetRenderingEngine()->RTVHeap.Get();//如果拿到渲染引擎则直接使用渲染目标视口堆
		}
	}
	return NULL;
}

ID3D12DescriptorHeap* IDirectXDeviceInterfece::GetDSVHeap()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//拿到RenderingEngine
		{
			return InEngine->GetRenderingEngine()->DSVHeap.Get();//直接使用深度模板描述堆
		}
	}

	return NULL;
}

UINT IDirectXDeviceInterfece::GetDescriptorHandleIncrementSizeByDSV()
{
	//深度模板视图的描述符句柄增量大小 DSV
	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

UINT IDirectXDeviceInterfece::GetDescriptorHandleIncrementSizeByRTV()
{
	//渲染目标视图的描述符句柄增量大小 RTV
	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

UINT IDirectXDeviceInterfece::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV()
{
	//渲染CBV SRV UAV描述符句柄增量大小 
	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

UINT64 IDirectXDeviceInterfece::GetCurrentFenceIndex()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//拿到RenderingEngine
		{
			return InEngine->GetRenderingEngine()->CurrentFenceIndex;//如果拿到渲染引擎则直接使用CurrentFenceIndex
		}
	}
	return NULL;
}

HWND IDirectXDeviceInterfece::GetMianWindowsHandle()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//拿到RenderingEngine
		{
			return InEngine->MianWindowsHandle;//如果拿到渲染引擎则直接使用MianWindowsHandle句柄
		}
	}
	return HWND();
}

#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterfece::GetEngine()
{
	return dynamic_cast<CWindowsEngine*>(Engine);
}
#else
CEngine* IDirectXDeviceInterfece::GetEngine()
{
	return Engine;
}
#endif



//面向结构体

ComPtr<ID3D12Fence> IDirectXDeviceInterfece_Struct::GetFence()
{
	//直接调用即可
	return Interfece.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterfece_Struct::GetD3dDevice()
{
	return Interfece.GetD3dDevice();
}


CLightManage* IDirectXDeviceInterfece_Struct::GetLightManage()
{
	return Interfece.GetLightManage();
}

CMeshManage* IDirectXDeviceInterfece_Struct::GetMeshManage()
{
	return Interfece.GetMeshManage();
}

CWorld* IDirectXDeviceInterfece_Struct::GetWorld()
{
	return Interfece.GetWorld();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterfece_Struct::GetGraphicsCommandList()
{
	return Interfece.GetGraphicsCommandList();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterfece_Struct::GetCommandAllocator()
{
	return Interfece.GetCommandAllocator();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterfece_Struct::GetCommandQueue()
{
	return Interfece.GetCommandQueue();
}

ID3D12DescriptorHeap* IDirectXDeviceInterfece_Struct::GetRTVHeap()
{
	//直接调用渲染目标视口堆
	return Interfece.GetRTVHeap();
}

ID3D12DescriptorHeap* IDirectXDeviceInterfece_Struct::GetDSVHeap()
{
	//直接调用深度模板描述堆
	return Interfece.GetDSVHeap();
}

UINT IDirectXDeviceInterfece_Struct::GetDescriptorHandleIncrementSizeByDSV()
{
	//直接调用 深度模板视图的描述符句柄增量大小 DSV 偏移
	return Interfece.GetDescriptorHandleIncrementSizeByDSV();
}

UINT IDirectXDeviceInterfece_Struct::GetDescriptorHandleIncrementSizeByRTV()
{
	//直接调用 渲染目标视图的描述符句柄增量大小 RTV 偏移
	return Interfece.GetDescriptorHandleIncrementSizeByRTV();
}

UINT IDirectXDeviceInterfece_Struct::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV()
{
	//直接调用  渲染CBV SRV UAV描述符句柄增量大小  偏移
	return Interfece.GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();
}

UINT64 IDirectXDeviceInterfece_Struct::GetCurrentFenceIndex()
{
	return Interfece.GetCurrentFenceIndex();
}

HWND IDirectXDeviceInterfece_Struct::GetMianWindowsHandle()
{
	return Interfece.GetMianWindowsHandle();
}


#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterfece_Struct::GetEngine()
{
	return Interfece.GetEngine();
}
#else
CEngine* IDirectXDeviceInterfece_Struct::GetEngine()
{
	return Interfece.GetEngine();
}
#endif