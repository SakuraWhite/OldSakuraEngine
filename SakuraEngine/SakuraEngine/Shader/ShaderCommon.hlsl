#ifndef ShaderCommon
#define ShaderCommon //�ж��Ƿ��ظ�����

#include "Light.hlsl"

SamplerState TextureSampler : register(s0); //���������  ������� �ֱ��ʽϵͣ�
SamplerState AnisotropicSampler : register(s1); //�������Բ�����

Texture2D SimpleTexture2DMap[TEXTURE2D_MAP_NUM] : register(t1); //���ƻ�����ɫ��ͼ ����t4��Ӧ��ǩ������ͼ���4
TextureCube SimpleCubeMap : register(t0); //����Cube��������ͼ

//cbuffer�ǳ���BUFF���������޸�  ����ConstBuffer����BUFF   ��������ģ�Ͷ�����������
cbuffer ObjectConstBuffer : register(b0) //�ӳ����Ĵ����ж����� �Ĵ���һ����15��  b0->b14
{
    float4x4 WorldMatrix; //�ӷ�װ�ĳ���������������XMFLOAT4X4 World����ռ����
    float4x4 ObjectTextureTransform; //��λ��������ʱ任����
    
    uint MaterialIndex; //��������
    uint RR0; //ռλ
    uint RR1; //ռλ
    uint RR2; //ռλ
}

//���������ӿ�ͶӰ����������
cbuffer ViewportConstBuffer : register(b1) //�ӵ�2�������Ĵ����ж����� b0->b14
{
    float4 ViewportPosition; //�����λ��
	
    float4x4 ViewProjectionMatrix; //��ͼͶӰ͸�Ӿ���
}

//�ƹⳣ��������
cbuffer LightConstBuffer : register(b2) //�ӵ�4�������Ĵ����ж����� b0->b14
{
    Light SceneLights[16]; //�����ƹ� ����16���ƹ�

}


//����������
cbuffer FogConstBuffer : register(b3) //�ӵ�4�������Ĵ����ж�����  b0->b14
{
    float3 FogColor; //����ɫ
    float FogStart; //����ʼ��

    float FogRange; //��Χ�뾶
    float FogHeight;//��߶�
    float FogTransparentCoefficient;//���͸��ϵ��
    float xxx2;//ռλ
}

//���ʳ����������ṹ��
struct MaterialConstBuffer
{
	//16���ֽڵĶ���
	//��Ҫ4���������� ���� 
    int MaterialType; //�������� 
    float MaterialRoughness; // ���� �ֲڶ�
    int BaseColorIndex; //������ɫ������ͼ��ţ����ⲿ���룩
    
    int NormalIndex; //��������
    
    int SpecularIndex;//�߹���ͼ����
    //int RoughnessIndex; //�ֲڶ���ͼ����
    int XX1;
    int XX2;
    int XX3;
	
    float4 BaseColor; //������ɫ��ͼ
    
    float3 SpecularColor;//�߹���ͼ
    float Refraction;//�����
    
    float3 FresnelF0;//����������
    float Transparency;//͸��
    
    float4x4 TransformInformation; //���ʱ任��Ϣ
};
//�ṹbuffer����
StructuredBuffer<MaterialConstBuffer> Materials : register(t0, space1);

#endif