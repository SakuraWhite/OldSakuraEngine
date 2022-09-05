#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"
#include "../RenderingPipelineType.h"

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

	//����PSO����
	void BuildParam();

	//������״̬����(pipeline state object)  PSO
	void Build(int InPSOType);

	//���Ƶ�ʱ�����  ���ù���״̬����(����)
	void ResetPSO(int InPSOType);

	//���ù���״̬����(����) ��ֵ ֻ�ṩ����͸���㼶ʹ��
	void ResetPSO();
public:
	//�������ģʽ  �ж��Ƿ�Ϊ�߿�ģʽ
	void SetFillMode(bool bWireframe);

	//������ȾĿ��		��Ⱦ�㼶���				��ȾĿ����ϵ��
	void SetRenderTarget(int Index, const D3D12_RENDER_TARGET_BLEND_DESC& InRenderTargetBlend);

	//���ù�դ��״̬								�����դ����Ⱦ������
	void SetRasterizerState(const CD3DX12_RASTERIZER_DESC& InRasterizerDesc);
	//�������ģ�����״̬					�������ģ������
	void SetDepthStencilState(const CD3DX12_DEPTH_STENCIL_DESC& InDepthStencilDesc);
private:
	//���񰴼� ������ͼ����Ⱦ״̬ shader��Ⱦ���߿���Ⱦ
	void CaptureKeyboardKeys();

private:

	//��ϣ����mapЧ�ʸ�	 ����״̬����(Pipeline State Object)  
	unordered_map<int, ComPtr<ID3D12PipelineState>> PSO;//�ֿ�����״̬���� ��int�������߿���Ⱦ���ǲ�����Ⱦ
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;//������ǰ������  ͼ��_����_״̬_����

	//��ͬ�Ĺ���״̬ ��Ⱦ���߿��ѡ��
	EPipelineState PipelineState;
};