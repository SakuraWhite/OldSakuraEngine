#pragma once
#include "../../EngineMinimal.h"

//创建常量缓冲区结构
class FRenderingResourcesUpdate :public enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
	FRenderingResourcesUpdate();//构造函数
	~FRenderingResourcesUpdate();//析构函数

	//初始化资源					驱动，		元素的大小,		   元素的数量		 判断常量缓冲区磨人true		
	void Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount, bool bConstBuffer = true);

	void Update(int Index, const void* InData);//更新数据

	//快速获取我们当前常量Buffer字节Size   两种方法
	UINT GetConstantBufferByteSize(UINT InTypeSzie);//元素类型大小 
	UINT GetConstantBufferByteSize();//元素类型大小 直接可以获取

	ID3D12Resource* GetBuffer() { return UploadBuffer.Get(); } //获取上传的Budder 创建的上传Buffer需要暴漏出来
private:
	ComPtr<ID3D12Resource> UploadBuffer;//上传Buffer 
	UINT ElementSize;//单个元素大小
	BYTE* Data;//字节  实际数据
};