#pragma once//��ֹ�ظ�����
#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"

//ǰ������
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;


//��̬��ͼ(��̬������������ͼ ��̬��Ӱ��ͼ)
struct  FDynamicMap :public IDirectXDeviceInterfece
{
public:
	FDynamicMap();

};