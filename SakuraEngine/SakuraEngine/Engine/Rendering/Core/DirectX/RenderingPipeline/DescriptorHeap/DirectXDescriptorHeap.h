#pragma once//防止重复包含
#include "../../../../../Interface/DirectXDeviceInterfece.h"

struct  FDirectXDescriptorHeap :public IDirectXDeviceInterfece_Struct
{		//描述堆

	void Build(UINT InNumDescriptor);//需要创建多少个描述

	void PreDraw(float DeltaTime);

	//直接获取常量缓冲区视图的接口
	ID3D12DescriptorHeap* GetHeap()const { return CBVHeap.Get(); }

protected:
	ComPtr<ID3D12DescriptorHeap> CBVHeap; //常量缓冲区视图(constant buffer view)
};