#pragma once //��ֹ�ظ�����
#include "../../Core/RenderingEngine.h"

class CMeshManage;
class CDirectXRenderingEngine :public CRenderingEngine
{
	friend class IRenderingInterface;//����һ���ӿ�

public://������ ��ʼ�����
	CDirectXRenderingEngine();
	~CDirectXRenderingEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);//Ԥ��ʼ�� ׼����ʼ��֮ǰ�Ĺ���

	virtual int Init(FWinMainCommandParameters InParameters);//����ĳ�ʼ��
	virtual int PostInit();//��ʼ��֮�����β

	virtual void Tick(float DeltaTime);//Tick���ϵ�ˢ����Ⱦ����

	virtual int PreExit();//Ԥ�˳�
	virtual int Exit();//�˳�
	virtual int PostExit();//�˳�����ڴ��ͷţ�Ҳ������β
public:
	ID3D12Resource* GetCurrentSwapBuff() const; //���ٻ�ȡSwapBuff   api
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;//���ٻ�ȡSwapBuffer��ͼλ�õ�api
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;//���ٻ�ȡ���λ�õ�api
public://������ �ӿ���� ����դ��ʹ��
	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }//�����ӿ� �����ʽ �󻺳���
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; } //��ȡ���ģ��
	UINT GetDXGISampleCount()const;//��ȡ�������� 
	UINT GetDXGISampleQuality()const;//��ȡ����������
protected://�ܱ����� ��������һЩGPUΧ��
	void WaitGPUCommandQueueComplete();

	bool InitDirect3D();//��ʼ��Direct3D
	void PostInitDirect3D();//��ʼ��Direct3D�ӿ�

protected:
	CMeshManage* MeshManage;
protected:
	UINT64 CurrentFenceIndex;//��ǰ��Χ����ָ������GPU��CPUͬ��ʹ�õ�
	int CurrentSwapBuffIndex;//��ǰSwap BUFF��Index 

	ComPtr<IDXGIFactory4> DXGIFactory;//���� DirectX ͼ�λ����ṹ (DXGI) ����
	ComPtr<ID3D12Device> D3dDevice;//��������������������б�������С�Fence����Դ���ܵ�״̬���󡢶ѡ���ǩ�����������������Դ��ͼ
	ComPtr<ID3D12Fence> Fence;//һ������ͬ�� CPU ��һ������ GPU �Ķ���
	//����������С�����������������б�ʵ��
	ComPtr<ID3D12CommandQueue> CommandQueue;//����
	ComPtr<ID3D12CommandAllocator> CommandAllocator; //�洢
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;//�����б�
	//������ʵ��
	ComPtr<IDXGISwapChain> SwapChain;

	//����������Ͷ�
	ComPtr<ID3D12DescriptorHeap> RTVHeap;
	ComPtr<ID3D12DescriptorHeap> DSVHeap;

	vector<ComPtr<ID3D12Resource>> SwapChainBuffer;
	ComPtr<ID3D12Resource> DepthStencilBuffer;

	//����Ļ���ӿ��й�
	D3D12_VIEWPORT ViewprotInfo;
	D3D12_RECT ViewprotRect;
protected://�ܱ�����  ����� ���� ��ȵ����
	UINT M4XQualityLevels;//��Windows���
	bool bMSAA4XEnabled;//���ز�������
	DXGI_FORMAT BackBufferFormat;//4XMSAA�Ƿ���
	DXGI_FORMAT DepthStencilFormat;//�����ʽ �󻺳���
	UINT RTVDescriptorSize;//���ģ���ʽ
};