#pragma once//��ֹ�ظ�����
#include "../../../Core/Engine.h"
#include "../../../Interface/DirectXDeviceInterfece.h"

namespace ConstructBuffer
{
	struct  FConstructBuffer:public IDirectXDeviceInterfece_Struct //�̳���DirectXDeviceInterfece
	{

	//����Ĭ�ϻ�����  ��������BUFF �������й�ϵ
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(
			ComPtr<ID3D12Resource>& OutTmpBuffer, //����Ҫ������ϴ�������
			const void* InData,	 //��ǰ������
			UINT64 InDataSize);  //��ǰ���ݵĴ�С

	};

}
