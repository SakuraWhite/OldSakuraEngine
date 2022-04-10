#pragma once//防止重复包含
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"


class GMesh;

//记录提供渲染相关的内容的接口    
struct  FRenderingData :public IDirectXDeviceInterfece_Struct //继承自DirectXDeviceInterfece
{		//我们要渲染的数据

public:
	FRenderingData();

	UINT IndexSize;//用于保存一份Index数量
	UINT VertexSize;//用于保存一份顶点数量

	UINT IndexOffsetPosition;//Index偏移的位置
	UINT VertexOffsetPosition;//顶点偏移的位置

	UINT VertexTypeSize;//顶点类型字符大小
	UINT IndexTypeSize;//Index类型字符大小

public:
	UINT GetVertexSizeInBytes() const { return VertexSize * VertexTypeSize; } //直接计算顶点字节大小
	UINT GetIndexSizeInBytes() const { return IndexSize * IndexTypeSize; }//直接计算Index字节大小
public:

	DXGI_FORMAT IndexFormat;//Index格式
	
	//临时的视口相关  世界空间坐标 其次裁剪空间坐标 物体空间坐标
	XMFLOAT4X4 WorldMatrix;//作为世界空间坐标

	GMesh* Mesh;

	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;
};