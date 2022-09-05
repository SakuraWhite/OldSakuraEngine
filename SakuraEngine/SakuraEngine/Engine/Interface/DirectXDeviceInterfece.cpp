#include "DirectXDeviceInterfece.h"
#include "../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"



void IDirectXDeviceInterfece::StartSetMainViewportRenderTarget()
{
	//ͨ��Engine ��ȡ��ȾEngine�ٵ�����ʼ�������ӿ���ȾĿ��
	GetEngine()->GetRenderingEngine()->StartSetMainViewportRenderTarget();
}

void IDirectXDeviceInterfece::EndSetMainViewportRenderTarget()
{
	//ͨ��Engine ��ȡ��ȾEngine�ٵ��������������ӿ���ȾĿ��
	GetEngine()->GetRenderingEngine()->EndSetMainViewportRenderTarget();
}

void IDirectXDeviceInterfece::ClearMainSwapChainCanvas()
{
	//ͨ��Engine ��ȡ��ȾEngine�ٵ���������ӿڽ���������
	GetEngine()->GetRenderingEngine()->ClearMainSwapChainCanvas();
}

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

ID3D12DescriptorHeap* IDirectXDeviceInterfece::GetRTVHeap()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//�õ�RenderingEngine
		{
			return InEngine->GetRenderingEngine()->RTVHeap.Get();//����õ���Ⱦ������ֱ��ʹ����ȾĿ���ӿڶ�
		}
	}
	return NULL;
}

ID3D12DescriptorHeap* IDirectXDeviceInterfece::GetDSVHeap()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())//�õ�RenderingEngine
		{
			return InEngine->GetRenderingEngine()->DSVHeap.Get();//ֱ��ʹ�����ģ��������
		}
	}

	return NULL;
}

UINT IDirectXDeviceInterfece::GetDescriptorHandleIncrementSizeByDSV()
{
	//���ģ����ͼ�����������������С DSV
	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

UINT IDirectXDeviceInterfece::GetDescriptorHandleIncrementSizeByRTV()
{
	//��ȾĿ����ͼ�����������������С RTV
	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

UINT IDirectXDeviceInterfece::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV()
{
	//��ȾCBV SRV UAV���������������С 
	return GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
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

ID3D12DescriptorHeap* IDirectXDeviceInterfece_Struct::GetRTVHeap()
{
	//ֱ�ӵ�����ȾĿ���ӿڶ�
	return Interfece.GetRTVHeap();
}

ID3D12DescriptorHeap* IDirectXDeviceInterfece_Struct::GetDSVHeap()
{
	//ֱ�ӵ������ģ��������
	return Interfece.GetDSVHeap();
}

UINT IDirectXDeviceInterfece_Struct::GetDescriptorHandleIncrementSizeByDSV()
{
	//ֱ�ӵ��� ���ģ����ͼ�����������������С DSV ƫ��
	return Interfece.GetDescriptorHandleIncrementSizeByDSV();
}

UINT IDirectXDeviceInterfece_Struct::GetDescriptorHandleIncrementSizeByRTV()
{
	//ֱ�ӵ��� ��ȾĿ����ͼ�����������������С RTV ƫ��
	return Interfece.GetDescriptorHandleIncrementSizeByRTV();
}

UINT IDirectXDeviceInterfece_Struct::GetDescriptorHandleIncrementSizeByCBV_SRV_UAV()
{
	//ֱ�ӵ���  ��ȾCBV SRV UAV���������������С  ƫ��
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