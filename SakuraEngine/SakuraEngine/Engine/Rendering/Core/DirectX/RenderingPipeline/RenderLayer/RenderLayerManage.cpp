#include "RenderLayerManage.h"
#include "RenderLayer/AlphaTestRenderLayer.h"
#include "RenderLayer/OpaqueRenderLayer.h"
#include "RenderLayer/TransparentRenderLayer.h"

//声明静态函数
std::vector<std::shared_ptr<FRenderLayer>> FRenderLayerManage::RenderLayers;

FRenderLayerManage::FRenderLayerManage()
{
	RenderLayers.clear();//渲染层级管理创建成功就清除

	//注册渲染层级
	CreateRenderLayer<FAlphaTestRenderLayer>();//Alpha测试
	CreateRenderLayer<FOpaqueRenderLayer>();//不透明层级
	CreateRenderLayer<FTransparentRenderLayer>();//透明层级

}

FRenderLayerManage::~FRenderLayerManage()
{
	RenderLayers.clear();//渲染层级管理创建成功就清除
}

void FRenderLayerManage::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState)
{
	for (auto& Tmp : RenderLayers)//遍历渲染层级
	{
		//注册渲染层级
		Tmp->Init(InGeometryMap, InDirectXPipelineState);
	}
}

void FRenderLayerManage::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (auto& Tmp : RenderLayers)//遍历渲染层级
	{
		//注册更新计算
		Tmp->UpdateCalculations(DeltaTime, ViewportInfo);
	}
}

void FRenderLayerManage::BuildShader()
{
	for (auto& Tmp : RenderLayers)//遍历渲染层级
	{
		//构建shder
		Tmp->BuildShader();
	}
}

void FRenderLayerManage::BuildPSO()
{
	for (auto& Tmp : RenderLayers)//遍历渲染层级
	{
		//构建管线状态PSO
		Tmp->BuildPSO();
	}
}

void FRenderLayerManage::Sort()
{
	//定义排序方式  
	auto CompRenderLayer =   //比较渲染层级
		[&](const std::shared_ptr<FRenderLayer>& InA, const std::shared_ptr<FRenderLayer>& InB)  //传入渲染层级
		->bool  //返回布尔
	{
		//判断渲染层排序
		return InA->GetRenderPriority() < InB->GetRenderPriority();
	};
 
	//进行排序		层级开始				层级结束         对比渲染层级
	std::sort(RenderLayers.begin(), RenderLayers.end(), CompRenderLayer);
}

std::shared_ptr<FRenderLayer> FRenderLayerManage::FindByRenderLayer(int InRenderLayer)
{
	//遍历寻找层级
	for (auto& Tmp : RenderLayers)
	{
		if (Tmp->GetRenderLayerType() == InRenderLayer)//判断获取渲染层级类型是否==为输入的渲染层级
		{
			return Tmp;
		}
	}
	//如果没有渲染层级则返回Null
	return NULL;
}

void FRenderLayerManage::PreDraw(float DeltaTime)
{
	//遍历寻找层级
	for (auto& Tmp : RenderLayers)
	{
		Tmp->PreDraw(DeltaTime);
	}
}

void FRenderLayerManage::Draw(float DeltaTime)
{
	//遍历寻找层级
	for (auto& Tmp : RenderLayers)
	{
		Tmp->Draw(DeltaTime);
	}
}

void FRenderLayerManage::PostDraw(float DeltaTime)
{
	//遍历寻找层级
	for (auto& Tmp : RenderLayers)
	{
		Tmp->Draw(DeltaTime);
	}
}