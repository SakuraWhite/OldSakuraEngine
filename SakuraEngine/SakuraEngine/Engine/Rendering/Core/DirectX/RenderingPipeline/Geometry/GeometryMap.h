#pragma once//防止重复包含
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

//前置声明
class CMaterial;

struct  FGeometry :public IDirectXDeviceInterfece_Struct 
{
	//有源的方法获得 描述模型渲染数据
	friend struct FGeometryMap;


	//判断是否已经存在了渲染数据
	bool IsRenderingDataExistence(CMeshComponent* InKey);
	//添加模型
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData, int InKey);
	//复制模型
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData, int InKey);
	//通过哈希寻找模型渲染数据
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex = -1);
	//构建模型(单个)
	void Build();

	//获取 DescribeMeshRenderingData 描述模型渲染数据
	UINT GetDrawObjectNumber() const;

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
		
};


//记录提供渲染相关的内容的接口    
struct  FGeometryMap :public IDirectXDeviceInterfece_Struct //继承自DirectXDeviceInterfece
{	//几何map映射

	
	friend class FRenderLayer;//渲染层级 有缘

	FGeometryMap();
	~FGeometryMap();//析构
	

	virtual void PreDraw(float DeltaTime);//在模型里使用Rendeing.h的预渲染接口 
	virtual void Draw(float DeltaTime);//在模型里使用Rendeing.h的渲染接口  进行时
	virtual void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw



	//更新MeshManage的数学的方法
	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	//更新材质着色器资源视图
	void UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo);

	//添加模型阶段
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData);
	//复制模型
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData);
	//通过哈希寻找模型渲染数据
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex = -1);

	//加载纹理贴图
	void LoadTexture();

	//构建模型
	void Build();

	//构建描述堆 CBV描述堆
	void BuildDescriptorHeap();

	//构建模型常量缓冲区
	void BuildMeshConstantBuffer();

	//构建材质常量缓冲区
	void BuildMaterialShaderResourceView();

	//构建灯光常量缓冲区
	void BuildLightConstantBuffer();

	
	//获得绘制我们当前模型对象的数量  该接口之后会有修改变化
	UINT GetDrawObjectNumber();

	//获得绘制我们当前材质对象的数量  该接口之后会有修改变化
	UINT GetDrawMaterialObjectNumber();

	//获得绘制我们当前灯光对象的数量  该接口之后会有修改变化
	UINT GetDrawLightObjectNumber();

	//获取绘制贴图纹理的数量
	UINT GetDrawTextureResourcesNumber();

	//构建贴图SRV视图  纹理常数缓冲区
	void BuildTextureConstantBuffer();


	//构建视口常量缓冲区视图
	void BuildViewportConstantBufferView();

public:
	//绘制灯光
	void DrawLight(float DeltaTime);
	//渲染视口
	void DrawViewport(float DeltaTime);
	//渲染模型Mesh
	void DrawMesh(float DeltaTime);
	//渲染绘制材质
	void DrawMaterial(float DeltaTime);
	//渲染纹理贴图(临时)
	void DrawTexture(float DeltaTime);
public:
	//获取常量缓冲区视口描述堆的接口
	ID3D12DescriptorHeap* GetHeap()const { return DescriptorHeap.GetHeap(); }
protected:

	map<int, FGeometry> Geometrys;

	FDirectXDescriptorHeap DescriptorHeap;//描述堆

	FConstantBufferViews MeshConstantBufferViews;//模型常量缓冲区
	FConstantBufferViews MaterialConstantBufferViews;//材质常量缓冲区
	FConstantBufferViews ViewportConstantBufferViews;//视口常量缓冲区
	FConstantBufferViews LightConstantBufferViews;//灯光常量缓冲区
	
	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingTextureResources;//纹理贴图常量缓冲区（智能指针）
	std::vector<CMaterial*> Materials;//封装材质动态数组的顺序容器材质

};