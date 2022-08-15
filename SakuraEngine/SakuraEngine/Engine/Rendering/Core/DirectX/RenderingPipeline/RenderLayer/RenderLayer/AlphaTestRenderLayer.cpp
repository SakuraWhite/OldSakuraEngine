#include "AlphaTestRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
	//�趨Alpha��Ⱦ�㼶
	RenderPriority = 2450;
}

void FAlphaTestRenderLayer::BuildShader()
{

}

void FAlphaTestRenderLayer::BuildPSO()
{
	//����PSO����״̬����
	DirectXPipelineState->Build(AlphaTest);//Alpha����ģʽ

}

void FAlphaTestRenderLayer::Draw(float DeltaTime)
{

	DirectXPipelineState->ResetPSO(AlphaTest);//���ù���״̬����PSO  ����ΪAlpha����ģʽ
	Super::Draw(DeltaTime);//��Ⱦ����
}