#include "EngineRenderConfig.h"

FEngineRenderConfig* FEngineRenderConfig::RenderConfig = nullptr;

FEngineRenderConfig::FEngineRenderConfig()
	:ScrrenWidth(1280)//���1280
	,ScrrenHight(720)//�߶�720
	,RefreshRate(60)//֡��60
	,SwapChainCount(2)//����������2
{
}

FEngineRenderConfig* FEngineRenderConfig::GetRenderConfig()
{
	if (!RenderConfig)
	{
		RenderConfig = new FEngineRenderConfig();
	}

	return RenderConfig;
}

void FEngineRenderConfig::Destroy()
{
	if (RenderConfig)
	{
		delete RenderConfig;
		RenderConfig = nullptr;

	}
}
