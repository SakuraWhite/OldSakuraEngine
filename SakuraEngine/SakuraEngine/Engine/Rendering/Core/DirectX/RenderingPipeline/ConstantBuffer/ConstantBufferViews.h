#pragma once//防止重复包含
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"

struct  FConstantBufferViews :public IDirectXDeviceInterfece_Struct
{	//常量缓冲区视图(constant buffer view)

	//创建常量缓冲区		指定当前对象的Size     指定对象的数量		判断是否为常量缓冲区默认为true
	void CreateConstant(UINT ObjectSize, UINT ObjectCount, bool bConstBuffer = true);

	void Update(int Index, const void* InData);//更新数据

	//通过地址偏移的方式构建常量缓冲区  
	void BuildMeshConstantBuffer(
		CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle,//CPU_描述符_句柄
		UINT InConstantBufferNum, //指定构建Burrer的数量
		UINT InHandleOffset = 0); //偏移的值
	//获得上传缓冲区 
	ID3D12Resource* GetBuffer() { return Constant->GetBuffer(); }
protected:
	
	shared_ptr<FRenderingResourcesUpdate> Constant; //对象缓冲区  (摄像机也能用)
};

