#ifndef ShaderCommon
#define ShaderCommon //判断是否重复包含

#include "Light.hlsl"

SamplerState TextureSampler : register(s0); //纹理采样器  （点采样 分辨率较低）
SamplerState AnisotropicSampler : register(s1); //各向异性采样器

Texture2D SimpleTexture2DMap[TEXTURE2D_MAP_NUM] : register(t1); //绘制基本颜色贴图 这里t4对应根签名中贴图序号4
TextureCube SimpleCubeMap : register(t0); //采样Cube立方体贴图

//cbuffer是常量BUFF声明后不能修改  声明ConstBuffer常量BUFF   用来描述模型对象常量缓冲区
cbuffer ObjectConstBuffer : register(b0) //从常量寄存器中读出来 寄存器一共有15个  b0->b14
{
    float4x4 WorldMatrix; //从封装的常量缓冲区读出来XMFLOAT4X4 World世界空间矩阵
    float4x4 ObjectTextureTransform; //单位化对象材质变换矩阵
    
    uint MaterialIndex; //材质索引
    uint RR0; //占位
    uint RR1; //占位
    uint RR2; //占位
}

//用来描述视口投影常量缓冲区
cbuffer ViewportConstBuffer : register(b1) //从第2个常量寄存器中读出来 b0->b14
{
    float4 ViewportPosition; //摄像机位置
	
    float4x4 ViewProjectionMatrix; //视图投影透视矩阵
}

//灯光常量缓冲区
cbuffer LightConstBuffer : register(b2) //从第4个常量寄存器中读出来 b0->b14
{
    Light SceneLights[16]; //场景灯光 定义16个灯光

}


//雾常量缓冲区
cbuffer FogConstBuffer : register(b3) //从第4个常量寄存器中读出来  b0->b14
{
    float3 FogColor; //雾颜色
    float FogStart; //雾起始点

    float FogRange; //雾范围半径
    float FogHeight;//雾高度
    float FogTransparentCoefficient;//雾的透明系数
    float xxx2;//占位
}

//材质常量缓冲区结构体
struct MaterialConstBuffer
{
	//16个字节的对齐
	//需要4个材质类型 对齐 
    int MaterialType; //材质类型 
    float MaterialRoughness; // 材质 粗糙度
    int BaseColorIndex; //基本颜色纹理贴图序号（从外部传入）
    
    int NormalIndex; //法线索引
    
    int SpecularIndex;//高光贴图索引
    //int RoughnessIndex; //粗糙度贴图索引
    int XX1;
    int XX2;
    int XX3;
	
    float4 BaseColor; //基本颜色贴图
    
    float3 SpecularColor;//高光贴图
    float Refraction;//折射度
    
    float3 FresnelF0;//菲涅尔因子
    float Transparency;//透明
    
    float4x4 TransformInformation; //材质变换信息
};
//结构buffer缓冲
StructuredBuffer<MaterialConstBuffer> Materials : register(t0, space1);

#endif