#pragma once
#include "../Core/RenderLayer.h"

class FBackgroundRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//渲染层反射

public:
	FBackgroundRenderLayer();

	virtual void Draw(float DeltaTime);//绘制(进行时)  PSO管线状态对象绘制

	//构建Shader
	virtual void BuildShader();
	//构建PSO管线状态对象
	virtual void BuildPSO();
	//获取模型渲染层级类型					背景模式序号为3
	virtual int GetRenderLayerType() const { return 3; }
};