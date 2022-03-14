#pragma once//防止重复包含
#include "../../Rending/Core/Rendering.h"
#include "MeshType.h"
#include "../../Shader/Core/Shader.h"


struct FObjectTransformation        //描述在模型世界中的位置  是个矩阵 这里暂时弄个函数
{
	FObjectTransformation(); //初始化一下值
	XMFLOAT4X4 World;
	
	//数学方法  数学计算库 
	static XMFLOAT4X4 IdentityMatrix4x4();//单位化4x4矩阵              
};
//需要继承一下继承来自C开头的核心                      这里是接口
class CMesh :public CCoreMinimalObject ,public IRenderingInterface
{
public:
	CMesh();//初始化Index与Vertex的一些缓冲区数据

	virtual void Init(); //Rendeing.h里的的渲染初始化

	virtual void BuildMesh(const FMeshRendingData* InRendingData);//用来构建Mesh接口

	virtual void PreDraw(float DeltaTime); //在模型里使用Rendeing.h的预渲染接口 
	virtual void Draw(float DeltaTime);  //在模型里使用Rendeing.h的渲染接口  进行时
	virtual void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw

	static CMesh*CreateMesh(const FMeshRendingData *InRendingData); //静态结构来描述顶点数据 静态方式来创建Mesh

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();//获取顶点Buffer视图
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(); //获取IndexBuffer视图
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr; //CPU顶点缓冲区
	ComPtr<ID3DBlob> CPUIndexBufferPtr;	 //CPU的Index缓冲区

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;	//GPU顶点缓冲区
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;	//GPU的Index缓冲区

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;	//临时的顶点缓冲区，方便CPU数据传入GPU
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;	//临时的Index缓冲区，方便CPU数据传入GPU

	//根签名
	ComPtr<ID3D12RootSignature> RootSignature;
	//创建栈
	ComPtr<ID3D12DescriptorHeap> CBVHeap;
												
	//创建常量指针
	shared_ptr<FRenderingResourcesUpdate> objectConstants;  //代表着它所描述的对象是唯一的 这里创建了一个对象常量

	ComPtr<ID3D12PipelineState> PSO;//Pso指针

	//着色器
	FShader VertexShader;//顶点着色
	FShader PixelShader;//像素着色
	vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;//描述输入的内容

protected:
	UINT VertexSizeInBytes;//顶点缓冲区字节大小
	UINT VertexStrideInBytes; //每顶点的字节大小

	UINT IndexSizeInBytes;//Index字节大小
	DXGI_FORMAT IndexFormat;//Index格式
	UINT IndexSize; //用于保存一份顶点数量


	//临时的视口相关  世界空间坐标 其次裁剪空间坐标 物体空间坐标
	XMFLOAT4X4 WorldMatrix;//作为世界空间坐标
	XMFLOAT4X4 ViewMatrix; //齐次裁剪空间坐标 也作为屏幕空间坐标
	XMFLOAT4X4 ProjectMatrix;//物体空间坐标

};