#pragma once//��ֹ�ظ�����


struct FEngineRenderConfig
{
	FEngineRenderConfig();

	//��Ļ����
	int ScrrenWidth;//��Ļ�ĳ���
	int ScrrenHight;//��Ļ�ĸ߶�
	int RefreshRate;//��Ļ֡��
	int SwapChainCount;//������������


	static FEngineRenderConfig* GetRenderConfig();//��ȡ
	static void Destroy();
private:
	static FEngineRenderConfig* RenderConfig;
};
