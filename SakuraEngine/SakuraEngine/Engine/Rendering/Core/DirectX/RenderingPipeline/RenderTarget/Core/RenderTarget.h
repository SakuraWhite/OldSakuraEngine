#pragma once
#include "../../../../../../Interface/DirectXDeviceInterfece.h"


//渲染目标
class FRenderTarget
	:public IDirectXDeviceInterfece		//渲染内容的接口
	, std::enable_shared_from_this<FRenderTarget> //渲染目标渲染层
{
public:
	FRenderTarget();
};