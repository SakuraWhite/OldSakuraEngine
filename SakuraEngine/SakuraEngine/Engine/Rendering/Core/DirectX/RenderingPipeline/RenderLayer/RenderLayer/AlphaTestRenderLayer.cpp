#include "AlphaTestRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
	//设定Alpha渲染层级
	RenderPriority = 2450;
}

void FAlphaTestRenderLayer::BuildShader()
{

}

void FAlphaTestRenderLayer::BuildPSO()
{
	//构建PSO管线状态对象
	DirectXPipelineState->Build(AlphaTest);//Alpha测试模式

}

void FAlphaTestRenderLayer::Draw(float DeltaTime)
{

	DirectXPipelineState->ResetPSO(AlphaTest);//重置管线状态对象PSO  重置为Alpha测试模式
	Super::Draw(DeltaTime);//渲染反射
}