
//TBN����							�������編��				������������
float3x3 GetBuildTBNMatrix(float3 InUnitWorldNormal,float3 InWorldTangent)
{
	//����
	float3 N = InUnitWorldNormal;
	
	//					���������߼�ȥ   ���������ڷ����ϵ�ͶӰWt - dot(Wt,n)*N
    float3 T = normalize(InWorldTangent - dot(InWorldTangent, N) * N); //�������߼�ȥͶӰ��Ϊ�˽��� ��ֹԭ�������ߺͷ��߲��Ǵ�ֱ
	
	//������ = �����뷨�ߵĲ��
	float3 B = cross(N, T);

	return float3x3(T,B,N);
}