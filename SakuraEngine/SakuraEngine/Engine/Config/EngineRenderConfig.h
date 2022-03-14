#pragma once//防止重复包含


struct FEngineRenderConfig
{
	FEngineRenderConfig();
	int ScrrenWidth;//屏幕的长度
	int ScrrenHight;//屏幕的高度
	int RefreshRate;//屏幕帧率
	int SwapChainCount;//交换链的数量
	static FEngineRenderConfig* GetRenderConfig();//获取
	static void Destroy();
private:
	static FEngineRenderConfig* RenderConfig;
};
