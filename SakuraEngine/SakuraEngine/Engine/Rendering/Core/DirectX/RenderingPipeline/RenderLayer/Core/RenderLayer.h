#pragma once
#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/RenderingData.h"
#include "../../../../../../Shader/Core/ShaderType.h"

struct FDirectXPipelineState;
struct FGeometryMap;
struct FRenderingData;
struct FViewportInfo;
//渲染层
class FRenderLayer
	:public IDirectXDeviceInterfece
	,public std::enable_shared_from_this<FRenderLayer>//智能指针 可以保留一个弱指针
{
	
	friend struct FGeometry;//直接进行访问
	friend struct FGeometryMap;//直接进行访问
public:
	FRenderLayer();

	//基础注册环节 初始化
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState);

	virtual void PreDraw(float DeltaTime);//预先绘制
	virtual void Draw(float DeltaTime);//绘制(进行时)
	virtual void PostDraw(float DeltaTime);//绘制结束

	//渲染对象（数据）
	virtual void DrawObject(float DeltaTime, const FRenderingData& InRenderingData);
	//指定渲染对象
	virtual void FindObjectDraw(float DeltaTime, const CMeshComponent* InKey);

	//PSO管线状态(渲染层级) 
	virtual void BuildPSO();

	//注册渲染层
	void RegisterRenderLayer();

	//构建Shader宏
	virtual void BuildShaderMacro(std::vector<ShaderType::FShaderMacro> &InMacro);

	//更新计算 常量缓冲区
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
public:
	//获取渲染排序优先级
	const UINT GetRenderPriority()const { return RenderPriority; }
	
public:
	// 构建Shader
	virtual void BuildShader() {};

	//获取模型渲染层级类型
	virtual int GetRenderLayerType() const = 0;
protected:
	UINT RenderPriority;//渲染索引  优先级

protected:
	//渲染层级对应的Shader
	FShader VertexShader; //顶点Shader
	FShader PixelShader;	//像素Shader

	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;//元素描述

	std::vector<FRenderingData> RenderDatas;//渲染层

	//接口
	FGeometryMap* GeometryMap;//几何信息
	FDirectXPipelineState* DirectXPipelineState;//DX渲染管线状态
};