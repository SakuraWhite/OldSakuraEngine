#include "Rendering.h"
#include "../../Debug/EngineDebug.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"

//vector<IRenderingInterface*> IRenderingInterface::RenderingInterface;//.h��ľ�̬���� ��Ҫ����

IRenderingInterface::IRenderingInterface()
{

	 
	//RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	/*
	for (vector<IRenderingInterface*>::const_iterator Iter = RenderingInterface.begin();
		Iter != RenderingInterface.end(); 
		++Iter)     //ʹ�ñ����ķ�ʽ���ж�ʲôʱ�����
	{
		if (*Iter == this) 
		{
			RenderingInterface.erase(Iter);//�Ƴ�
			break;
		}
	}*/
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::PreDraw(float DeltaTime)//Ԥ��Ⱦ
{

	//�������ǵ������б�
	ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), NULL));

}

void IRenderingInterface::Draw(float DeltaTime) //��Ⱦ����
{

}

void IRenderingInterface::PostDraw(float DeltaTime)//��Ⱦ����
{
}

ComPtr<ID3D12Resource> IRenderingInterface::ConstructDefaultBuffer(
	ComPtr<ID3D12Resource>& OutTmpBuffer, 
	const void* InData, 
	UINT64 InDataSize)
{

	ComPtr<ID3D12Resource> Buffer; //׼����Դ
	//��ʼ��
	CD3DX12_RESOURCE_DESC BufferResourceDESC = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
	//����Ĭ�϶�
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(  //ʹ������ָ�봴��һ��Resource��Դ
		&BufferProperties,						
		D3D12_HEAP_FLAG_NONE,					
		&BufferResourceDESC,
		D3D12_RESOURCE_STATE_COMMON,
		NULL,
		IID_PPV_ARGS(Buffer.GetAddressOf())));
	//��ʱ�ϴ�������
	CD3DX12_HEAP_PROPERTIES UpdateBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&UpdateBufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&BufferResourceDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL,
		IID_PPV_ARGS(OutTmpBuffer.GetAddressOf())));

	//��������Դ����
	D3D12_SUBRESOURCE_DATA SubResourceData = {};
	SubResourceData.pData = InData;//������ǰ��Դ��ָ��
	SubResourceData.RowPitch = InDataSize; //�����������ֽ�Ϊ��λ��һ���м�࣬��ȣ������С
	SubResourceData.SlicePitch = SubResourceData.RowPitch; //����ǰ���ݵ���ȼ�࣬��ȣ������С

	//�����ԴΪ����Ŀ��
	CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		Buffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_COPY_DEST);
	
	//���ýӿ�������ȥʵ��
	GetGraphicsCommandList()->ResourceBarrier(1, &CopyDestBarrier);
	
	//������ɺ�ȥ��������Դ Ӧ�������������Դ���顣
	UpdateSubresources<1>
			(GetGraphicsCommandList().Get(), //��һ�������ǽӿ�
			Buffer.Get(), //�ڶ���������buff���׵�ַ
			OutTmpBuffer.Get(), //������������OutTmpBuffer
			0,  //ȡֵ��Χ��0 -> D3D12_REQ_SUBRESOURCES   ���������ǵ�һ����Դ������
			0,  //ȡֵ��Χ��0 -> D3D12_REQ_SUBRESOURCES   ��������Դ������Դ������
			1,  //��Դ�ĳߴ�Size
			&SubResourceData);//���߸�������֮ǰ��������Դ����

	//��֮ǰ��ǵ�Ŀ����Դ�Ӹ������õ��ɶ�
	CD3DX12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		Buffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	return Buffer;
}

ComPtr<ID3D12Device> IRenderingInterface::GetD3dDevice()
{

	if (CWindowsEngine* InEngine = GetEngine())//�ж�Engineʵ���Ƿ�������
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->D3dDevice;//��������ݷ��ص�GetD3dDevice��
		}
	}
	return NULL;
}

ComPtr<ID3D12GraphicsCommandList> IRenderingInterface::GetGraphicsCommandList()
{

	if (CWindowsEngine* InEngine = GetEngine())//�ж�Engineʵ���Ƿ������ݼǵ�ת��ΪWindowsEngine
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GraphicsCommandList;//��������ݷ��ص�GraphicsCommandList��
		}
	}
	return NULL;
}

ComPtr<ID3D12CommandAllocator> IRenderingInterface::GetCommandAllocator()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CommandAllocator;
		}
	}
	return NULL;
}


#if defined(_WIN32)	 //�����л���������
CWindowsEngine* IRenderingInterface::GetEngine()
{
	return dynamic_cast<CWindowsEngine*>(Engine);
}
#else
CEngine* IRenderingInterface::GetEngine()
{
	return Engine;
}
#endif


