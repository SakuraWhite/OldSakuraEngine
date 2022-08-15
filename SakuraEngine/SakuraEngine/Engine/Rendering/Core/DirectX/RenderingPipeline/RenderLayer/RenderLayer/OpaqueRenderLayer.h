#pragma once
#include "../Core/RenderLayer.h"

//��͸��������Ⱦ��
class FOpaqueRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//��Ⱦ�㷴��

public:
	FOpaqueRenderLayer();

	virtual void Draw(float DeltaTime);//����(����ʱ)  PSO����״̬�������

	// ����Shader
	virtual void BuildShader();

	//��ȡģ����Ⱦ�㼶����					��͸�����Ϊ0
	virtual int GetRenderLayerType() const { return 0; }

	//PSO����״̬����(��Ⱦ�㼶) ����ӿ�
	virtual void BuildPSO();
};