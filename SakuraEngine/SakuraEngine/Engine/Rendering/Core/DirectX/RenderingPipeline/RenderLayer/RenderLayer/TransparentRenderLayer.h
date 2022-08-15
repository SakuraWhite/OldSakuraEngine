#pragma once
#include "../Core/RenderLayer.h"


//透明渲染层
class FTransparentRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//渲染层反射

public:
	FTransparentRenderLayer();

	virtual void Draw(float DeltaTime);//绘制(进行时)  PSO管线状态对象绘制

	// 构建Shader
	virtual void BuildShader();

	//获取模型渲染层级类型					不透明序号为1
	virtual int GetRenderLayerType() const { return 1; }

	//PSO管线状态(渲染层级) 虚拟接口
	virtual void BuildPSO();
};