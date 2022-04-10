#include "ConstructBuffer.h"
#include "../../../Debug/EngineDebug.h"


namespace ConstructBuffer
{
	//����Ĭ�ϻ�����  ��������BUFF �������й�ϵ
	ComPtr<ID3D12Resource> FConstructBuffer::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTmpBuffer, const void* InData, UINT64 InDataSize)
	{

		ComPtr<ID3D12Resource> Buffer; //׼����Դ
		//��ʼ��
		CD3DX12_RESOURCE_DESC BufferResourceDESC = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
		//����Ĭ�϶�
		CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(  //ʹ������ָ�봴��һ��Resource��Դ
			&BufferProperties,
			D3D12_HEAP_FLAG_NONE,
			&BufferResourceDESC,
			D3D12_RESOURCE_STATE_COMMON,
			NULL,
			IID_PPV_ARGS(Buffer.GetAddressOf())));
		//��ʱ�ϴ�������
		CD3DX12_HEAP_PROPERTIES UpdateBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
			&UpdateBufferProperties,
			D3D12_HEAP_FLAG_NONE,
			&BufferResourceDESC,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			NULL,
			IID_PPV_ARGS(OutTmpBuffer.GetAddressOf())));

		//��������Դ����
		D3D12_SUBRESOURCE_DATA SubResourceData = {};
		SubResourceData.pData = InData;//������ǰ��Դ��ָ��
		SubResourceData.RowPitch = InDataSize; //�����������ֽ�Ϊ��λ��һ���м�࣬��ȣ������С
		SubResourceData.SlicePitch = SubResourceData.RowPitch; //����ǰ���ݵ���ȼ�࣬��ȣ������С

		//�����ԴΪ����Ŀ��
		CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			Buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST);

		//���ýӿ�������ȥʵ��
		GetGraphicsCommandList()->ResourceBarrier(1, &CopyDestBarrier);

		//������ɺ�ȥ��������Դ Ӧ�������������Դ���顣
		UpdateSubresources<1>
			(GetGraphicsCommandList().Get(), //��һ�������ǽӿ�
				Buffer.Get(), //�ڶ���������buff���׵�ַ
				OutTmpBuffer.Get(), //������������OutTmpBuffer
				0,  //ȡֵ��Χ��0 -> D3D12_REQ_SUBRESOURCES   ���������ǵ�һ����Դ������
				0,  //ȡֵ��Χ��0 -> D3D12_REQ_SUBRESOURCES   ��������Դ������Դ������
				1,  //��Դ�ĳߴ�Size
				&SubResourceData);//���߸�������֮ǰ��������Դ����

		//��֮ǰ��ǵ�Ŀ����Դ�Ӹ������õ��ɶ�
		CD3DX12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			Buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ);

		return Buffer;
	}

}
