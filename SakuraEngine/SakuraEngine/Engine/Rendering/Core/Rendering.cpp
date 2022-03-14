#include "Rendering.h"
#include "../../Debug/EngineDebug.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"

//vector<IRenderingInterface*> IRenderingInterface::RenderingInterface;//.h里的静态函数 需要声明

IRenderingInterface::IRenderingInterface()
{

	 
	//RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	/*
	for (vector<IRenderingInterface*>::const_iterator Iter = RenderingInterface.begin();
		Iter != RenderingInterface.end(); 
		++Iter)     //使用遍历的方式来判断什么时候结束
	{
		if (*Iter == this) 
		{
			RenderingInterface.erase(Iter);//移除
			break;
		}
	}*/
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::PreDraw(float DeltaTime)//预渲染
{

	//重置我们的命令列表
	ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), NULL));

}

void IRenderingInterface::Draw(float DeltaTime) //渲染进行
{

}

void IRenderingInterface::PostDraw(float DeltaTime)//渲染交付
{
}

ComPtr<ID3D12Resource> IRenderingInterface::ConstructDefaultBuffer(
	ComPtr<ID3D12Resource>& OutTmpBuffer, 
	const void* InData, 
	UINT64 InDataSize)
{

	ComPtr<ID3D12Resource> Buffer; //准备资源
	//初始化
	CD3DX12_RESOURCE_DESC BufferResourceDESC = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
	//创建默认堆
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(  //使用驱动指针创建一个Resource资源
		&BufferProperties,						
		D3D12_HEAP_FLAG_NONE,					
		&BufferResourceDESC,
		D3D12_RESOURCE_STATE_COMMON,
		NULL,
		IID_PPV_ARGS(Buffer.GetAddressOf())));
	//临时上传缓冲区
	CD3DX12_HEAP_PROPERTIES UpdateBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&UpdateBufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&BufferResourceDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL,
		IID_PPV_ARGS(OutTmpBuffer.GetAddressOf())));

	//描述子资源数据
	D3D12_SUBRESOURCE_DATA SubResourceData = {};
	SubResourceData.pData = InData;//描述当前资源的指针
	SubResourceData.RowPitch = InDataSize; //代表数据以字节为单位的一个行间距，宽度，物理大小
	SubResourceData.SlicePitch = SubResourceData.RowPitch; //代表当前数据的深度间距，宽度，物理大小

	//标记资源为复制目标
	CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		Buffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_COPY_DEST);
	
	//调用接口真正的去实现
	GetGraphicsCommandList()->ResourceBarrier(1, &CopyDestBarrier);
	
	//设置完成后去更新子资源 应该填充所有子资源数组。
	UpdateSubresources<1>
			(GetGraphicsCommandList().Get(), //第一个输入是接口
			Buffer.Get(), //第二个输入是buff的首地址
			OutTmpBuffer.Get(), //第三个输入是OutTmpBuffer
			0,  //取值范围是0 -> D3D12_REQ_SUBRESOURCES   代表着我们第一个资源的索引
			0,  //取值范围是0 -> D3D12_REQ_SUBRESOURCES   代表着资源中子资源的数量
			1,  //资源的尺寸Size
			&SubResourceData);//第七个输入是之前描述的资源数据

	//将之前标记的目标资源从复制设置到可读
	CD3DX12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		Buffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ);

	return Buffer;
}

ComPtr<ID3D12Device> IRenderingInterface::GetD3dDevice()
{

	if (CWindowsEngine* InEngine = GetEngine())//判断Engine实例是否有内容
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->D3dDevice;//如果有内容返回到GetD3dDevice里
		}
	}
	return NULL;
}

ComPtr<ID3D12GraphicsCommandList> IRenderingInterface::GetGraphicsCommandList()
{

	if (CWindowsEngine* InEngine = GetEngine())//判断Engine实例是否有内容记得转换为WindowsEngine
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GraphicsCommandList;//如果有内容返回到GraphicsCommandList里
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


#if defined(_WIN32)	 //做运行环境的区分
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


