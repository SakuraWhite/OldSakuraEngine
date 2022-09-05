#pragma once//防止重复包含
#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"

//前置声明
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;


//动态贴图(动态环境立方体贴图 动态阴影贴图)
struct  FDynamicMap :public IDirectXDeviceInterfece
{
public:
	FDynamicMap();

};