#pragma once

//ö��ģ����Ⱦ�㼶����
enum EMeshRenderLayerType 
{
	RENDERLAYER_OPAQUE = 0,//��͸��
	RENDERLAYER_TRANSPARENT,//͸��
	RENDERLAYER_ALPHATEST,//Alpha����
	RENDERLAYER_BACKGROUND,//����ģʽ

	RENDERLAYER_OPAQUE_REFLECTOR = 4,//��͸���ķ����
};