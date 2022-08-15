#include "Light.hlsl"

SamplerState TextureSampler : register(s0); //���������  ������� �ֱ��ʽϵͣ�
SamplerState AnisotropicSampler : register(s1); //�������Բ�����

Texture2D SimpleTexture2DMap[TEXTURE2D_MAP_NUM] : register(t3); //���ƻ�����ɫ��ͼ ����t4��Ӧ��ǩ������ͼ���4




//cbuffer�ǳ���BUFF���������޸�  ����ConstBuffer����BUFF   ��������ģ�Ͷ�����������
cbuffer ObjectConstBuffer : register(b0) //�ӳ����Ĵ����ж����� �Ĵ���һ����15��  ��b0��b14
{
    float4x4 WorldMatrix; //�ӷ�װ�ĳ���������������XMFLOAT4X4 World����ռ����
    float4x4 ObjectTextureTransform; //��λ��������ʱ任����
    
    uint MaterialIndex; //��������
    uint RR0; //ռλ
    uint RR1; //ռλ
    uint RR2; //ռλ
}

//���������ӿ�ͶӰ����������
cbuffer ViewportConstBuffer : register(b1) //�ӵ�2�������Ĵ����ж�����
{
    float4 ViewportPosition; //�����λ��
	
    float4x4 ViewProjectionMatrix; //��ͼͶӰ͸�Ӿ���
}

//�ƹⳣ��������
cbuffer LightConstBuffer : register(b2) //�ӵ�4�������Ĵ����ж�����
{
    Light SceneLights[16]; //�����ƹ� ����16���ƹ�

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
    int XX4;
    //float3 RoughnessColor;//�ֲڶ���ɫ
    //int XX5;
    
    float4x4 TransformInformation; //���ʱ任��Ϣ
};
//�ṹbuffer����
StructuredBuffer<MaterialConstBuffer> Materials : register(t4, space1);

