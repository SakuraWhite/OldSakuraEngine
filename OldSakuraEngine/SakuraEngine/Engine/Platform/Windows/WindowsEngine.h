#pragma once//��ֹ�ظ�����

#if defined(_WIN32)
#include "../../Core/Engine.h"//�����������ļ����ҵ�Core�ļ��а�����Engine.h���ļ�

class CWindowsEngine : public CEngine //CWindowsEngine�̳���"Engine.h"�е�CEngine
{
	friend class IRenderingInterface; //����һ���ӿڿ�����Rendering.h����WindowsEngine.h������к���

public://������ ��ʼ�����
	CWindowsEngine();
	~CWindowsEngine();

	virtual int PreInit(FwinMainCommandParameters InParameters);//Ԥ��ʼ�� ׼����ʼ��֮ǰ�Ĺ���
	
	virtual int Init(FwinMainCommandParameters InParameters); //����ĳ�ʼ��
	virtual int PostInit();//��ʼ��֮�����β

	
	virtual void Tick(float DeltaTime);//Tick���ϵ�ˢ����Ⱦ����

	virtual int PreExit(); //Ԥ�˳�
	virtual int Exit(); //�˳�
	virtual int PostEixt(); //�˳�����ڴ��ͷţ�Ҳ������β

	

public://������ ��Ⱦ��ͼ���
	ID3D12Resource* GetCurrentSwapBuff() const;   //���ٻ�ȡSwapBuff   api
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView()const; //���ٻ�ȡSwapBuffer��ͼλ�õ�api
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView()const;//���ٻ�ȡ���λ�õ�api

public://������ �ӿ���� ����դ��ʹ��
	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }//�����ӿ� �����ʽ �󻺳���
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; } //��ȡ���ģ��
	UINT GetDXGISampleCount() const; //��ȡ�������� 
	UINT GetDXGISampleQuality() const; //��ȡ����������

protected://�ܱ����� ��������һЩGPUΧ��
	void WaitGPUCommandQueueComplete();

private: //˽�е� ��ʼ���������
	bool InitWindows(FwinMainCommandParameters InParameters);  //��ʼ��windows����
	
	bool InitDirect3D();//��ʼ��Direct3D
	void PostInitDirect3D();//��ʼ��Direct3D�ӿ�

protected://�ܱ����� ��Ⱦ���д����������
	UINT64 CurrentFenceIndex;  //��ǰ��Χ����ָ������GPU��CPUͬ��ʹ�õ�

	int CurrentSwapBuffIndex;//��ǰSwap BUFF��Index 

	ComPtr<IDXGIFactory4> DXGIFactory; //����ͨ��Factoryȥ������ö��һЩ������ ȥ����һЩDirectXͼ�λ����ṹ��DXGI������
	ComPtr<ID3D12Device>  D3dDevice;   //��������������������б�������С�Fence����Դ���ܵ�״̬���󡢶ѡ�ջ����ǩ�����������������Դ��ͼ
	ComPtr<ID3D12Fence>	  Fence;	   //����ͬ��CPU��һ������GPU�Ķ��� ��ֹCPU������ർ��GPU���У������෴��

	//����������С�����������������б�ʵ��
	ComPtr<ID3D12CommandQueue> CommandQueue;			  //����
	ComPtr<ID3D12CommandAllocator> CommandAllocator;	  //������ ����
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;//�����б�

	//������ʵ��
	ComPtr<IDXGISwapChain> SwapChain;

	//����������Ͷ�
	ComPtr<ID3D12DescriptorHeap> RTVHeap;
	ComPtr<ID3D12DescriptorHeap> DSVHeap;

	vector<ComPtr<ID3D12Resource>> SwapChainBuffer;
	ComPtr<ID3D12Resource> DepthStencilBuffer;

	//��Ļ�ӿ���ü�
	D3D12_VIEWPORT ViewprotInfo; 
	D3D12_RECT ViewprotRect;//����
protected: //�ܱ�����  ����� ���� ��ȵ����
	HWND MianWindowsHandle;//��Windows���
	UINT M4XQualityLevels;//���ز�������
	bool bMSAA4XEnabled;//4XMSAA�Ƿ���
	DXGI_FORMAT BackBufferFormat;//�����ʽ �󻺳���
	DXGI_FORMAT DepthStencilFormat;//���ģ���ʽ

	UINT RTVDescriptorSize;
};

#endif
