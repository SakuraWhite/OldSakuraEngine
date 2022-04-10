#pragma once//防止重复包含
#include "../../../Core/Engine.h"
#include "../../../Interface/DirectXDeviceInterfece.h"

namespace ConstructBuffer
{
	struct  FConstructBuffer:public IDirectXDeviceInterfece_Struct //继承自DirectXDeviceInterfece
	{

	//创建默认缓冲区  常量构造BUFF 跟管线有关系
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(
			ComPtr<ID3D12Resource>& OutTmpBuffer, //我们要输出的上传缓冲区
			const void* InData,	 //当前的数据
			UINT64 InDataSize);  //当前数据的大小

	};

}
