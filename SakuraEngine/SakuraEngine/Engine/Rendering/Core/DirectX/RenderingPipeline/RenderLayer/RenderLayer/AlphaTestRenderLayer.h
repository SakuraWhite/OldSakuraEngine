#pragma once
#include "../Core/RenderLayer.h"

class FAlphaTestRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//��Ⱦ�㷴��

public:
	FAlphaTestRenderLayer();

	virtual void Draw(float DeltaTime);//����(����ʱ)  PSO����״̬�������

	// ����Shader
	virtual void BuildShader();
	//����PSO����״̬����
	virtual void BuildPSO();
	//��ȡģ����Ⱦ�㼶����					Alpha�������Ϊ2
	virtual int GetRenderLayerType() const { return 2; }
};