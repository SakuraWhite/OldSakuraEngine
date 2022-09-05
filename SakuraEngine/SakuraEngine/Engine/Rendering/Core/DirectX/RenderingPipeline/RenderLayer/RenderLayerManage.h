#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "Core/RenderLayer.h"


//层级管理
class FRenderLayerManage
{
	//可以访问内容
	friend class FRenderLayer;//可以访问渲染层级
	friend struct FGeometry;//可以访问几何
	friend struct FGeometryMap;//可以访问几何map
public:
	FRenderLayerManage();
	~FRenderLayerManage();//析构

	//初始化当前层级
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState);
	//更新计算 常量缓冲区
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	virtual void PreDraw(float DeltaTime);//预先绘制
	virtual void Draw(float DeltaTime);//绘制(进行时)
	virtual void PostDraw(float DeltaTime);//绘制结束

	//需要渲染哪个层 单独制定渲染那个层级
	virtual void Draw(int InLayer, float DeltaTime);
	//找到要绘制的对象
	virtual void FindObjectDraw(float DeltaTime, int InLayer, const CMeshComponent* InKey);

	// 构建Shader
	virtual void BuildShader();

	//PSO管线状态对象(渲染层级)
	virtual void BuildPSO();

	//渲染排序接口
	virtual void Sort();//根据渲染优先级进行排序

	//静态 按照渲染层级查找
	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int InRenderLayer);
protected:
	//静态渲染层级
	static std::vector<std::shared_ptr<FRenderLayer>> RenderLayers;
};

//注册渲染层级 模板
template<class T>
//创建渲染层级
std::shared_ptr<T> CreateRenderLayer()
{
	std::shared_ptr<T> RenderLayer = std::make_shared<T>();//创建实例
	RenderLayer->RegisterRenderLayer();//注册渲染层及

	return RenderLayer;
}