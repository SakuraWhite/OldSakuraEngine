#include "DirectXRenderingEngine.h"

#include "../../../../Debug/EngineDebug.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "../../../../Rendering/Core/Rendering.h"
#include "../../../../Mesh/BoxMesh.h"
#include "../../../../Mesh/SphereMesh.h"
#include "../../../../Mesh/CylinderMesh.h"
#include "../../../../Mesh/ConeMesh.h"
#include "../../../../Mesh/PlaneMesh.h"
#include "../../../../Mesh/CustomMesh.h"
#include "../../../../Core/CoreObject/CoreMinimalObject.h"
#include "../../../../Core/World.h"
#include "../../../../Mesh/Core/MeshManage.h"

#if defined(_WIN32)
#include "../../../../Core/WinMainCommandParameters.h"

//class FVector
//{
//	unsigned char r;//255 ->[0,1]
//	unsigned char g;//255
//	unsigned char b;//255
//	unsigned char a;//255
//};

CDirectXRenderingEngine::CDirectXRenderingEngine()
	:CurrentFenceIndex(0)
	, M4XQualityLevels(0)
	, bMSAA4XEnabled(false)
	, BackBufferFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)//���ð�λ��RGBA
	, DepthStencilFormat(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
	, CurrentSwapBuffIndex(0)
{
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}

	bTick = false;

	MeshManage = new CMeshManage();
}

CDirectXRenderingEngine::~CDirectXRenderingEngine()
{
	delete MeshManage;
}

int CDirectXRenderingEngine::PreInit(FWinMainCommandParameters InParameters)
{
	Engine_Log("DirectXRenderingEngine pre initialization complete.");
	return 0;
}

int CDirectXRenderingEngine::Init(FWinMainCommandParameters InParameters)
{
	InitDirect3D();//d3d����

	PostInitDirect3D();

	MeshManage->Init();


	Engine_Log("DirectXRenderingEngine initialization complete.");

	return 0;
}

int CDirectXRenderingEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");

	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));
	{
		//����Mesh
		// BOX ����
		//	CBoxMesh* Box = CBoxMesh::CreateMesh();
		//MeshManage->CreateBoxMesh(4.f,3.f,1.5f);

		//ConeMesh Բ׶
		//	CConeMesh* ConeMesh = CConeMesh::CreateMesh(1.f, 5.f, 20, 20);
		//MeshManage ->CreateConeMesh(1.f, 5.f, 20, 20);
		
		// �Զ���
			string MeshObjPath = "../../Mesh/EDMesh_01.obj";
			MeshManage->CreateMesh(MeshObjPath);
		
		// Cylinder Բ����
		//CCylinderMesh* CylinderMesh = CCylinderMesh::CreateMesh(1.f,1.f,5.f,20,20);
		//MeshManage ->CreateCylinderMesh(1.f, 1.f, 5.f, 20, 20);
		
		// Plane ��Ƭ
		//CPlaneMesh PlaneMesh =CPlaneMesh:: PlaneMesh(4.f, 3.f, 20, 20);
		//MeshManage->CreatePlaneMesh(4.f, 3.f, 20, 20);
		
		//Sphere ����
		//CSphereMesh* SphereMesh = CSphereMesh::CreateMesh(2.f, 20, 20);
		//MeshManage ->CreateSphereMesh(2.f, 20, 20);

	}

	//�ύ¼���ʼ��
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);
	//CPU�ȴ�GPU��ʼ��
	WaitGPUCommandQueueComplete();

	return 0;
}

void CDirectXRenderingEngine::Tick(float DeltaTime)
{
	//����¼����ص��ڴ棬Ϊ��һ֡��׼�� 
	ANALYSIS_HRESULT(CommandAllocator->Reset());

	MeshManage->PreDraw(DeltaTime);

	//ָ���ĸ���Դ ת����״̬
	//D3D12_RESOURCE_STATE_RENDER_TARGET������Ҫ��Ⱦ��Ŀ������д���״̬
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuff(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);

	//��Ҫÿִ֡��
	//�󶨾��ο�
	GraphicsCommandList->RSSetViewports(1, &ViewprotInfo);//1�����Ű󶨵�������֮���ǰ��ӿ���Ϣ
	GraphicsCommandList->RSSetScissorRects(1, &ViewprotRect);//1������������֮���ǰ��ӿھ��δ�С

	//�������
	GraphicsCommandList->ClearRenderTargetView(//���ǿ���ʹ��ClearRenderTargetView���ɻ������Ϊ������ɫ�������ɫ��ɫ��ɫ�ȵ�
		GetCurrentSwapBufferView(),//������Ҫ��ȡ����ͼ
		DirectX::Colors::Cornsilk,		//�ѻ��������Ϊ������ɫ��API
		0, nullptr);//0��nullptr�����ӿ����

	//������ģ�建����
	//����ǰ��һ����������ȾĿ�������ģ�建������������ĺϲ��׶�
	GraphicsCommandList->ClearDepthStencilView(
		GetCurrentDepthStencilView(), //���ٻ�ȡ���λ��
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,//��������ģ�� λ����
		1.f,//������������1�������Ȼ�����
		0, //������������0�����ģ�建����
		0, NULL);//�����������������DX12��APIһ�¼���

	//ָ����Ⱦ������ ����ϲ��׶�
	D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();//ָ����ȾĿ��
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();//ָ�����Ŀ��
	GraphicsCommandList->OMSetRenderTargets(
		1,//ָ���󶨵�����
		&SwapBufferView, //ָ����ȾĿ��
		true,//��������ľ��ָ��һ��������������ָ��
		&DepthStencilView);//ָ�����Ŀ��

	MeshManage->Draw(DeltaTime);//��Ⱦ�Ľṹ���
	MeshManage->PostDraw(DeltaTime);//��Ⱦ�ύ

	//�����µ�״̬׼����Ⱦ
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentSwapBuff(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 
		D3D12_RESOURCE_STATE_PRESENT);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

	//¼�����
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	//�ύ����
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//��������buff������
	ANALYSIS_HRESULT(SwapChain->Present(0, 0));//�������һ��0�Ǵ�ֱͬ�����ڶ�����inflag
	CurrentSwapBuffIndex = !(bool)CurrentSwapBuffIndex;

	//CPU��GPU
	WaitGPUCommandQueueComplete();
}

int CDirectXRenderingEngine::PreExit()
{


	Engine_Log("Engine post exit complete.");
	return 0;
}

int CDirectXRenderingEngine::Exit()
{

	Engine_Log("Engine exit complete.");
	return 0;
}

int CDirectXRenderingEngine::PostExit()
{
	FEngineRenderConfig::Destroy();

	Engine_Log("Engine post exit complete.");
	return 0;
}

ID3D12Resource* CDirectXRenderingEngine::GetCurrentSwapBuff() const
{
	return SwapChainBuffer[CurrentSwapBuffIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),//��ȡCPU��ǰ����
		CurrentSwapBuffIndex, //��ȡ�ڴ���ĵ�ǰ��SwapBuffindex
		RTVDescriptorSize);//��ȡRTV������ ��С�Ƕ���
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();//��ȡDSV��ȵ�CPU����
}

UINT CDirectXRenderingEngine::GetDXGISampleCount() const
{
	return bMSAA4XEnabled ? 4 : 1;;//�жϲ����Ƿ������������������4�����������1
}

UINT CDirectXRenderingEngine::GetDXGISampleQuality() const
{
	return bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;//�жϲ����Ƿ��������������ô�������������Ĭ��ֵ��1���������0
}

//GPU��CPU��һ��ͬ��
void CDirectXRenderingEngine::WaitGPUCommandQueueComplete()
{
	CurrentFenceIndex++;

	//��GPU�����µ�Χ��������㣩 �ȴ�GPU��������ź�
	ANALYSIS_HRESULT(CommandQueue->Signal(		//�µĸ���� GPU��ɺ��֪ͨ
		Fence.Get(),							//֮ǰ�����õ�Χ��������㣩
		CurrentFenceIndex));					//��ǰ��Χ��������㣩

	//ͨ����ǰ��Fence������ж�GPU��������ź�
	if (Fence->GetCompletedValue() < CurrentFenceIndex)	//ͨ��fence��ȡ����ɵ�ֵ���������ֵ�뵱ǰ��ֵ�����ж�
	{
		//�������һ���¼��ں˶��󣬲����ظ��ں˶���ľ����
		//��һ�����������������ǲ���Ҫ���ӽ��̵����̳У��������ﷵ��Null    SECURITY_ATTRIBUTES
		//�ڶ�������NULL������������Ƿ���Null ��ô���Ǵ����Ķ����¼���������һ���¼����ں˶���
		/*��������������CREATE_EVENT_INITIAL_SET 0x00000002(�����ŵ�ǰ����ĳ�ʼ���Ƿ񱻴��� ���Ϊ����δ����)
		��CREATE_EVENT_MANUAL_RESET 0x00000001���  (����������¼�������Ҫ��"ResetEvents"����������) */
		//���ĸ���������ָ�����ǵ��¼��������п��ܵķ���Ȩ��
		HANDLE EventEX = CreateEventEx(
			NULL,
			NULL, 
			0, 
			EVENT_ALL_ACCESS);

		//GPU��ɺ��֪ͨ���ǵ�Handle
		ANALYSIS_HRESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		//�ȴ�GPU�����������߳�,��ֹGPU�����ύ
		WaitForSingleObject(EventEX, INFINITE);//���޵ȴ��ȴ�
		CloseHandle(EventEX);//���յ���ɵ��¼���Ȼ�����̣߳��ص�Handle
	}
}

bool CDirectXRenderingEngine::InitDirect3D()
{
	//DX12 deBug
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12Debug))))
	{
		D3D12Debug->EnableDebugLayer();
	}

	////////////////////////////////////////////////////////////////////////////////////////
		//HRESULT
		//S_OK				0x00000000
		//E_UNEXPECTED		0x8000FFFF �����ʧ��
		//E_NOTIMPL			0x80004001 δʵ��
		//E_OUTOFMEMORY		0x8007000E δ�ܷ���������ڴ�
		//E_INVALIDARG		0x80070057 һ������������Ч 
		//E_NOINTERFACE		0x80004002 ��֧�ִ˽ӿ�
		//E_POINTER			0x80004003 ��Чָ��
		//E_HANDLE			0x80070006 ��Ч���
		//E_ABORT			0x80004004 ������ֹ
		//E_FAIL			0x80004005 ����
		//E_ACCESSDENIED	0x80070005 һ��ķ��ʱ��ܾ�����
	ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory)));

	/*
	D3D_FEATURE_LEVEL_9_1  Ŀ�깦�ܼ���֧��Direct3D 9.1���� shader model 2.
	D3D_FEATURE_LEVEL_9_2  Ŀ�깦�ܼ���֧��Direct3D 9.2���� shader model 2.
	D3D_FEATURE_LEVEL_9_3  Ŀ�깦�ܼ���֧��Direct3D 9.3���� shader model 3.
	D3D_FEATURE_LEVEL_10_0 Ŀ�깦�ܼ���֧��Direct3D 10.0���� shader model 4.
	D3D_FEATURE_LEVEL_10_1 Ŀ�깦�ܼ���֧��Direct3D 10.1���� shader model 4.
	D3D_FEATURE_LEVEL_11_0 Ŀ�깦�ܼ���֧��Direct3D 11.0���� shader model 5.
	*/
	//"NULL"ָ�����ǵ���ʾ���ڣ�������ʾ������
	//"D3D_FEATURE_LEVEL_11_0"Ŀ�깦�ܼ�����Ⱦ ��Ӧ��DX9-DX11�ĸ��ְ汾
	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		//warp�߼���դ��ƽ̨ ����ͨ��������ʾ������ȥģ��3D��ͼ��Ӳ������
		//���Ӳ����Ⱦʧ�ܣ����ǿ��Բ���WARP������Ⱦ��
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		//�������ʧЧ,����ȥ����WARP������
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	/*
	����Fence����Ϊ��CPU��GPUͬ����׼��
	"0"�������Ǵ�����һ����ʼֵFenceΧ��
	"D3D12_FENCE_FLAG_NONE"��һ��ö��
	"IID_PPV_ARGS"����Ϊ�˸���Fence����ֵ

	���С�D3D12_FENCE_FLAG_NONE���Ľ��ͣ�
	D3D12_FENCE_FLAG_NONE				  ���������Ǵ�����һ����������Fence
	D3D12_FENCE_FLAG_SHARED				  �����ŵ�ǰFence�ǹ�����
	D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER �����ŵ�ǰFence�������һ������������

	Fence->SteEventOnCompletion  ͨ��Fenceȥ����EventOnCompletion
	ִ������
	�ύ����
	Queue->Signal	���л�ȥ�����ź�
	wait	�ȴ�CPUȥ��Ⱦ

	*/
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));//����Χ��

	//��ʼ���������
////////////////////////////////////////////////////////////////////////////////////////
	//INT Priority 
	//D3D12_COMMAND_QUEUE_PRIORITY
	//D3D12_COMMAND_QUEUE_PRIORITY_NORMAL
	//D3D12_COMMAND_QUEUE_PRIORITY_HIGH
	//NodeMask ָʾ���������Ӧ���ĸ�GPU�ڵ���ִ��
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;//ֱ��
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));//����


	//	ID3D12CommandAllocator Allocator();//����������
	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,//ֱ������
		IID_PPV_ARGS(CommandAllocator.GetAddressOf())));

	//�����б��б�
	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0, //Ĭ�ϵ���Gpu 
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,//ֱ������
		CommandAllocator.Get(),//���������� ��Commandlist������Allocatorr
		NULL, //����һ���ӿڡ�ID3D12PipelineState��  ����ӿ��ǵ�ǰ�Ĺ���״̬.����Ĭ��һ��NULL����û�У����ʱ�������Լ�����һ���������״̬
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));//��ȡ��ǰ������

	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	
////////////////////////////////////////////////////////////////////
	//���ز��� SSAA ��MSAA
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;//ָ��������ʽ
	QualityLevels.SampleCount = 4;//����֧�ֵĲ��� ������4�β���
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;//Ĭ�ϲ�֧���κ�ѡ��
	QualityLevels.NumQualityLevels = 0;

	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(//��⵱ǰ���������ǲ���֧�ֵ�
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, //ָ�������ǵ�ǰ��������
		&QualityLevels,							 //��������	
		sizeof(QualityLevels)));				//��ǰ������С

	M4XQualityLevels = QualityLevels.NumQualityLevels;

	
////////////////////////////////////////////////////////////////////
	//������������
	SwapChain.Reset();//��� ��֤���Ǵ��������µĽ�����
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;//�����������Ľṹ��
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScrrenWidth;//�趨����
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScrrenHight;//�趨����
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;//����֡��
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//��դ
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;//���ý���������
	
																					   //���õģ�DXGI_USAGE_BACK_BUFFER ��˼�Ǳ������Դ�����ں�̨������
	//DXGI_USAGE_READ_ONLY �������ṩ�˱������Դ������ֻ�ṩ�Ķ� ���ɶ�ģʽ
	//DXGI_USAGE_SHADER_INPUT  ʹ�ñ������Դ��Ϊ���ǵ�ǰ��ɫ��������
	//DXGI_USAGE_SHARED ����������Դ
	//DXGI_USAGE_UNORDERED_ACCESS ʹ�ñ��������Դ��������ķ���
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//BUFF��������ʹ�� "GXGI"�⴮�������˼����ʹ�ñ������Դ��Ϊ�����ȾĿ��
	SwapChainDesc.OutputWindow = MianWindowsHandle;//ָ��Windows���
	SwapChainDesc.Windowed = true;//�Դ�������
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;//�������ݽ�����ʱ����δ��á� ����ָ�����Ƕ���
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//�����л����ڴ�С������ȫ�����ߴ��ڻ�
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;//������ʽ

	//���ز�������
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();//�жϲ����Ƿ������������������4�����������1
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();//�жϲ����Ƿ��������������ô�������������Ĭ��ֵ��1���������0
	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(
		CommandQueue.Get(),
		&SwapChainDesc, SwapChain.GetAddressOf()));


	////////////////////////////////////////////////////////////////////
	//��Դ������
	//D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV	CBV������������ͼ SRV��ɫ����Դ��ͼ UAV���������ͼ
	//D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER		��������ͼ
	//D3D12_DESCRIPTOR_HEAP_TYPE_RTV			��ȾĿ�����ͼ��Դ
	//D3D12_DESCRIPTOR_HEAP_TYPE_DSV			���/ģ�����ͼ��Դ

	//RTV
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&RTVDescriptorHeapDesc,
		IID_PPV_ARGS(RTVHeap.GetAddressOf())));

	//DSV
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.NumDescriptors = 1;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&DSVDescriptorHeapDesc,
		IID_PPV_ARGS(DSVHeap.GetAddressOf())));

	return false;
}

void CDirectXRenderingEngine::PostInitDirect3D()
{
	//CPUͬ��GPU
	WaitGPUCommandQueueComplete();

	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	DepthStencilBuffer.Reset();

	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScrrenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScrrenHight,
		BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	//�õ�����size
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle);
		HeapHandle.Offset(1, RTVDescriptorSize);
	}

	//�����ģ�建��
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScrrenWidth;
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScrrenHight;
	ResourceDesc.Alignment = 0;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	ResourceDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	ResourceDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DepthStencilFormat;

	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(
		&Properties,
		D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON, &ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Format = DepthStencilFormat;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	GraphicsCommandList->ResourceBarrier(1, &Barrier);
	//�ύ¼��
	GraphicsCommandList->Close();

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//��Щ�Ḳ��ԭ��windows����
	//�����ӿڳߴ�
	ViewprotInfo.TopLeftX = 0;//������Ⱦ�ӿڵ�X��
	ViewprotInfo.TopLeftY = 0;//������Ⱦ�ӿڵ�X��
	ViewprotInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScrrenWidth;//������Ⱦ�ӿڵĿ���
	ViewprotInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScrrenHight;//������Ⱦ�ӿڵĸ߶�
	ViewprotInfo.MinDepth = 0.f;//��ǰ��С���
	ViewprotInfo.MaxDepth = 1.f;//��ǰ������

	//���ƾ���
	ViewprotRect.left = 0;//DX���������Ͻ���ԭ�㣬������߻���Ϊ�㼴��
	ViewprotRect.top = 0;//DX���������Ͻ���ԭ�㣬�ϱ߻���Ϊ�㼴��
	ViewprotRect.right = FEngineRenderConfig::GetRenderConfig()->ScrrenWidth;//�ӿڻ��ƿ��ȼ���
	ViewprotRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScrrenHight;//�ӿڻ��Ƹ߶ȼ���
	//CPU�ȴ�GPU
	WaitGPUCommandQueueComplete();
}

#endif