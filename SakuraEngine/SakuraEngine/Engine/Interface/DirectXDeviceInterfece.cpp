#include "DirectXDeviceInterfece.h"
#include "../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"



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