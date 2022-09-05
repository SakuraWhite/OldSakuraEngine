#pragma once//防止重复包含
#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"
#include "../Core/DynamicMap.h"

//前置声明
class GClientViewport;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;


//动态环境贴图
struct  FDynamicCubeMap :public FDynamicMap
{
	struct FTmpViewportCapture  //Tmp视口捕获结构体
	{
		FTmpViewportCapture() {}  //构造
		FTmpViewportCapture(const fvector_3d& InCenterPoint); //构造

		fvector_3d TargetPoint[6]; //视口捕获(目标) 构建6个
		fvector_3d UP[6];

		//构建视口捕获
		void BuildViewportCapture(const fvector_3d& InCenterPoint);
	};

public:
	FDynamicCubeMap();
	//更新计算     计算cubemap相关         
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	//注册初始化						输入几何map							 输入管线对象状态									输入渲染层级
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer);
	//预绘制
	virtual void PreDraw(float DeltaTime);
	//绘制 实体渲染
	virtual void Draw(float DeltaTime);


public:
	//设置cubeMap视口位置
	void SetCubeMapViewportPosition(const fvector_3d& InPosition);
	//是否退出动态反射网格
	bool IsExitDynamicReflectionMesh();

public:
	//构建视口(动态立方体贴图的6个摄像机的视口)		输入中心点 XYZ的一个值
	virtual void BuildViewport(const fvector_3d& InCenterPoint);
	//构建深度模板
	virtual void BuildDepthStencil();

	//构建深度模板描述符
	virtual void BuildDepthStencilDescriptor();
	//构建渲染目标描述符
	virtual void BuildRenderTargetDescriptor();

protected:
	virtual void BuildRenderTargetRTV();//构建渲染目标渲染状态视口
	virtual void BuildRenderTargetSRV();//构建渲染目标shader着色器资源视图
protected:
	std::unique_ptr<FCubeMapRenderTarget> RenderTarget;//渲染目标(立方体贴图)
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDes;//深度模板描述(立方体贴图 CPU)

	std::vector<GClientViewport*> CubeMapViewport;//6个视口(用作立方体贴图)

	FGeometryMap* GeometryMap;//几何Map
	FDirectXPipelineState* DirectXPipelineState;//管线对象 用于绑定

	FRenderLayerManage* RenderLayer;//渲染层级(类)

	ComPtr<ID3D12Resource> DepthStencilBuffer;//深度模板缓冲区
	UINT Width; //视图的宽度
	UINT Height;//视图的高度
};