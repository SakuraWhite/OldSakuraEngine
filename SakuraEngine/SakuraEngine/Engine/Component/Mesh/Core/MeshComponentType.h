#pragma once

//枚举模型渲染层级类型
enum EMeshRenderLayerType 
{
	RENDERLAYER_OPAQUE = 0,//不透明
	RENDERLAYER_TRANSPARENT,//透明
	RENDERLAYER_ALPHATEST,//Alpha测试
};