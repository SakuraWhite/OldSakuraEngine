#include "RenderingResourcesUpdate.h"

//构造函数
FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{

}
//析构函数
FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer != nullptr)//当UploadBuffer不等于nullptr的时候 去析构释放掉Unmap
	{
		UploadBuffer->Unmap(0, NULL); //解除当前的映射
		UploadBuffer = nullptr;
	}
}

//初始化
void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount)
{
	assert(InDevice);

	ElementSize = GetConstantBufferByteSize(InElemetSize);//初始化数据 元素

	CD3DX12_HEAP_PROPERTIES HeapPropertie = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//标明HeapPropertie是上传堆
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ElementSize * InElemetCount);//元素乘以数量 资源描述
	ANALYSIS_HRESULT(InDevice->CreateCommittedResource(
		&HeapPropertie,//指定上传堆
		D3D12_HEAP_FLAG_NONE, //输入默认值NONE
		&ResourceDesc, //输入资源数量
		D3D12_RESOURCE_STATE_GENERIC_READ,//只读
		nullptr, //不需要清除 所以是Nullptr
		IID_PPV_ARGS(&UploadBuffer)));//最后添加UploadBuffer指针
	//创建成功后做相应的映射
	ANALYSIS_HRESULT(UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&Data)));
}

//更新
void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	memcpy(&Data[Index * ElementSize], InData, ElementSize);  //复制一下资源
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize(UINT InTypeSzie)
{
	/*处理常量的数据，其实我们的数据就是从CPU传入到GPU里这个时候就需要我们创建一个常量缓冲区。
	由于硬件的原因，缓冲区的大小必须是256的倍数*/
	/*if (!(InTypeSzie % 256)) //先判断能不能被256整除,被整除直接可以被返回 这里是自己写的算法
	{
		//如果不能被整除
		float NewFloat = (float) InTypeSzie / 256.f;
		int Num = (NewFloat +=1);//向上取整 然后得出的结果转成int
		InTypeSzie = Num * 256;//最后再乘以256，如果最后结果为256那么会被256整除
	}*/

	//龙书上提供的算法(InTypeSzie + 255) &~255
	return (InTypeSzie + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
	return GetConstantBufferByteSize(ElementSize);
}
