#pragma once//防止重复包含
#include "../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "Geometry/GeometryMap.h"
#include "RootSignature/DirectXRootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "RenderLayer/RenderLayerManage.h"

class CMeshComponent;

//记录提供渲染相关的内容的接口
class  FRenderingPipeline :public IDirectXDeviceInterfece //继承自DirectXDeviceInterfece
{	//渲染管线


public:
	FRenderingPipeline();

	//添加模型阶段
	void BuildMesh(const size_t InMeshHash, CMeshComponent *InMesh,const FMeshRenderingData& MeshData);
	//复制模型
	void DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData);
	//通过哈希寻找模型渲染数据
	bool FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex = -1);

	//更新MeshManage的数学的方法
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	//创建管线
	void BuildPipeline();


	virtual void PreDraw(float DeltaTime);//在模型里使用Rendeing.h的预渲染接口 
	virtual void Draw(float DeltaTime);//在模型里使用Rendeing.h的渲染接口  进行时
	virtual void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw
protected:

	//着色器

	FRenderLayerManage RenderLayer;//渲染层级

	FDirectXPipelineState DirectXPipelineState; //渲染管线状态
	FDirectXRootSignature RootSignature;//根签名
	FGeometryMap GeometryMap;//拥有几何模型
};