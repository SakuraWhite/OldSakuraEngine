#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"

class FRenderingResourcesUpdate;

class CMeshManage :public CCoreMinimalObject, public IRenderingInterface
{
public:
	CMeshManage();//初始化Index与Vertex的一些缓冲区数据

	virtual void Init(); //Rendeing.h里的的渲染初始化

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);//用来构建Mesh接口

	virtual void PreDraw(float DeltaTime);//在模型里使用Rendeing.h的预渲染接口 
	virtual void Draw(float DeltaTime);//在模型里使用Rendeing.h的渲染接口  进行时
	virtual void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();//获取顶点Buffer视图
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();//获取IndexBuffer视图
public:
	CMesh* CreateBoxMesh(
		float InHeight,			//BOX高度
		float InWidth,			//BOX宽度
		float InDepth);			//BOX深度

	CMesh* CreateConeMesh(
		float InRadius,						//圆锥体底部半径  因为圆锥体顶部是单个顶点 ，所以只有底部有半径
		float InHeight,						//圆锥体高度
		uint32_t InAxialSubdivision,		//轴细分
		uint32_t InHeightSubdivision);		//高度细分

	CMesh* CreateCylinderMesh(
		float InTopRadius,					//圆柱顶部半径
		float InBottomRadius,				//圆柱底部半径
		float InHeight,						//圆柱体的高度
		uint32_t InAxialSubdivision,		//球体的轴向细分
		uint32_t InHeightSubdivision);		//球体的高度细分

	CMesh* CreatePlaneMesh(
		float InHeight,						//平面高度
		float InWidth,						//平面宽度
		uint32_t InHeightSubdivide,			//平面的高度细分
		uint32_t InWidthSubdivide);			//平面宽度细分

	CMesh* CreateSphereMesh(
		float InRadius,						//球体的是半径
		uint32_t InAxialSubdivision,		//球体的轴向细分
		uint32_t InHeightSubdivision);		//球体的高度细分

	CMesh* CreateMesh(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;//CPU顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;//CPU的Index缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;//GPU顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;//GPU的Index缓冲区

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;	//临时的顶点缓冲区，方便CPU数据传入GPU
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;	//临时的Index缓冲区，方便CPU数据传入GPU
	

	//根签名
	ComPtr<ID3D12RootSignature>  RootSignature;
	//创建栈
	ComPtr<ID3D12DescriptorHeap> CBVHeap;
	//创建常量指针
	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;//代表着它所描述的对象是唯一的 这里创建了一个对象常量
	

	ComPtr<ID3D12PipelineState> PSO;//Pso指针
	//着色器
	FShader VertexShader;//顶点着色
	FShader PixelShader;//像素着色

	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;//描述输入的内容
protected:
	UINT VertexSizeInBytes;//顶点缓冲区字节大小
	UINT VertexStrideInBytes;//每顶点的字节大小

	UINT IndexSizeInBytes;//Index字节大小
	DXGI_FORMAT IndexFormat;//Index格式
	UINT IndexSize;//用于保存一份顶点数量

	//临时的视口相关  世界空间坐标 其次裁剪空间坐标 物体空间坐标
	XMFLOAT4X4 WorldMatrix;//作为世界空间坐标
	XMFLOAT4X4 ViewMatrix;//齐次裁剪空间坐标 也作为屏幕空间坐标
	XMFLOAT4X4 ProjectMatrix;//物体空间坐标
};
