#include "EngineRenderConfig.h"

FEngineRenderConfig* FEngineRenderConfig::RenderConfig = nullptr;

FEngineRenderConfig::FEngineRenderConfig()
	:ScrrenWidth(1280)//宽度1280
	,ScrrenHight(720)//高度720
	,RefreshRate(60)//帧数60
	,SwapChainCount(2)//交换链数量2
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
