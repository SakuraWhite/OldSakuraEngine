#ifndef SkyFunction
#define SkyFunction//�ж��Ƿ��ظ�����

#include "ShaderCommon.hlsl"

float4 GetFogValue(float4 InColor, float3 InPointPosition) //���������ɫ�� �������ԭ��λ��
{
    
#if START_UP_FOG //�ж��Ƿ�����Ч
    
    //��ԭ�㵽�ӿڵľ���
    float Distance = length(InPointPosition - ViewportPosition.xyz);//ʹ����ĵ��λ�ü�ȥ��Ұλ�� ͨ��length���������
    
    //���������ʼ�㵽��Ľ�����˥��ֵ
    float AlphaValue = saturate((Distance - FogStart) / FogRange);//ʹ�þ����ȥ��ʼ��λ�ò�������ķ�Χ
    //ʹ�����Բ�ֵ��������ӽ���Զ ��ϡ��Ũ   
    float3 Color = lerp(InColor.xyz, FogColor.xyz, AlphaValue);
    
    //��߶ȵ�˥��ֵ                    ʹ�����ԭ���Y�� ��ȥ��Ұ��Y��ó��ľ���    Ȼ�����   �������Ĳ�������������С��1��
    float HeightAlphaValue = saturate((InPointPosition.y - ViewportPosition.y) / max(FogHeight,1.f));
    //ʹ�����Բ�ֵ��������ĸ߶ȵĽ���    ����ԭ����ɫ�������ɫʹ����߶�˥����в�ֵ  ��˥�仹Ҫ��ȥһ����͸����
    InColor.xyz = lerp(Color.xyz, InColor.xyz,HeightAlphaValue - FogTransparentCoefficient);
    
#endif
    
    return InColor;
}

#endif