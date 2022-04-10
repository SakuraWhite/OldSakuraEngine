#include "ConstantBufferViews.h"


void FConstantBufferViews::CreateConstant(UINT ObjectSize, UINT ObjectCount)
{
	Constant = make_shared<FRenderingResourcesUpdate>();//���칲��

	Constant->Init(GetD3dDevice().Get(), ObjectSize, ObjectCount);//��ʼ������ĳ���������
}

void FConstantBufferViews::Update(int Index, const void* InData)
{
	//����
	Constant->Update(Index, InData);
}

void FConstantBufferViews::BuildConstantBuffer(
	CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle, 
	UINT InConstantBufferNum,
	UINT InHandleOffset)
{
	//�������峣������������
	//��ȡƫ�� ͨ��D3D�����õ���ǰ������UAV��ƫ��  UAV=���������ͼ
	 UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	 D3D12_GPU_VIRTUAL_ADDRESS Addr = Constant->GetBuffer()->GetGPUVirtualAddress();//�õ�������ַ ����ͨ��GetBuffer��ȡ��GPU�ĵ�ַ

	for (int i = 0; i < InConstantBufferNum; i++) //ѭ�����״������ٸ�����������
	{
		//ÿһ֡������ʼ��ַ
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = InHandle;
		//��ʼ��������������
		//��������
		D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
		CBVDesc.BufferLocation = Addr + i * Constant->GetConstantBufferByteSize();//��������ǰ��Ҫ������Buffer��ַ  ��Ϊ����Ҫ�������������������������ʹ�õ�ַƫ�Ƶķ���
		CBVDesc.SizeInBytes = Constant->GetConstantBufferByteSize();//���Buffer��size

		//��ƫ�Ƶķ�ʽ
		Handle.Offset(i + InHandleOffset, DescriptorOffset);//���ǵĳ���������ÿ��Ҫ���棬���ǵĳ���������������ƫ�ơ�


		//ͨ�����D3D��������������������
		GetD3dDevice()->CreateConstantBufferView(
			&CBVDesc,//��������
			Handle);//ͨ��CBVջ��ȡ��CPU�������
	}


}
