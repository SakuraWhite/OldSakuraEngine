#include "ConstructBuffer.h"
#include "../../../Debug/EngineDebug.h"


namespace ConstructBuffer
{
	//创建默认缓冲区  常量构造BUFF 跟管线有关系
	ComPtr<ID3D12Resource> FConstructBuffer::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTmpBuffer, const void* InData, UINT64 InDataSize)
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

}
