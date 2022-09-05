#ifndef PBR
#define PBR



float3 GetDiffuseLambert(float3 DiffuseColor)
{
	float PI = 3.1415926;
	return DiffuseColor * (1 / PI);
}

float4 GetDistributionGGX(float3 N, float3 H, float Roughness)
{
	float PI = 3.1415926;

	float a2 = pow(Roughness, 4);

	float NoH = saturate(dot(N, H));
	float d = (NoH * NoH * (a2 - 1) + 1);

	return a2 / max((d * d * PI), 0.001);
}

float SchlickGGX(float NoV, float Roughness)
{
	float K_Dir = pow(Roughness, 2) / 8.f;

	return NoV / max(K_Dir + ((1 - K_Dir) * NoV), 0.001);
}

float GSmith(float3 N, float3 V, float3 L, float Roughness)
{
	float NoV = saturate(dot(N, V));
	float NoL = saturate(dot(N, L));

	return SchlickGGX(NoV, Roughness) * SchlickGGX(NoL, Roughness);
}

#endif