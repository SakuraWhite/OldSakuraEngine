


struct Light
{
    float3 LightIntensity; //灯光强度
    float StartAttenuation; //灯光的衰减起点（这里使用float节点 正好可以占位）
	
    float3 LightDirection; //灯光方向
    float EndAttenuation; //灯光的衰减的结束点
    
    float3 Position;//灯光位置
    int LightType;//灯光类型
    
    float ConicalInnerCorner; //聚光灯内角
    float ConicalOuterCorner; //聚光灯外角
    float xx1;//占位符
    float xx2;//占位符
};

//判断灯光方向区分不同的灯光类型   灯光      对象世界位置
float3 GetLightDirection(Light L, float3 InObjectWorldLocation)
{
    if (L.LightType == 0) //序号0 平行光
    {
        //返回光源自己的方向
        return L.LightDirection;
    }
    else if (L.LightType == 1)//序号1 点光源
    {
        //指定方向    对象顶点指向灯源  灯光向量-对象世界位置向量
        return L.Position - InObjectWorldLocation;
    }
    else if (L.LightType == 2)//序号2 聚光灯
    {
        return L.Position - InObjectWorldLocation;
    }

    return L.LightDirection; //返回光源自己的方向
}

//点光源衰减公式1  光强 ×(光到物体的距离 ÷ 衰减范围)
float4 AttenuationPointLights1(Light L, float Distance)
{
    //点光源强度
    float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f);
    //点光源的衰减范围          光源衰减终点  - 衰减范围起点
    float AttenuationRange = L.EndAttenuation - L.StartAttenuation;
    //返回的数值   光强度 × (光到物的距离 /衰减范围)  计算的结果为 光强度在某个距离的衰减比值百分比衰减
    return LightStrength * (1.f -Distance / AttenuationRange); //dot(l·n) × (d / e-s)    dot(l·n)为兰伯特 (d / e-s)为灯光衰减百分比
}

//CIQ因子衰减               输入灯光    D受光点与光源距离  C常熟衰减因子 I线性衰减因子 Q平方衰减因子                              
float4 AttenuationPointLights2(Light L, float Distance, float C, float I, float Q)
{
    //点光源强度
    float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f);
    //点光源CIQ因子计算公式
    return (1.f / (C + I * Distance + pow(Q, 2.f) * Distance)) * LightStrength; //F = 1 / (C + I×D + Q×D) 

}


//计算灯光强度API
float4 ComputeLightStrength(
    Light L,                    //参数1 输入灯光 Light
    float3 InObjectPointNormal, //参数2 输入对象点法线
    float3 InObjectWorldLocation, //参数3 输入对象世界位置
    float3 NormalizeLightDirection) //参数4 输入归一化灯光方向
{
    if (L.LightType == 0) //判断灯光什么类型 0平行光
    {
        return float4(1.f, 1.f, 1.f, 1.f);
    }
    else if (L.LightType == 1) //判断灯光什么类型 如果为1 则为点光源
    {

        //点光源的方向  点光源位置向量- 对象世界位置向量
        float3 LightVector = L.Position - InObjectWorldLocation;
        //点光源到物体的距离（灯光的方向转换为距离长度）
        float Distance = length(LightVector);
        
        //灯光距离判断 光源到对象的距离必须小雨光源衰减终点 只有小于才会被照亮
        if (Distance < L.EndAttenuation)
        {
            //return AttenuationPointLights1(L, Distance) * LightStrength; //线性衰减          
            return AttenuationPointLights2(
			L, //灯光
			Distance,// D受光点与光源距离	
			0.f, //C常熟衰减因子
            0.5f, //I线性衰减因子	0.9f); 
			0.9f); //Q平方衰减因子
        }  
    }
    if (L.LightType == 2) //序号2 聚光灯
    //{
        ////点光源的方向
        //float3 LightVector = L.Position - InObjectWorldLocation;;
        ////点光源到物体的距离
        //float Distance = length(LightVector);
        //
        //if (Distance < L.EndAttenuation)
        //{
        //    //聚光灯算法(面光源可用)                       灯光强度                         归一化灯光方向点乘灯光本身方向(旋转)      求最大值然后再次幂
        //    float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f) * pow(max(dot(NormalizeLightDirection, L.LightDirection), 0.f), 1.f);
        //
		//	return AttenuationPointLights1(L, Distance) * LightStrength; //线性衰减   
        //    //return AttenuationPointLights2(
		//	//L, //灯光
		//	//Distance, // D受光点与光源距离	
		//	//0.f, //C常熟衰减因子
        //    //0.5f, //I线性衰减因子	0.9f); 
		//	//0.9f); //Q平方衰减因子
        //}
    //}
    
    
    {
        //点光源的方向
        float3 LightVector = L.Position - InObjectWorldLocation;;
        //点光源到物体的距离
        float Distance = length(LightVector);
        //灯光距离判断
        if (Distance < L.EndAttenuation)
        {
                //点乘的值                  归一化灯光方向  点乘   灯光本身的方向        求最大值
            float DotValue = max(dot(NormalizeLightDirection, L.LightDirection), 0.f);
                
                //灯光强度                        当前强度       ×  输入的灯光强度
            float4 LightStrength = float4(1.f, 1.f, 1.f, 1.f) * float4(L.LightIntensity, 1.f);
        
             //计算聚光灯角度 θ
            float Theta1 = acos(DotValue);
            
            if (Theta1 == 0.f) //如果θ等于0
            {
                return LightStrength; //直接返回没有衰减的灯光强度
            }
            else if (Theta1 <= L.ConicalInnerCorner) //判断θ是否小于内角 
            {
                return LightStrength; //如果是聚光灯的内角光 则返回没有衰减的灯光强度
            }
            else if (Theta1 <= L.ConicalOuterCorner)//如果θ小于外角 
            {
                //聚光灯外角大于内角的部分开始有衰减
                //外角减去内角的值                  外角              内角
                float OuterInnerDistance = L.ConicalOuterCorner - L.ConicalInnerCorner;
                    
                //受光点与光源距离当前距离       外角减去内角的值  -  （θ - 内角）
                float CurrentDistance = OuterInnerDistance - (Theta1 - L.ConicalInnerCorner);
                    
                //聚光灯外角开始衰减 线性衰减算法   当前距离÷外角减去内角的值 ×光强度
                return (CurrentDistance / OuterInnerDistance) * LightStrength;
                    
                ////CIQ因子衰减
				//return AttenuationPointLights2(
				//	L,              //灯光
				//	CurrentDistance,//D受光点与光源距离	
				//0.f, //C常熟衰减因子
                //0.4f, //I线性衰减因子	0.9f); 
		        //0.3f); //Q平方衰减因子
            }
        } 
    }
        return float4(0.f, 0.f, 0.f, 1.f); //如果不是以上灯光类型给则返回
    }