#pragma once
#include "../Core/RenderLayer.h"

//不透明测试渲染层
class FOpaqueRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//渲染层反射

public:
	FOpaqueRenderLayer();

	virtual void Draw(float DeltaTime);//绘制(进行时)  PSO管线状态对象绘制

	// 构建Shader
	virtual void BuildShader();

	//获取模型渲染层级类型					不透明序号为0
	virtual int GetRenderLayerType() const { return 0; }

	//PSO管线状态对象(渲染层级) 虚拟接口
	virtual void BuildPSO();
};