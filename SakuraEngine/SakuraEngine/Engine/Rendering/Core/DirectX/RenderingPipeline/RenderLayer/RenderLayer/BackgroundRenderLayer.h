#pragma once
#include "../Core/RenderLayer.h"

class FBackgroundRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//��Ⱦ�㷴��

public:
	FBackgroundRenderLayer();

	virtual void Draw(float DeltaTime);//����(����ʱ)  PSO����״̬�������

	//����Shader
	virtual void BuildShader();
	//����PSO����״̬����
	virtual void BuildPSO();
	//��ȡģ����Ⱦ�㼶����					����ģʽ���Ϊ3
	virtual int GetRenderLayerType() const { return 3; }
};