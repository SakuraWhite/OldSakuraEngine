#pragma once//��ֹ�ظ�����
#include "../../Core/Engine.h"
#include "../../Core/CoreObject/GuidInterface.h"
#if defined(_WIN32)   //��������������ͷ�ļ�
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif


//��¼�ṩ��Ⱦ��ص����ݵĽӿ�
class  IRenderingInterface   //:public IGuidInterface //�̳�GUID
{
	friend class CDirectXRenderingEngine;//��Engine����ֱ�ӷ���
public:
	IRenderingInterface();  //����
	virtual ~IRenderingInterface();	//��������

	virtual void Init(); //��Ⱦ��ʼ��

	virtual void PreDraw(float DeltaTime); //Ԥ��Ⱦ
	virtual void Draw(float DeltaTime); //��Ⱦ��� ����ʱ
	virtual void PostDraw(float DeltaTime);//������Ⱦ poatDraw

protected:
	//����Ĭ�ϻ�����
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(
		ComPtr<ID3D12Resource>& OutTmpBuffer, //����Ҫ������ϴ�������
		const void*InData,	 //��ǰ������
		UINT64 InDataSize);  //��ǰ���ݵĴ�С
protected:
	ComPtr<ID3D12Device>  GetD3dDevice(); //�õ���WindowsEngine.h���D3D����
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();//�õ���WindowsEngine.h������ͬ��CPU��һ������GPU�Ķ���
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();//�ӿ�
#if defined(_WIN32)	 //�����л���������
	CWindowsEngine* GetEngine();
#else
	CEngine* GetEngine();  //�����win32����Ĭ�ϵ�
#endif
		
	/*
private:
	static vector<IRenderingInterface*> RenderingInterface;//��̬����
	*/
};
