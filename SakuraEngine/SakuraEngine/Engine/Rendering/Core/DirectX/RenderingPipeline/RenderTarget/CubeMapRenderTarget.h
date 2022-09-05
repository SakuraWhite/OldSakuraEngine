#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "Core/RenderTarget.h"

//�ṩ��Ⱦ���ݵĽӿ�
class FCubeMapRenderTarget 
	:public FRenderTarget //�̳���ȾĿ��
{
	friend class FDynamicCubeMap; //��̬��������ͼ ��Դ
public:
	FCubeMapRenderTarget();

	//��ʼ����ȾĿ��Ŀ� ���Լ��洢��ȾĿ���ӿڵ���ͼ��ʽ
	void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat);

	//������ȾĿ�� �ӿ������Ϣ
	void ResetViewport(UINT InWidth, UINT InHeight);
	//�����ӿڲü������Ϣ
	void ResetScissorRect(UINT InWidth, UINT InHeight);

public:
	//������ȾĿ�� ��ȾĿ��Ŀ������Ӧ
	void ResetRenderTarget(UINT InWidth, UINT InHeight);

private:
	//������ȾĿ����ͼ��Ϣ
	void BuildRenderTargetMap();
	//����SRV������  ��ɫ����Դͼ
	void BuildSRVDescriptors();
	//����RTV������  ��ȾĿ����ͼ
	void BuildRTVDescriptors();

public:
	//��ȡ��ȾĿ��
	FORCEINLINE ID3D12Resource* GetRenderTarget() const { return RenderTargetMap.Get(); }
	//��ȡ��ȾĿ���ӿ������Ϣ
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	//��ȡ��ȾĿ��ü������Ϣ
	FORCEINLINE D3D12_RECT GetScissorRect()const { return ScissorRect; }


private:
	UINT Width; //������ȾĿ��Ŀ�
	UINT Height;//������ȾĿ��ĸ�
	DXGI_FORMAT Format;//��ȾĿ��֮��洢��RGB��ʽ

	D3D12_VIEWPORT Viewport;//��ȾĿ�� �ӿ������Ϣ
	D3D12_RECT ScissorRect;//��ȾĿ�� �ӿڲü������Ϣ

	//�����ݵĵط�
	ComPtr<ID3D12Resource> RenderTargetMap;//��ȾĿ����ͼ  �洢��ȾĿ���ӿڵ���ͼ

	//SRV ��ɫ����Դ��ͼ
	CD3DX12_CPU_DESCRIPTOR_HANDLE CPUShaderResourceView;//CPU��ɫ����Դ��ͼ
	CD3DX12_GPU_DESCRIPTOR_HANDLE GPUShaderResourceView;//GPU��ɫ����Դ��ͼ

	//RTV ��ȾĿ����ͼ
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> CPURenderTargetView;//CPU��ȾĿ����ͼ
	//CD3DX12_GPU_DESCRIPTOR_HANDLE GPURenderTargetView[6];//GPU��ȾĿ����ͼ
};