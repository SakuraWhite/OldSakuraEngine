#ifndef SkyFunction
#define SkyFunction//判断是否重复包含

#include "ShaderCommon.hlsl"

float4 GetFogValue(float4 InColor, float3 InPointPosition) //输入雾的颜色， 输入雾的原点位置
{
    
#if START_UP_FOG //判断是否开启雾效
    
    //雾原点到视口的距离
    float Distance = length(InPointPosition - ViewportPosition.xyz);//使用雾的点的位置减去视野位置 通过length来求出距离
    
    //计算雾的起始点到雾的结束的衰变值
    float AlphaValue = saturate((Distance - FogStart) / FogRange);//使用距离减去雾开始的位置并除以雾的范围
    //使用线性插值来进行雾从近到远 从稀到浓   
    float3 Color = lerp(InColor.xyz, FogColor.xyz, AlphaValue);
    
    //雾高度的衰变值                    使用雾的原点的Y轴 减去视野的Y轴得出的距离    然后除以   输入的雾的参数（参数不能小于1）
    float HeightAlphaValue = saturate((InPointPosition.y - ViewportPosition.y) / max(FogHeight,1.f));
    //使用线性插值来进行雾的高度的渐变    场景原本颜色与雾的颜色使用雾高度衰变进行插值  雾衰变还要减去一个雾透明度
    InColor.xyz = lerp(Color.xyz, InColor.xyz,HeightAlphaValue - FogTransparentCoefficient);
    
#endif
    
    return InColor;
}

#endif