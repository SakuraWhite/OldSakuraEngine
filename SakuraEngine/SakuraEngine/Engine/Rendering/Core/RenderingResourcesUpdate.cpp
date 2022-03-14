#include "RenderingResourcesUpdate.h"

//���캯��
FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{

}
//��������
FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer != nullptr)//��UploadBuffer������nullptr��ʱ�� ȥ�����ͷŵ�Unmap
	{
		UploadBuffer->Unmap(0, NULL); //�����ǰ��ӳ��
		UploadBuffer = nullptr;
	}
}

//��ʼ��
void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount)
{
	assert(InDevice);

	ElementSize = GetConstantBufferByteSize(InElemetSize);//��ʼ������ Ԫ��

	CD3DX12_HEAP_PROPERTIES HeapPropertie = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//����HeapPropertie���ϴ���
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ElementSize * InElemetCount);//Ԫ�س������� ��Դ����
	ANALYSIS_HRESULT(InDevice->CreateCommittedResource(
		&HeapPropertie,//ָ���ϴ���
		D3D12_HEAP_FLAG_NONE, //����Ĭ��ֵNONE
		&ResourceDesc, //������Դ����
		D3D12_RESOURCE_STATE_GENERIC_READ,//ֻ��
		nullptr, //����Ҫ��� ������Nullptr
		IID_PPV_ARGS(&UploadBuffer)));//������UploadBufferָ��
	//�����ɹ�������Ӧ��ӳ��
	ANALYSIS_HRESULT(UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&Data)));
}

//����
void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	memcpy(&Data[Index * ElementSize], InData, ElementSize);  //����һ����Դ
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize(UINT InTypeSzie)
{
	/*�����������ݣ���ʵ���ǵ����ݾ��Ǵ�CPU���뵽GPU�����ʱ�����Ҫ���Ǵ���һ��������������
	����Ӳ����ԭ�򣬻������Ĵ�С������256�ı���*/
	/*if (!(InTypeSzie % 256)) //���ж��ܲ��ܱ�256����,������ֱ�ӿ��Ա����� �������Լ�д���㷨
	{
		//������ܱ�����
		float NewFloat = (float) InTypeSzie / 256.f;
		int Num = (NewFloat +=1);//����ȡ�� Ȼ��ó��Ľ��ת��int
		InTypeSzie = Num * 256;//����ٳ���256����������Ϊ256��ô�ᱻ256����
	}*/

	//�������ṩ���㷨(InTypeSzie + 255) &~255
	return (InTypeSzie + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
	return GetConstantBufferByteSize(ElementSize);
}
