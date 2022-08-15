#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"

struct  FConstantBufferViews :public IDirectXDeviceInterfece_Struct
{	//������������ͼ(constant buffer view)

	//��������������		ָ����ǰ�����Size     ָ�����������		�ж��Ƿ�Ϊ����������Ĭ��Ϊtrue
	void CreateConstant(UINT ObjectSize, UINT ObjectCount, bool bConstBuffer = true);

	void Update(int Index, const void* InData);//��������

	//ͨ����ַƫ�Ƶķ�ʽ��������������  
	void BuildMeshConstantBuffer(
		CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle,//CPU_������_���
		UINT InConstantBufferNum, //ָ������Burrer������
		UINT InHandleOffset = 0); //ƫ�Ƶ�ֵ
	//����ϴ������� 
	ID3D12Resource* GetBuffer() { return Constant->GetBuffer(); }
protected:
	
	shared_ptr<FRenderingResourcesUpdate> Constant; //���󻺳���  (�����Ҳ����)
};

