#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"


//��¼�ṩ��Ⱦ��ص����ݵĽӿ�    
struct  FDirectXPipelineState :public IDirectXDeviceInterfece_Struct //�̳���DirectXDeviceInterfece
{		//��Ⱦ����״̬

public:
	FDirectXPipelineState();

	void PreDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	void Draw(float DeltaTime);//��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw

	void ResetGPSDesc();//���� ͼ��_����_״̬_����

	//�����벼��
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize);

	//�󶨸�ǩ��
	void BindRootSignature(ID3D12RootSignature* InRootSignature);

	//�󶨶�����ɫ����������ɫ��
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

	//������״̬����(pipeline state object)  PSO
	void Build();
private:
	ComPtr<ID3D12PipelineState> PSO;//����״̬����(pipeline state object) 

	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;//������ǰ������  ͼ��_����_״̬_����
};