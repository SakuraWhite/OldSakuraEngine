#pragma once//防止重复包含
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

struct  FGeometry :public IDirectXDeviceInterfece_Struct 
{
	//有源的方法获得 描述模型渲染数据
	friend struct FGeometryMap;


	//判断是否已经存在了渲染数据
	bool bRenderingDataExistence(GMesh* InKey);
	//添加模型
	void BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData);

	//构建模型(单个)
	void Build();

	//获取 DescribeMeshRenderingData 描述模型渲染数据
	UINT GetDrawObjectNumber() const { return DescribeMeshRenderingData.size(); }

	//获取顶点Buffer视图 VBV
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	//获取IndexBuffer视图 IBV
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;//CPU顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;//CPU的Index缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;//GPU顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;//GPU的Index缓冲区

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;	//临时的顶点缓冲区，方便CPU数据传入GPU
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;	//临时的Index缓冲区，方便CPU数据传入GPU


	//模型渲染数据
	FMeshRenderingData MeshRenderingData;

	//DescribeMeshRenderingData 描述模型渲染数据
	vector<FRenderingData> DescribeMeshRenderingData;

};


//记录提供渲染相关的内容的接口    
struct  FGeometryMap :public IDirectXDeviceInterfece_Struct //继承自DirectXDeviceInterfece
{	//几何map映射

	FGeometryMap();

	virtual void PreDraw(float DeltaTime);//在模型里使用Rendeing.h的预渲染接口 
	virtual void Draw(float DeltaTime);//在模型里使用Rendeing.h的渲染接口  进行时
	virtual void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw

	//添加模型阶段
	void BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData);

	//MeshManage的数学的方法
	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	//构建模型
	void Build();

	//构建描述堆 CBV描述堆
	void BuildDescriptorHeap();

	//构建常量缓冲区
	void BuildConstantBuffer();
	
	//获得渲染物体数量  该接口之后会有修改变化
	UINT GetDrawObjectNumber();

	//构建视口常量缓冲区视图
	void BuildViewportConstantBufferView();

public:
	//渲染视口
	void DrawViewport(float DeltaTime);
	//渲染模型Mesh
	void DrawMesh(float DeltaTime);
public:
	//获取常量缓冲区视口描述堆的接口
	ID3D12DescriptorHeap* GetHeap()const { return DescriptorHeap.GetHeap(); }
protected:

	map<int, FGeometry> Geometrys;

	FDirectXDescriptorHeap DescriptorHeap;//描述堆

	FConstantBufferViews ObjectConstantBufferViews;//对象常量缓冲区
	FConstantBufferViews ViewportConstantBufferViews;//视口常量缓冲区
	
};