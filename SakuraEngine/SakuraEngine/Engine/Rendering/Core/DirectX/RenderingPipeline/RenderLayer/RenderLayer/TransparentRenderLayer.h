#pragma once
#include "../Core/RenderLayer.h"


//͸����Ⱦ��
class FTransparentRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//��Ⱦ�㷴��

public:
	FTransparentRenderLayer();

	virtual void Draw(float DeltaTime);//����(����ʱ)  PSO����״̬�������

	// ����Shader
	virtual void BuildShader();

	//��ȡģ����Ⱦ�㼶����					��͸�����Ϊ1
	virtual int GetRenderLayerType() const { return 1; }

	//PSO����״̬(��Ⱦ�㼶) ����ӿ�
	virtual void BuildPSO();
};