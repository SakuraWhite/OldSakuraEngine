#include "ShaderCommon.hlsl"
#include "SkyFunction.hlsl"

struct MeshVertexIn //Mash�Ķ��� ��������IN
{
    float3 Position : POSITION; //����λ�� XYZ��ͨ�� ���ָ��ΪPOSITIONλ��
    float3 Normal : NORMAL; //����
    float2 TexCoord : TEXCOORD; //UV����
};


struct MeshVertexOut //Mash�Ķ������ out
{
    float4 PositionH : POSITION; //��ǰ��������ͼ�������嶥��λ��
    float4 PositionL : SV_POSITION; //MVP����˳�����λ�� �ӿ��� ����������ͼ�����ӿ��ϵ�λ��
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)//������ɫ��
{
    MeshVertexOut Out = (MeshVertexOut) 0;//ģ�Ͷ������
    
    Out.PositionH = float4(MV.Position, 1.f); //���붥������
    
    //��������λ��          ��ǰ��������ͼ�������嶥��λ�á�����任����
    float4 WorldPosition = mul(Out.PositionH, WorldMatrix);
    
    //�任����μ����ռ�
    Out.PositionL = mul(WorldPosition, ViewProjectionMatrix);
    
    return Out;//���
}

float4 PixelShaderMain(MeshVertexOut MVOut) : SV_TARGET//������ɫ��
{
    //��������ͼ��ɫ ����CubeMap           ������           ����Ķ���λ��  
    float4 Color = SimpleCubeMap.Sample(TextureSampler, MVOut.PositionH);
    
    //��ȡ��������ͼ����λ��
    float4 WorldPosition = mul(MVOut.PositionH, WorldMatrix);
    
    //�����ɫ����������ͼ���
    Color = GetFogValue(Color, WorldPosition);
    return Color;
    //return float4(0.3f, 0.3f, 0.3f, 1.f);//������ɫ
}

