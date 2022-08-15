
//TBN矩阵							输入世界法线				输入世界切线
float3x3 GetBuildTBNMatrix(float3 InUnitWorldNormal,float3 InWorldTangent)
{
	//法线
	float3 N = InUnitWorldNormal;
	
	//					用世界切线减去   世界切线在法线上的投影Wt - dot(Wt,n)*N
    float3 T = normalize(InWorldTangent - dot(InWorldTangent, N) * N); //世界切线减去投影是为了矫正 防止原来的切线和法线不是垂直
	
	//负切线 = 切线与法线的叉乘
	float3 B = cross(N, T);

	return float3x3(T,B,N);
}