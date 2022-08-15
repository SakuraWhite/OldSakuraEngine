#include "DirectXDeviceInterfece.h"
#include "../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"



//������
ComPtr<ID3D12Fence> IDirectXDeviceInterfece::GetFence()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//�õ�RenderingEngine
		{
			return InEngine->GetRenderingEngine()->Fence;//����õ���Ⱦ������ֱ��ʹ��Fence
		}
	}
	return NULL;

}

ComPtr<ID3D12Device> IDirectXDeviceInterfece::GetD3dDevice()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//�õ�RenderingEngine
		{
			return InEngine->GetRenderingEngine()->D3dDevice;//����õ���Ⱦ������ֱ��ʹ��D3dDevice
		}
	}
	return NULL;
}

CLightManage* IDirectXDeviceInterfece::GetLightManage()
{
	//���صƹ���Ϣ  ͨ����Ⱦ������л�ȡ
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
	//����ģ����Ϣ
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
		if (InEngine->GetRenderingEngine())//�õ�RenderingEngine
		{
			return InEngine->GetRenderingEngine()->GraphicsCommandList;//����õ���Ⱦ������ֱ��ʹ�������б�
		}
	}
	return NULL;
}


ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterfece::GetCommandAllocator()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//�õ�RenderingEngine
		{
			return InEngine->GetRenderingEngine()->CommandAllocator;//����õ���Ⱦ������ֱ��ʹ��CommandAllocator
		}
	}
	return NULL;
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterfece::GetCommandQueue()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//�õ�RenderingEngine
		{
			return InEngine->GetRenderingEngine()->CommandQueue;//����õ���Ⱦ������ֱ��ʹ��GetCommandQueue
		}
	}
	return NULL;
}

UINT64 IDirectXDeviceInterfece::GetCurrentFenceIndex()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//�õ�RenderingEngine
		{
			return InEngine->GetRenderingEngine()->CurrentFenceIndex;//����õ���Ⱦ������ֱ��ʹ��CurrentFenceIndex
		}
	}
	return NULL;
}

HWND IDirectXDeviceInterfece::GetMianWindowsHandle()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//�õ�RenderingEngine
		{
			return InEngine->MianWindowsHandle;//����õ���Ⱦ������ֱ��ʹ��MianWindowsHandle���
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



//����ṹ��

ComPtr<ID3D12Fence> IDirectXDeviceInterfece_Struct::GetFence()
{
	//ֱ�ӵ��ü���
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