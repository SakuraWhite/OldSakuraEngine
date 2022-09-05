#pragma once
#include "../Core/RenderLayer.h"

//��͸��������Ⱦ��
class FOpaqueReflectorRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;//��Ⱦ�㷴��

public:
	FOpaqueReflectorRenderLayer();

	virtual void Draw(float DeltaTime);//����(����ʱ)  PSO����״̬�������

	// ����Shader
	virtual void BuildShader();

	//��ȡģ����Ⱦ�㼶����					��͸����������Ϊ4
	virtual int GetRenderLayerType() const { return 4; }

	//PSO����״̬����(��Ⱦ�㼶) ����ӿ�
	virtual void BuildPSO();
};