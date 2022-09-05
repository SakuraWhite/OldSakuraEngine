#pragma once
#include "../Core/RenderLayer.h"

//不透明反射渲染层
class FOpaqueReflectorRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//渲染层反射

public:
	FOpaqueReflectorRenderLayer();

	virtual void Draw(float DeltaTime);//绘制(进行时)  PSO管线状态对象绘制

	// 构建Shader
	virtual void BuildShader();

	//获取模型渲染层级类型					不透明反射层序号为4
	virtual int GetRenderLayerType() const { return 4; }

	//PSO管线状态对象(渲染层级) 虚拟接口
	virtual void BuildPSO();
};