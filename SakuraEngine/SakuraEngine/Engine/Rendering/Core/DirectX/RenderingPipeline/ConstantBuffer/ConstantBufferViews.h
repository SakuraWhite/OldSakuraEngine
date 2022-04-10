#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../RenderingResourcesUpdate.h"

struct  FConstantBufferViews :public IDirectXDeviceInterfece_Struct
{	//������������ͼ(constant buffer view)

	//��������������		ָ����ǰ�����Size     ָ�����������
	void CreateConstant(UINT ObjectSize, UINT ObjectCount);

	void Update(int Index, const void* InData);//��������

	//ͨ����ַƫ�Ƶķ�ʽ��������������  
	void BuildConstantBuffer(
		CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle,//CPU_������_���
		UINT InConstantBufferNum, //ָ������Burrer������
		UINT InHandleOffset = 0); //ƫ�Ƶ�ֵ

protected:
	
	shared_ptr<FRenderingResourcesUpdate> Constant; //���󻺳���  (�����Ҳ����)
};

