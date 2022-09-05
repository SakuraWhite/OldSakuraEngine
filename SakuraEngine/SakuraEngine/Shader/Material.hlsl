#ifndef MATERIAL_HLSL
#define MATERIAL_HLSL


#include "ShaderCommon.hlsl"
#include "ShaderFunctionLibrary.hlsl"
//�������

struct FMaterial
{
	//ͨ�ò���
	
	float4 BaseColor; //������ɫ
};

//							���������F0       ���嶥�㷨��			���������Ҳ�����ǹ��߷��� �ݴη�
float3 FresnelSchlickMethod(float3 InF0, float3 InObjectPointNorma, float3 InDirection, int InPowM)
{	
	//f0Ϊ������  1-fo���Գ�Ϊ�������������Ʒ������ķ��䷶Χ��С�����ڸ߹ⷶΧ
	 //�������㷨	������  ������Ч��������	 ���㷨�ߵ����������򣨵�λ�����޶���Χ0~1 ����ֵ�ٷ���  �ٳ���M�η� 
    return			InF0 + (1.f - InF0) * pow((1.f - saturate(dot(InObjectPointNorma, InDirection))), InPowM);
	
}


//��ȡ������ɫ��ͼ              ������ʻ�����                     ����UV����
float4 GetMaterialBaseColor(MaterialConstBuffer MatConstBuffer, float2 InTexCoord)
{
	  
    if (MatConstBuffer.BaseColorIndex != -1) //ͨ�������еĻ�����ɫ�������ж� ���������-1�򷵻ش�����ɫ��ͼ�Ĳ���
    {
        //���ز�����ɫ            ����������ͼ       ������ͼ��Ž��в���      ���������          ����UV�������   
        return SimpleTexture2DMap[MatConstBuffer.BaseColorIndex].Sample(TextureSampler, InTexCoord)
        * MatConstBuffer.BaseColor; // ģ�ͱ������ɫ
        
    }
    //����Ĭ�ϲ�����ɫ(ָ����RGB)
    return MatConstBuffer.BaseColor;
}

//��ȡ������ͼ                                
float3 GetMaterialNormals(
    MaterialConstBuffer MatConstBuffer, //������ʻ�����
    float2 InTexCoord, //����UV����
    float3 InUnitWorldNormal,//�������編��
    float3 InWorldTangent)//������������
{
	  
    if (MatConstBuffer.NormalIndex != -1) //ͨ�����ʻ������еķ��������ж�
    {
        //������ͼ                 ��������       ������ͼ��Ž��в���                �������Բ���            ����UV�������   
        float4 SampleNormal = SimpleTexture2DMap[MatConstBuffer.NormalIndex].Sample(AnisotropicSampler, InTexCoord);
        
        //��[0,1]��ֵ�� ӳ�䵽[-1,1]��  ���߿ռ��еķ���
        //[0,1]->[-1,1] => [0,1] * 2.f = [0,2] => [0-2]-1.f = [-1,1];
        float3 NormalsInTangentSpace = 2.0f * SampleNormal.rgb - 1.f;
        
        //�õ�����TBN����
        float3x3 TBN = GetBuildTBNMatrix(InUnitWorldNormal, InWorldTangent);
        
        //�����߿ռ��µĲ�������תΪ����ķ���
        return mul(NormalsInTangentSpace, TBN);
    }
    //����Ĭ�����編��
    return InUnitWorldNormal;
}

//��ȡ���ʸ߹���ͼ              ������ʻ�����                     ����UV����
float4 GetMaterialSpecular(MaterialConstBuffer MatConstBuffer, float2 InTexCoord)
{
    if (MatConstBuffer.SpecularIndex != -1)//ͨ���������Ƿ���ֵ
    {
        //���ز��ʸ߹�           ����������ͼ       ������ͼ��Ž��в���    ���������      ����UV�������   
        return SimpleTexture2DMap[MatConstBuffer.SpecularIndex].Sample(TextureSampler, InTexCoord);
    }
    //����Ĭ�ϲ��ʸ߹�(ָ����RGB)
    return float4(MatConstBuffer.SpecularColor, 1.f);
}



//��ȡ����              ���뵥λ���編��             ��������λ��
float3 GetReflect(float3 InUnitWorldNormal, float3 WorldPosition)
{   
    //�۲췽��                                  ��Ұλ�ü�ȥ����λ��
    float3 ViewDirection = normalize(ViewportPosition.xyz - WorldPosition);
     //����            �۲췽�򷴷���    ��λ���編��    
    return reflect(-ViewDirection, InUnitWorldNormal);
}

//��ȡ����                  ���뵥λ���編��    ��������λ��              ���뷴��indax
float3 GetRefract(float3 InUnitWorldNormal, float3 WorldPosition, float InRefractiveIndex)
{
    //�۲췽��                          ����ӿڵ�λ��   ��ȥ���������λ��
    float3 ViewDirection = normalize(ViewportPosition.xyz - WorldPosition);
    //����           
    return refract(-ViewDirection, InUnitWorldNormal, InRefractiveIndex);
}

//��ȡ�����������ɫ(�����Ļ���)   ���뵥λ���編��        ���뷴��
float3 GetReflectionSampleColor(float3 InUnitWorldNormal, float3 NewReflect)
{
    
    //��������ɫ     �����廷����ͼ+�������ɫ 
    return SimpleCubeMap.Sample(TextureSampler, NewReflect);

}

//��ȡ����� ���÷���ǿ��              ���ʳ���������
float GetShininess(MaterialConstBuffer MatConstBuffer)
{
    //      1��ȥ�ֲڶ�
    return 1.f - MatConstBuffer.MaterialRoughness;
}

//��ȡ����������                          �������(���ʻ�����)            ��λ�����編��            ���뷴��
float3 FresnelSchlickFactor(MaterialConstBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 InReflect)
{
    //�������������                      ������F0          ��λ�����編��        ���䷽��  �ݴη� 
    return FresnelSchlickMethod(MatConstBuffer.FresnelF0, InUnitWorldNormal, InReflect, 5);
}

//�������ֲڶ�
float3 FresnelSchlickRoughness(float NV, float3 F0, float Roughness)
{
    return F0 + (max(float3(1.0 - Roughness, 1.0 - Roughness, 1.0 - Roughness), F0) - F0) * pow(1.0 - NV, 5.0);
}

//���շ�����ɫ                            �������(���ʻ�����)             ��λ�����編��         ����λ��
float3 GetReflectionColor(MaterialConstBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 WorldPosition)
{
    //��ȡ����                      ���뵥λ�����編��   ��������λ��
    float3 NewReflect = GetReflect(InUnitWorldNormal, WorldPosition);
    //��ȡ�����������ɫ                 �����������ɫ   ���뵥λ�����編��  ���뷴��
    float3 SampleReflectionColor = GetReflectionSampleColor(InUnitWorldNormal, NewReflect);
    //��ȡ�����                   ������ʳ�������������
    float Shininess = GetShininess(MatConstBuffer);
    //��ȡ����������                            �������(������)   ���뵥λ�����編��  ���뷴��
    float3 FresnelFactor = FresnelSchlickFactor(MatConstBuffer, InUnitWorldNormal, NewReflect);
    
    //�������     �����������ɫ���Է������ٳ��Է����
    return SampleReflectionColor * FresnelFactor * Shininess;

}

//��ȡ�������ɫ 
float3 GetRefractColor(MaterialConstBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 WorldPosition)
{
    float3 NewRefract = GetRefract(InUnitWorldNormal, WorldPosition, MatConstBuffer.Refraction);
    float3 SampleReflectionColor = GetReflectionSampleColor(InUnitWorldNormal, NewRefract);
    float Shininess = GetShininess(MatConstBuffer);
    float3 FresnelFactor = FresnelSchlickFactor(MatConstBuffer, InUnitWorldNormal, NewRefract);

    return SampleReflectionColor * FresnelFactor * Shininess;
}

#endif

