


struct Light
{
    float3 LightIntensity; //�ƹ�ǿ��
    float StartAttenuation; //�ƹ��˥����㣨����ʹ��float�ڵ� ���ÿ���ռλ��
	
    float3 LightDirection; //�ƹⷽ��
    float EndAttenuation; //�ƹ��˥���Ľ�����
    
    float3 Position;//�ƹ�λ��
    int LightType;//�ƹ�����
    
    float ConicalInnerCorner; //�۹���ڽ�
    float ConicalOuterCorner; //�۹�����
    float xx1;//ռλ��
    float xx2;//ռλ��
};

//�жϵƹⷽ�����ֲ�ͬ�ĵƹ�����   �ƹ�      ��������λ��
float3 GetLightDirection(Light L, float3 InObjectWorldLocation)
{
    if (L.LightType == 0) //���0 ƽ�й�
    {
        //���ع�Դ�Լ��ķ���
        return L.LightDirection;
    }
    else if (L.LightType == 1)//���1 ���Դ
    {
        //ָ������    ���󶥵�ָ���Դ  �ƹ�����-��������λ������
        return L.Position - InObjectWorldLocation;
    }
    else if (L.LightType == 2)//���2 �۹��
    {
        return L.Position - InObjectWorldLocation;
    }

    return L.LightDirection; //���ع�Դ�Լ��ķ���
}

//���Դ˥����ʽ1  ��ǿ ��(�⵽����ľ��� �� ˥����Χ)
float4 AttenuationPointLights1(Light L, float Distance)
{
    //���Դǿ��
    float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f);
    //���Դ��˥����Χ          ��Դ˥���յ�  - ˥����Χ���
    float AttenuationRange = L.EndAttenuation - L.StartAttenuation;
    //���ص���ֵ   ��ǿ�� �� (�⵽��ľ��� /˥����Χ)  ����Ľ��Ϊ ��ǿ����ĳ�������˥����ֵ�ٷֱ�˥��
    return LightStrength * (1.f -Distance / AttenuationRange); //dot(l��n) �� (d / e-s)    dot(l��n)Ϊ������ (d / e-s)Ϊ�ƹ�˥���ٷֱ�
}

//CIQ����˥��               ����ƹ�    D�ܹ�����Դ����  C����˥������ I����˥������ Qƽ��˥������                              
float4 AttenuationPointLights2(Light L, float Distance, float C, float I, float Q)
{
    //���Դǿ��
    float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f);
    //���ԴCIQ���Ӽ��㹫ʽ
    return (1.f / (C + I * Distance + pow(Q, 2.f) * Distance)) * LightStrength; //F = 1 / (C + I��D + Q��D) 

}


//����ƹ�ǿ��API
float4 ComputeLightStrength(
    Light L,                    //����1 ����ƹ� Light
    float3 InObjectPointNormal, //����2 �������㷨��
    float3 InObjectWorldLocation, //����3 �����������λ��
    float3 NormalizeLightDirection) //����4 �����һ���ƹⷽ��
{
    if (L.LightType == 0) //�жϵƹ�ʲô���� 0ƽ�й�
    {
        return float4(1.f, 1.f, 1.f, 1.f);
    }
    else if (L.LightType == 1) //�жϵƹ�ʲô���� ���Ϊ1 ��Ϊ���Դ
    {

        //���Դ�ķ���  ���Դλ������- ��������λ������
        float3 LightVector = L.Position - InObjectWorldLocation;
        //���Դ������ľ��루�ƹ�ķ���ת��Ϊ���볤�ȣ�
        float Distance = length(LightVector);
        
        //�ƹ�����ж� ��Դ������ľ������С���Դ˥���յ� ֻ��С�ڲŻᱻ����
        if (Distance < L.EndAttenuation)
        {
            //return AttenuationPointLights1(L, Distance) * LightStrength; //����˥��          
            return AttenuationPointLights2(
			L, //�ƹ�
			Distance,// D�ܹ�����Դ����	
			0.f, //C����˥������
            0.5f, //I����˥������	0.9f); 
			0.9f); //Qƽ��˥������
        }  
    }
    if (L.LightType == 2) //���2 �۹��
    //{
        ////���Դ�ķ���
        //float3 LightVector = L.Position - InObjectWorldLocation;;
        ////���Դ������ľ���
        //float Distance = length(LightVector);
        //
        //if (Distance < L.EndAttenuation)
        //{
        //    //�۹���㷨(���Դ����)                       �ƹ�ǿ��                         ��һ���ƹⷽ���˵ƹⱾ����(��ת)      �����ֵȻ���ٴ���
        //    float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f) * pow(max(dot(NormalizeLightDirection, L.LightDirection), 0.f), 1.f);
        //
		//	return AttenuationPointLights1(L, Distance) * LightStrength; //����˥��   
        //    //return AttenuationPointLights2(
		//	//L, //�ƹ�
		//	//Distance, // D�ܹ�����Դ����	
		//	//0.f, //C����˥������
        //    //0.5f, //I����˥������	0.9f); 
		//	//0.9f); //Qƽ��˥������
        //}
    //}
    
    
    {
        //���Դ�ķ���
        float3 LightVector = L.Position - InObjectWorldLocation;;
        //���Դ������ľ���
        float Distance = length(LightVector);
        //�ƹ�����ж�
        if (Distance < L.EndAttenuation)
        {
                //��˵�ֵ                  ��һ���ƹⷽ��  ���   �ƹⱾ��ķ���        �����ֵ
            float DotValue = max(dot(NormalizeLightDirection, L.LightDirection), 0.f);
                
                //�ƹ�ǿ��                        ��ǰǿ��       ��  ����ĵƹ�ǿ��
            float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f) * float4(L.LightIntensity, 1.f);
        
             //����۹�ƽǶ� ��
            float Theta1 = acos(DotValue);
            
            if (Theta1 == 0.f) //����ȵ���0
            {
                return LightStrength; //ֱ�ӷ���û��˥���ĵƹ�ǿ��
            }
            else if (Theta1 <= L.ConicalInnerCorner) //�жϦ��Ƿ�С���ڽ� 
            {
                return LightStrength; //����Ǿ۹�Ƶ��ڽǹ� �򷵻�û��˥���ĵƹ�ǿ��
            }
            else if (Theta1 <= L.ConicalOuterCorner)//�����С����� 
            {
                //�۹����Ǵ����ڽǵĲ��ֿ�ʼ��˥��
                //��Ǽ�ȥ�ڽǵ�ֵ                  ���              �ڽ�
                float OuterInnerDistance = L.ConicalOuterCorner - L.ConicalInnerCorner;
                    
                //�ܹ�����Դ���뵱ǰ����       ��Ǽ�ȥ�ڽǵ�ֵ  -  ���� - �ڽǣ�
                float CurrentDistance = OuterInnerDistance - (Theta1 - L.ConicalInnerCorner);
                    
                //�۹����ǿ�ʼ˥�� ����˥���㷨   ��ǰ�������Ǽ�ȥ�ڽǵ�ֵ ����ǿ��
                return (CurrentDistance / OuterInnerDistance) * LightStrength;
                    
                ////CIQ����˥��
				//return AttenuationPointLights2(
				//	L,              //�ƹ�
				//	CurrentDistance,//D�ܹ�����Դ����	
				//0.f, //C����˥������
                //0.4f, //I����˥������	0.9f); 
		        //0.3f); //Qƽ��˥������
            }
        } 
    }
        return float4(0.f, 0.f, 0.f, 1.f); //����������ϵƹ����͸��򷵻�
    }