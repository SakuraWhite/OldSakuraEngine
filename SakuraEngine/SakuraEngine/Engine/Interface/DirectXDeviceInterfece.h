#pragma  once 
#include "../core/Engine.h"

//ǰ������
 #if defined(_WIN32)
class CWindowsEngine;
 #else
class CEngine;
#endif

class CLightManage;
class CMeshManage;
class CWorld;



//�ṩ��Ⱦ���ݵĽӿ� ����ǰ����
class IDirectXDeviceInterfece
{
public:
	ComPtr<ID3D12Fence> GetFence();//��ȡΧ�� Χ���ӿ�
	ComPtr<ID3D12Device> GetD3dDevice();//��ȡD3D���� �����ӿ�
	CLightManage* GetLightManage();//��ȡ�ƹ�������Ϣ�ӿ�
	CMeshManage* GetMeshManage();//��ȡģ��������Ϣ�ӿ�
	CWorld* GetWorld();//���������Ϣ�ӿ�

	//��ȡ��ǰ����
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();//�õ���WindowsEngine.h������ͬ��CPU��һ������GPU�Ķ���
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	
	UINT64 GetCurrentFenceIndex();//��ȡ��ǰΧ����Index
	HWND GetMianWindowsHandle();//��ȡ��ǰ��Windows�ľ��

 #if defined(_WIN32)	 //�����л���������
 	CWindowsEngine* GetEngine();
 #else
 	CEngine* GetEngine();  //�����win32����Ĭ�ϵ�
 #endif


};

//�ṩ��Ⱦ���ݵĽӿ� ����ǰ�Ľṹ��
struct IDirectXDeviceInterfece_Struct 
{
public:
	ComPtr<ID3D12Fence> GetFence();//��ȡΧ�� Χ���ӿ�
	ComPtr<ID3D12Device> GetD3dDevice();//��ȡD3D���� �����ӿ�
	CLightManage* GetLightManage();//��ȡ�ƹ�������Ϣ�ӿ�
	CMeshManage* GetMeshManage();//��ȡģ��������Ϣ�ӿ�
	CWorld* GetWorld();//���������Ϣ�ӿ�

	//��ȡ��ǰ����
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();//�õ���WindowsEngine.h������ͬ��CPU��һ������GPU�Ķ���
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();


	UINT64 GetCurrentFenceIndex();//��ȡ��ǰΧ����Index
	HWND GetMianWindowsHandle();//��ȡ��ǰ��Windows�ľ��

#if defined(_WIN32)	 //�����л���������
	CWindowsEngine* GetEngine();
#else
	CEngine* GetEngine();  //�����win32����Ĭ�ϵ�
#endif


private:
	//������ӿڵ�ʵ��
	IDirectXDeviceInterfece Interfece;

};