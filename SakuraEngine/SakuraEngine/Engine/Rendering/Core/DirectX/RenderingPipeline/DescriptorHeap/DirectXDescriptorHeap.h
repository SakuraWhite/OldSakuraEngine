#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"

struct  FDirectXDescriptorHeap :public IDirectXDeviceInterfece_Struct
{		//������

	void Build(UINT InNumDescriptor);//��Ҫ�������ٸ�����

	void PreDraw(float DeltaTime);

	//ֱ�ӻ�ȡ������������ͼ�Ľӿ�
	ID3D12DescriptorHeap* GetHeap()const { return CBVHeap.Get(); }

protected:
	ComPtr<ID3D12DescriptorHeap> CBVHeap; //������������ͼ(constant buffer view)
};