#include "Material.hlsl"

struct MeshVertexIn //Mash的顶点 顶点输入IN
{
	float3 Position : POSITION;//顶点位置 XYZ三通道 最后指定为POSITION位置
	float4 Color : COLOR;//顶点颜色 RGBA四通道 最后指定为COLOR颜色
	float3 Normal : NORMAL;//法线
    float3 UTangent : TANGENT;//切线 U方向
    float2 TexCoord : TEXCOORD; //UV坐标
};

struct MeshVertexOut //Mash的顶点输出 out
{
    float4 WorldPosition : POSITION; //世界空间坐标位置
	float4 Position : SV_POSITION;  //顶点输出必须要有齐次坐标，所以是float4 XYZW
	float4 Color : COLOR;	//顶点颜色
	float3 Normal : NORMAL;//法线
    float3 UTangent : TANGENT; //切线 U方向
    float2 TexCoord : TEXCOORD; //UV坐标
	
};


MeshVertexOut VertexShaderMain(MeshVertexIn MV)  //入口 在Mash.cpp中VertexShaderMain入口 顶点着色器   传入MeshVertex中定义的POSITION与COLOR含义
{
    //取出材质   
    MaterialConstBuffer MatConstBuffer = Materials[MaterialIndex];
    
    MeshVertexOut Out;

    //颜色初始化
    Out.Color = MV.Color;
    
	//世界坐标位置                 用模型当前的位置去乘以世界坐标的矩阵  
    Out.WorldPosition = mul(float4(MV.Position, 1.f), WorldMatrix);
	
	// 将世界坐标位置转换为屏幕空间坐标(齐次裁剪空间坐标)    世界空间坐标去乘视图投影矩阵
    Out.Position = mul(Out.WorldPosition, ViewProjectionMatrix);
    
	
	//用来判断是否需要把世界法线转换为局部法线（材质用）
    if (MatConstBuffer.MaterialType == 13) 
    {
		
        Out.Normal = MV.Normal; //法线(局部)

    }
	else
    {
		//通过世界矩阵 来得到世界空间法线
        Out.Normal = mul(MV.Normal, (float3x3) WorldMatrix);
    }
    //通过世界矩阵 来得到世界空间切线
    Out.UTangent = mul(MV.UTangent, (float3x3) WorldMatrix);
	
    //UV坐标矩阵转换                  uv坐标     X      Y         
    float4 MyTexCoord = mul(float4(MV.TexCoord, 0.0f, 1.f), ObjectTextureTransform);
    //转换UV坐标          UV坐标     材质坐标
    Out.TexCoord = mul(MyTexCoord, MatConstBuffer.TransformInformation).xy;
    
    
	//计算并输出顶点颜色
    Out.Color.rgb = MV.Normal.rgb;

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) : SV_TARGET //入口 在Mash.cpp中的PixelShaderMain入口  像素着色器 传入MeshVertex中定义的COLOR含义
{
        
    //取出材质   
    MaterialConstBuffer MatConstBuffer = Materials[ MaterialIndex];
    
	//使用材质hlsl来定义颜色  (模型本身的材质 基本颜色)
    FMaterial Material;
    
    //赋予材质颜色贴图                          材质缓冲区           UV
    Material.BaseColor = GetMaterialBaseColor(MatConstBuffer, MVOut.TexCoord);
  
	//高光贴图/颜色
    float4 Specular = GetMaterialSpecular(MatConstBuffer, MVOut.TexCoord); //高光反射的颜色 
    
    
	//不受环境光影响的基础材质  基础basecolor模型
    if (MatConstBuffer.MaterialType == 12)
    {
        //          基本颜色 ×高光 +基本颜色+0.1
        return Material.BaseColor * Specular + Material.BaseColor + 0.1f;
    }
    else if (MatConstBuffer.MaterialType == 13)  //法线(局部)
    {
        return float4(MVOut.Normal, 1.f);
    }
    else if (MatConstBuffer.MaterialType == 14)  //世界法线
    {
        return float4(MVOut.Normal, 1.f);
    }
	
	//计算需要的数据
    float4 AmbientLight = { 0.15f, 0.15f, 0.25f, 1.0f }; //因为灯光的方向问题 这里写了一个固定的环境光照
    float3 ModelNormal = normalize(MVOut.Normal); //单位化法线 模型

    //获取法线贴图                        材质缓冲区          UV       单位化法线      世界切线
    ModelNormal = GetMaterialNormals(MatConstBuffer, MVOut.TexCoord, ModelNormal, MVOut.UTangent);
    
	//光照模型计算所需要的函数
    float DotValue = 0; //序号0 兰伯特
    
    //灯光强度（多个）
    float4 LightStrengths = { 0.f, 0.f, 0.f, 1.f }; 
    
	
	//遍历场景中的灯光
    for (int i = 0; i < 16; i++)
    {
        //判断灯光是否有效 我们要保证灯光信息长度大于0
        if (length(SceneLights[i].LightIntensity.xyz) > 0.f)
        {
            //单位化灯光方向                            单位化灯光方向    传入场景灯光类型序号    世界位置
            float3 NormalizeLightDirection = normalize(GetLightDirection(SceneLights[i], MVOut.WorldPosition.xyz));
            
            //判断场景中灯光强度（单个)                    指定灯光结构   指定法线(单位化)     指定世界位置       指定单位化灯光方向
            float4 LightStrength = ComputeLightStrength(SceneLights[i], ModelNormal, MVOut.WorldPosition.xyz, NormalizeLightDirection);
            
		    

        
                //光照模型
            if (MatConstBuffer.MaterialType == 0) //判断选择的材质类型 序号0 兰伯特
            {
		        //兰伯特   次幂          点乘法线与灯光方向 同时值限制到0-1限制最大值~限制的最小值 最后的值二次方 来进行多个灯光的平滑过渡
                DotValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0),2.f);
            }
            else if (MatConstBuffer.MaterialType == 1) //序号1 半兰伯特 
            {
		        //点乘法线与灯光 求出法线与灯光的夹角度数
                float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
		        //半兰伯特      模型法线点乘灯光 再乘以0.5 加0.5  值域限制为0~1
                DotValue = max((DiffuseReflection * 0.5f + 0.5f), 0.0);
            }
            else if (MatConstBuffer.MaterialType == 2) //序号2 冯Phong 
            {
		        //光线反射方向   		     单位化	       用内置的反射计算函数  输入光线的方向 与法线  然后值求反
                float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
		
		        //摄像机方向(朝向)		单位化	摄像机(视口)位置的xyz 减去 世界空间顶点位置的xyz
                float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
	
		        //兰伯特					法线点乘光方向
                DotValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f);
		
		        //冯
                //使用粗糙度来控制高光范围          把粗糙度限制到0-1范围内
                float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
			    //冯模型的高光次幂   高光次幂最大值为256   使用粗糙度映射控制高光范围
                float M = MaterialShininess * 100.f;
                
                //冯的归一化 m+2/2Π × (max((h · n)^m)/Π   视角方向点乘光线反射方向
                Specular *= saturate((M + 2.0f) * pow(max(dot(ViewDirection, ReflectDirection), 0.f), M) / 3.1415926);
			    //普通冯归一化之前		       点乘摄像机方向与光线反射方向
                //Specular = pow(max(dot(ViewDirection, ReflectDirection), 0.f), M);
            }
            else if (MatConstBuffer.MaterialType == 3) //序号3 布林冯BlinnPhong 
            {
		
		        //摄像机方向(朝向)		单位化	摄像机(视口)位置的xyz 减去 世界空间顶点位置的xyz
                float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
		
		        //光线反射方向(半角)   		单位化	 摄像机(视角)方向 + 光线方向
                float3 HalfDirection = normalize(ViewDirection + NormalizeLightDirection);
		

		        //兰伯特		     先半兰博特化 再减去0.2f曝光的，再平方，变得更柔和   
                DotValue = pow(max(0.0, (dot(ModelNormal, NormalizeLightDirection) * 0.5f + 0.5f) - 0.2f), 2);
		
		        //冯
                //使用粗糙度来控制高光范围          把粗糙度限制到0-1范围内
                float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
			    //冯模型的高光次幂   高光次幂最大值为256   使用粗糙度映射控制高光范围
                float M = MaterialShininess * 100.f;
                
                //冯的归一化 m+2/2Π × (max((h · n)^m)/Π   视角方向半角向量
                Specular *= saturate((M + 2.0f) * pow(max(dot(HalfDirection, ModelNormal), 0.f), M) / 3.1415926); //c=(m+2.f/PI) 归一化系数
              
                //普通冯高光半角向量  点乘   模型法线向量
                //Specular = pow(max(dot(HrelfDirection, ModelNormal), 0.f), M); //归一化之前的冯 R= max(h·n)^m

		
            }
            else if (MatConstBuffer.MaterialType == 4) //序号4  WrapLight 
            {
                //float WrapValue = 1.f;  当WrapLight的控制值为1时 计算结果等于半兰伯特
                float WrapValue = 1.6f; //用作控制WrapLight的效果
		
		        //点乘法线与灯光 求出法线与灯光的夹角度数
                float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
		
		        //WrapLight      模型法线点乘灯光 加控制值         再除以1-控制值    值域限制为0~1
                DotValue = max((DiffuseReflection + WrapValue) / (1.f + WrapValue), 0.0);
		
            }
            else if (MatConstBuffer.MaterialType == 5) //序号5 Minnaert Lighting 
            {
		        //摄像机方向(朝向)		单位化	摄像机(视口)位置的xyz 减去 世界空间顶点位置的xyz
                float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
		
		        //光线计算结果(兰伯特)				   法线点乘光方向
                float DotLight = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f);
		        //视角计算结果					法线点乘摄像机方向
                float DotView = max(dot(ModelNormal, ViewDirection), 0.0);
		
		
		        //使用粗糙度来控制Minnaert范围          把粗糙度限制到0-1范围内
                float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
		        //Minnaert的范围   高光次幂最大值为256   使用粗糙度映射控制高光范围
                float M = MaterialShininess * 10.f;
		
		        //Minnaert的计算   光线计算结果×视角计算结果 用Pow来控制范围  最后再乘以光线计算结果加强效果  最终的值限制在0~1内
                DotValue = saturate(DotLight * pow(DotLight * DotView, M));
		
            }
            else if (MatConstBuffer.MaterialType == 6) //序号6 Banded 基础卡通材质 
            {
		
		        //作为卡通材质的基础反射	  点乘法线与灯光 求出法线与灯光的夹角度数   半角  然后类似半兰伯特一样 最后的值+1 值域变成0~2然后 × 0.5
                float DiffuseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1) * 0.5;
		        //作为卡通材质的分层 分为两层
                float Layered = 3.f;
		        //使用向上取整来制作基础卡通材质   
                DotValue = floor(DiffuseReflection * Layered) / Layered;

            }
            else if (MatConstBuffer.MaterialType == 7) //序号7 GradualBanded 具有渐变的卡通材质
            {
		        //准备一个颜色 作为渐变色混合
                float4 Color2 = { 245.f / 255.f, 88.f / 255.f, 0.0f, 1.0f };
		
		        //作为卡通材质的基础反射 基于半兰伯特
                float LightDotValue = dot(ModelNormal, NormalizeLightDirection); //法线点乘光纤 NdotL
                float DiffuseReflection = (LightDotValue + 1) * 0.5; //半兰伯特 + 1 × 0.5
		
		        //作为卡通材质的分层 分为两层
                float Layered = 3.f;
		        //使用向上取整来制作基础卡通材质   
                DotValue = floor(DiffuseReflection * Layered) / Layered;
		
		        //混合颜色												LightDotValue充当Alpha来控制颜色混合
                Material.BaseColor = lerp(Color2, Material.BaseColor, LightDotValue);

            }
            else if (MatConstBuffer.MaterialType == 8) //序号8 FinalBanded   自定义的卡通材质 融合了很多效果的卡通材质
            {
		
		        //卡通材质表面反射
		        //融入半兰伯特作为卡通基础表面	  点乘法线与灯光 求出法线与灯光的夹角度数   半角  然后类似半兰伯特一样 最后的值+1 值域变成0~2然后 × 0.5
                float DiffuseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;
		        //作为卡通材质的分层 分为4层
                float Layered = 4.f;
		        //使用向上取整来制作基础卡通材质   
                DotValue = floor(DiffuseReflection * Layered) / Layered;
		
		        //卡通材质的菲涅尔效果
		        //f0   F0作为模拟金属的反射率  
                float3 F0 = { 0.05f, 0.05f, 0.05f };
		        //摄像机方向
                float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
		        //把菲涅尔塞到了高光里  封装好的菲涅尔方法   f0   顶点法线      摄像机方向
                Specular.xyz = FresnelSchlickMethod(F0, ModelNormal, ViewDirection, 3).xyz;
		
		        //光线反射方向   		     单位化	       用内置的反射计算函数  输入光线的方向 与法线  然后值求反
                float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
		
		        //卡通材质的高光
		        //冯  在兰伯特的值大于零的区域 才会有高光
                if (DotValue > 0.f)
                {

			        //使用粗糙度来控制高光范围          把粗糙度限制到0-1范围内
                    float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
			        //冯模型的高光次幂   高光次幂最大值为256   使用粗糙度映射控制高光范围
                    float M = MaterialShininess * 60.f;
			        //冯作为高光	    让高光更加锐利（测出来的值）
                    Specular += saturate(Specular + pow(max(dot(ViewDirection, ReflectDirection), 0.f), M) / 0.0314f);
                }
		
            }
            else if (MatConstBuffer.MaterialType == 9)//序号9 BackLight   模型透射模拟
            {
		        //光线反射方向   		
                float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
		        //摄像机方向(朝向)
                float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);

		        //做模型表面的Wrap
		        //float WrapValue = 1.f;
                float WrapValue = 1.6f; //用作控制WrapLight的效果
		        //点乘法线与灯光 求出法线与灯光的夹角度数
                float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
		        //WrapLight
                DotValue = max((DiffuseReflection + WrapValue) / (1.f + WrapValue), 0.0);
		
		        //做高光冯    
                if (DotValue > 0.f)
                {
			        //使用粗糙度来控制高光范围 
                    float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
			        //冯模型的高光次幂  
                    float M = MaterialShininess * 100.f;
			        //冯作为高光	
                    Specular = saturate(pow(max(dot(ViewDirection, ReflectDirection), 0.f), M));
                }
		
		        //创建一个系数来控制投射出光线的角度  也叫光线偏折率
                float SSSValue = 3.f;
		        //控制透射光线的强度
                float TransmissionIntensity = 2.f;
                //控制透射范围
                float TransmissionScope = 1.5f;
		        //模拟透射出的光线		          法线先乘以一个光线偏移的系数然后再加上光线方向再取反
                float3 LightNormalizeDotValue = -normalize(ModelNormal * SSSValue + NormalizeLightDirection);
		        //			叠加其他效果		我们把透射之后的光线与相机方向点乘 让其跟随摄像机移动而变化
                DotValue = DotValue + pow(saturate(dot(LightNormalizeDotValue, ViewDirection)), TransmissionScope) * TransmissionIntensity;

            }
            else if (MatConstBuffer.MaterialType == 10)//序号10  AnisotropyKajiyaKay 各向异性
            {
		
                
            }
            else if (MatConstBuffer.MaterialType == 11)//序号11  OrenNayar GDC 类沙漠粗糙表面效果
            {
		        //摄像机方向(朝向)
                float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
		        //半角计算方式 法线点乘光线  兰伯特效果
                float NormalLight = saturate(pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f));
		        //视角计算结果   法线点乘视角方向
                float NormalView = saturate(dot(ModelNormal, ViewDirection));

		        //Φ  模长       
                float PhiRI =
				length(ViewDirection - ModelNormal * NormalView) * //摄像机方向 - (模型法线 × NormalView)关于法线的缩放   视角到法线的距离 模长
				length(NormalizeLightDirection - ModelNormal * NormalLight); //光线方向 - (模型法线 × NormalView)关于法线的缩放   灯光到法线的距离 模长
		        //反余弦
                float ACosNormalView = acos(NormalView);
                float ACosNormalLight = acos(NormalLight);
		
		        //计算 α与В
                float Alpha = max(ACosNormalView, ACosNormalLight); //在光线与视线中谁与模型平面的切线越近 夹角θ越小 就取谁的值
                float Beta = min(ACosNormalView, ACosNormalLight); ////在光线与视线中谁与模型平面的法线越近 夹角θ越小  就取谁的值
		
		        //粗糙度影响因子   当粗糙度为0时  A = 1 B = 0 则下面计算公式结果为兰伯特
                float MyRoughness = pow(MatConstBuffer.MaterialRoughness, 2);
                float A = 1 - 0.5f * (MyRoughness / (MyRoughness + 0.33f));
                float B = 0.45f * (MyRoughness / (MyRoughness + 0.09f));
		
		        //      法线点乘光线	 A&B粗糙度影响因子	 上面计算的模长	L与V在切线与法线的夹角    兰伯特  辐射度(能量)
		        //		       N/L	   ·（A + B · max[0, cos(Φi - Φr)] · sinα α · tanВ)      ·   cosθi E0
                DotValue = NormalLight * (A + B * max(0, PhiRI) * sin(Alpha) * tan(Beta));

            }
            else if (MatConstBuffer.MaterialType == 100) //序号100 菲涅尔  菲涅尔是效果 并不是材质
            {
		        //在Material.hlsl中封装的菲涅尔算法
		        
		        //f0   F0作为模拟金属的反射率  
                float3 F0 = { 0.02f, 0.02f, 0.02f };
		        //摄像机方向
                float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
		        //把菲涅尔塞到了高光里  封装好的菲涅尔方法   f0   顶点法线      摄像机方向
                Specular.xyz = FresnelSchlickMethod(F0, ModelNormal, ViewDirection, 3).xyz;
		
            }
            //光线强度(多个)   限制到0-1   原本的光线强度(单个) × 兰伯特  ×     自己设定的灯光强度 
            LightStrengths += saturate(LightStrength * DotValue * float4(SceneLights[i].LightIntensity, 1.f)); //计算多盏灯光对一个像素产生的效果            
            LightStrengths.w= 1.f;     //把W设置为1即可（灯光XYZW  W不参与计算）
            
            //把这些属性限制到 0-1
            LightStrengths = saturate(LightStrengths);
            Specular = saturate(Specular);
            Material.BaseColor = saturate(Material.BaseColor);
        }
    }
	
	
	//计算并输出像素颜色（最终颜色贡献）
    MVOut.Color = LightStrengths * (Material.BaseColor //灯光强度乘以模型颜色 漫反射
                 + Specular * Material.BaseColor)  //灯光强度再影响高光与颜色  高光
				  + AmbientLight * Material.BaseColor; //环境光照乘以模型颜色 间接光(环境光)

		return MVOut.Color;
	
}