#include "ShaderCommon.hlsl"
#include "SkyFunction.hlsl"

struct MeshVertexIn //Mash的顶点 顶点输入IN
{
    float3 Position : POSITION; //顶点位置 XYZ三通道 最后指定为POSITION位置
    float3 Normal : NORMAL; //法线
    float2 TexCoord : TEXCOORD; //UV坐标
};


struct MeshVertexOut //Mash的顶点输出 out
{
    float4 PositionH : POSITION; //当前立方体贴图的立方体顶点位置
    float4 PositionL : SV_POSITION; //MVP矩阵乘出来的位置 视口下 把立方体贴图绑定在视口上的位置
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)//顶点着色器
{
    MeshVertexOut Out = (MeshVertexOut) 0;//模型顶点输出
    
    Out.PositionH = float4(MV.Position, 1.f); //输入顶点数据
    
    //世界坐标位置          当前立方体贴图的立方体顶点位置×世界变换矩阵
    float4 WorldPosition = mul(Out.PositionH, WorldMatrix);
    
    //变换到齐次剪辑空间
    Out.PositionL = mul(WorldPosition, ViewProjectionMatrix);
    
    return Out;//输出
}

float4 PixelShaderMain(MeshVertexOut MVOut) : SV_TARGET//像素着色器
{
    //立方体贴图颜色 采样CubeMap           采样器           输出的顶点位置  
    float4 Color = SimpleCubeMap.Sample(TextureSampler, MVOut.PositionH);
    
    //获取立方体贴图世界位置
    float4 WorldPosition = mul(MVOut.PositionH, WorldMatrix);
    
    //雾的颜色与立方体贴图混合
    Color = GetFogValue(Color, WorldPosition);
    return Color;
    //return float4(0.3f, 0.3f, 0.3f, 1.f);//返回颜色
}

