#ifndef MATERIAL_HLSL
#define MATERIAL_HLSL


#include "ShaderCommon.hlsl"
#include "ShaderFunctionLibrary.hlsl"
//定义材质

struct FMaterial
{
	//通用材质
	
	float4 BaseColor; //基本颜色
};

//							三个方向的F0       物体顶点法线			摄像机方向（也可以是光线方向） 幂次方
float3 FresnelSchlickMethod(float3 InF0, float3 InObjectPointNorma, float3 InDirection, int InPowM)
{	
	//f0为反射率  1-fo可以称为缩放量用来控制菲涅尔的反射范围大小类似于高光范围
	 //菲涅尔算法	反射率  菲涅尔效果缩放量	 顶点法线点乘摄像机方向（单位化后）限定范围0~1 最后的值再反向  再乘以M次方 
    return			InF0 + (1.f - InF0) * pow((1.f - saturate(dot(InObjectPointNorma, InDirection))), InPowM);
	
}


//获取材质颜色贴图              输入材质缓冲区                     输入UV坐标
float4 GetMaterialBaseColor(MaterialConstBuffer MatConstBuffer, float2 InTexCoord)
{
	  
    if (MatConstBuffer.BaseColorIndex != -1) //通过材质中的基本颜色索引来判断 如果不等于-1则返回带有颜色贴图的材质
    {
        //返回材质颜色            采样纹理贴图       按照贴图序号进行采样      点纹理采样          按照UV坐标采样   
        return SimpleTexture2DMap[MatConstBuffer.BaseColorIndex].Sample(TextureSampler, InTexCoord)
        * MatConstBuffer.BaseColor; // 模型本身的颜色
        
    }
    //返回默认材质颜色(指定的RGB)
    return MatConstBuffer.BaseColor;
}

//获取法线贴图                                
float3 GetMaterialNormals(
    MaterialConstBuffer MatConstBuffer, //输入材质缓冲区
    float2 InTexCoord, //输入UV坐标
    float3 InUnitWorldNormal,//输入世界法线
    float3 InWorldTangent)//输入世界切线
{
	  
    if (MatConstBuffer.NormalIndex != -1) //通过材质缓冲区中的法线索引判断
    {
        //法线贴图                 采样法线       按照贴图序号进行采样                各向异性采样            按照UV坐标采样   
        float4 SampleNormal = SimpleTexture2DMap[MatConstBuffer.NormalIndex].Sample(AnisotropicSampler, InTexCoord);
        
        //将[0,1]的值域 映射到[-1,1]中  切线空间中的法线
        //[0,1]->[-1,1] => [0,1] * 2.f = [0,2] => [0-2]-1.f = [-1,1];
        float3 NormalsInTangentSpace = 2.0f * SampleNormal.rgb - 1.f;
        
        //拿到世界TBN矩阵
        float3x3 TBN = GetBuildTBNMatrix(InUnitWorldNormal, InWorldTangent);
        
        //把切线空间下的采样法线转为世界的法线
        return mul(NormalsInTangentSpace, TBN);
    }
    //返回默认世界法线
    return InUnitWorldNormal;
}

//获取材质高光贴图              输入材质缓冲区                     输入UV坐标
float4 GetMaterialSpecular(MaterialConstBuffer MatConstBuffer, float2 InTexCoord)
{
    if (MatConstBuffer.SpecularIndex != -1)//通过材质中是否有值
    {
        //返回材质高光           采样纹理贴图       按照贴图序号进行采样    点纹理采样      按照UV坐标采样   
        return SimpleTexture2DMap[MatConstBuffer.SpecularIndex].Sample(TextureSampler, InTexCoord);
    }
    //返回默认材质高光(指定的RGB)
    return float4(MatConstBuffer.SpecularColor, 1.f);
}



//获取反射              输入单位世界法线             输入世界位置
float3 GetReflect(float3 InUnitWorldNormal, float3 WorldPosition)
{   
    //观察方向                                  视野位置减去世界位置
    float3 ViewDirection = normalize(ViewportPosition.xyz - WorldPosition);
     //反射            观察方向反方向    单位世界法线    
    return reflect(-ViewDirection, InUnitWorldNormal);
}

//获取折射                  输入单位世界法线    输入世界位置              输入反射indax
float3 GetRefract(float3 InUnitWorldNormal, float3 WorldPosition, float InRefractiveIndex)
{
    //观察方向                          相机视口的位置   减去顶点的世界位置
    float3 ViewDirection = normalize(ViewportPosition.xyz - WorldPosition);
    //折射           
    return refract(-ViewDirection, InUnitWorldNormal, InRefractiveIndex);
}

//获取反射采样的颜色(采样的环境)   输入单位世界法线        输入反射
float3 GetReflectionSampleColor(float3 InUnitWorldNormal, float3 NewReflect)
{
    
    //最后输出颜色     立方体环境贴图+反射的颜色 
    return SimpleCubeMap.Sample(TextureSampler, NewReflect);

}

//获取反射度 设置反射强度              材质常量缓冲区
float GetShininess(MaterialConstBuffer MatConstBuffer)
{
    //      1减去粗糙度
    return 1.f - MatConstBuffer.MaterialRoughness;
}

//获取菲涅尔因子                          传入材质(材质缓冲区)            单位化世界法线            输入反射
float3 FresnelSchlickFactor(MaterialConstBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 InReflect)
{
    //输出菲涅尔因子                      菲涅尔F0          单位化世界法线        反射方向  幂次方 
    return FresnelSchlickMethod(MatConstBuffer.FresnelF0, InUnitWorldNormal, InReflect, 5);
}

//菲涅尔粗糙度
float3 FresnelSchlickRoughness(float NV, float3 F0, float Roughness)
{
    return F0 + (max(float3(1.0 - Roughness, 1.0 - Roughness, 1.0 - Roughness), F0) - F0) * pow(1.0 - NV, 5.0);
}

//最终反射颜色                            传入材质(材质缓冲区)             单位化世界法线         世界位置
float3 GetReflectionColor(MaterialConstBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 WorldPosition)
{
    //获取反射                      输入单位化世界法线   输入世界位置
    float3 NewReflect = GetReflect(InUnitWorldNormal, WorldPosition);
    //获取采样后反射的颜色                 反射采样的颜色   输入单位化世界法线  输入反射
    float3 SampleReflectionColor = GetReflectionSampleColor(InUnitWorldNormal, NewReflect);
    //获取反射度                   输入材质常量缓冲区数据
    float Shininess = GetShininess(MatConstBuffer);
    //获取菲涅尔因子                            输入材质(菲涅尔)   输入单位化世界法线  输入反射
    float3 FresnelFactor = FresnelSchlickFactor(MatConstBuffer, InUnitWorldNormal, NewReflect);
    
    //最终输出     采样后反射的颜色乘以菲涅尔再乘以反射度
    return SampleReflectionColor * FresnelFactor * Shininess;

}

//获取折射的颜色 
float3 GetRefractColor(MaterialConstBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 WorldPosition)
{
    float3 NewRefract = GetRefract(InUnitWorldNormal, WorldPosition, MatConstBuffer.Refraction);
    float3 SampleReflectionColor = GetReflectionSampleColor(InUnitWorldNormal, NewRefract);
    float Shininess = GetShininess(MatConstBuffer);
    float3 FresnelFactor = FresnelSchlickFactor(MatConstBuffer, InUnitWorldNormal, NewRefract);

    return SampleReflectionColor * FresnelFactor * Shininess;
}

#endif

