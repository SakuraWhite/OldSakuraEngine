#include "ConstantBufferViews.h"


void FConstantBufferViews::CreateConstant(UINT ObjectSize, UINT ObjectCount)
{
	Constant = make_shared<FRenderingResourcesUpdate>();//制造共享

	Constant->Init(GetD3dDevice().Get(), ObjectSize, ObjectCount);//初始化创造的常量缓冲区
}

void FConstantBufferViews::Update(int Index, const void* InData)
{
	//更新
	Constant->Update(Index, InData);
}

void FConstantBufferViews::BuildConstantBuffer(
	CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle, 
	UINT InConstantBufferNum,
	UINT InHandleOffset)
{
	//对象物体常量缓冲区构建
	//获取偏移 通过D3D驱动拿到当前描述符UAV的偏移  UAV=无序访问视图
	 UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	 D3D12_GPU_VIRTUAL_ADDRESS Addr = Constant->GetBuffer()->GetGPUVirtualAddress();//拿到常量地址 可以通过GetBuffer获取到GPU的地址

	for (int i = 0; i < InConstantBufferNum; i++) //循环到底创建多少个常量缓冲区
	{
		//每一帧都是起始地址
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = InHandle;
		//开始创建常量缓冲区
		//增加描述
		D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
		CBVDesc.BufferLocation = Addr + i * Constant->GetConstantBufferByteSize();//创建描述前需要获得这个Buffer地址  因为我们要创建多个常量缓冲区所以这里使用地址偏移的方法
		CBVDesc.SizeInBytes = Constant->GetConstantBufferByteSize();//获得Buffer的size

		//用偏移的方式
		Handle.Offset(i + InHandleOffset, DescriptorOffset);//我们的常量缓冲区每次要储存，我们的常量缓冲区都会做偏移。


		//通过获得D3D驱动来创建常量缓冲区
		GetD3dDevice()->CreateConstantBufferView(
			&CBVDesc,//输入描述
			Handle);//通过CBV栈获取到CPU里的描述
	}


}
