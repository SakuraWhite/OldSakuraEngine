#pragma once
#include "../../../../../../Interface/DirectXDeviceInterfece.h"


//��ȾĿ��
class FRenderTarget
	:public IDirectXDeviceInterfece		//��Ⱦ���ݵĽӿ�
	, std::enable_shared_from_this<FRenderTarget> //��ȾĿ����Ⱦ��
{
public:
	FRenderTarget();
};