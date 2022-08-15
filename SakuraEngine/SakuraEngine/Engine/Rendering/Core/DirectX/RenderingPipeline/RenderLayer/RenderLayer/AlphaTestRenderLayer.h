#pragma once
#include "../Core/RenderLayer.h"

class FAlphaTestRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//渲染层反射

public:
	FAlphaTestRenderLayer();

	virtual void Draw(float DeltaTime);//绘制(进行时)  PSO管线状态对象绘制

	// 构建Shader
	virtual void BuildShader();
	//构建PSO管线状态对象
	virtual void BuildPSO();
	//获取模型渲染层级类型					Alpha测试序号为2
	virtual int GetRenderLayerType() const { return 2; }
};