#include "DirectXDescriptorHeap.h"

void FDirectXDescriptorHeap::Build(UINT InNumDescriptor)
{
	//构建CBV栈描述
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = InNumDescriptor;//多少个描述
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; //需要指定一个Type 这里指定栈的类型包含CBV的描述状态
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//设置当前标志
	HeapDesc.NodeMask = 0;
	GetD3dDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&CBVHeap));

}

void FDirectXDescriptorHeap::PreDraw(float DeltaTime)
{
	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
}
