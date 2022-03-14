#include "Core/MeshType.h"
#include "SphereMesh.h"

void CSphereMesh::Init()
{
	Super::Init();//ִ�и���
}

void CSphereMesh::BuildMesh(const FMeshRendingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//ִ�и���

}

void CSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}

CSphereMesh* CSphereMesh::CreateMesh(
	float InRadius, //������ǰ뾶
	uint32_t InAxialSubdivision,//���������ϸ��
	uint32_t InHeightSubdivision) //����ĸ߶�ϸ��
{
	FMeshRendingData MeshData;//������ǰMesh�ṹ����

	//��������ļ���¦�
	float ThetaValue = XM_2PI / InHeightSubdivision;//�����Ǽ�����Ǧ� �߶���
	float BetaValue = XM_PI / InAxialSubdivision; //���������Ǧ� ����
	//������ʼ��
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(0.f,InRadius,0.f),XMFLOAT4(Colors::Red))); //������ɫ


	//��ʼ���㹹�����嶥��
	for (uint32_t i = 1; i< InAxialSubdivision; ++i)
	{
		float Beta = i * BetaValue;  //������ ������ת360��  ÿ��תһ�ܾ�������һ��
		//����������ĸ߶���
		for (uint32_t j = 0; j<= InHeightSubdivision; ++j)//�жϸ߶���
 		{
			float Theta = j * ThetaValue; //�����߶��� ����������ת180�� 
			//��������ת��Ϊ�ѿ�������
			MeshData.VertexDeta.push_back(FVertex(
				XMFLOAT3(
					InRadius * sinf(Beta) * cosf(Theta),//����X�� x = r * sin�� * cos�� r����뾶InRadius
					InRadius * cosf(Beta), //����Y�� y = r * cos��
					InRadius * sinf(Beta) * sinf(Theta)),//����Z�� z = r * sin�� * sin��
				XMFLOAT4(Colors::Red))); //������ɫ

			int TopIndex = MeshData.VertexDeta.size() - 1; //��ȡ����

			//���㷨�� �õ���DX��API 
			XMVECTOR Pos = XMLoadFloat3(&MeshData.VertexDeta[TopIndex].Position);//����תһ�½ṹ 
			XMStoreFloat3(&MeshData.VertexDeta[TopIndex].Normal, XMVector3Normalize(Pos));//�ڶ������ɷ���
 		}
	} 

	//��������ײ���ֹ��
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(0.f, -InRadius, 0.f), XMFLOAT4(Colors::Red))); //������ɫ


	//���Ʊ���
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//��������ʱ�� ��ʱ�뷨��������
	{
	
		MeshData.IndexDeta.push_back(0);//��һ��һ����0 
		MeshData.IndexDeta.push_back(Index +1);//2
		MeshData.IndexDeta.push_back(Index); //1    0-2-1��ɵ�������
	}


	float BaseIndex = 1;//��ʼ���� 1
	float VertexCircleNum = InAxialSubdivision + 1;//��תһȦ�Ķ�������
	

	 //�������м�Ļ���
	for (size_t i = 0; i< InHeightSubdivision - 2; ++i) //����ĸ߶����2�Ǽ�ȥ�ϱ���
	{
		for (size_t j = 0; j < InAxialSubdivision; ++j)//��ʼ���ƻ�Ȧ
		{
			//�����ı���
			//������1
			MeshData.IndexDeta.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexDeta.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexDeta.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//������2
			MeshData.IndexDeta.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexDeta.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexDeta.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);

		}

	}




	//�����ϼ�  �ϼ�����ʼ�������ǵ�ǰ�Ķ�������ȥ1
	uint32_t SouthBaseIndex =  MeshData.VertexDeta.size() - 1;//�ϼ�����ʼ�������ǵ�ǰ�Ķ�������ȥ1
	BaseIndex = SouthBaseIndex - VertexCircleNum;//�ϼ���Index������͵��� ��ǰ���ϼ���ʼ���ȥ�����ת���һȦ�����ټ�1������1��Ϊ����Index��������������
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//��������ʱ�� ��ʱ�뷨��������
	{
	
		MeshData.IndexDeta.push_back(SouthBaseIndex);//��һ��һ����0 
		MeshData.IndexDeta.push_back(BaseIndex + Index);//2
		MeshData.IndexDeta.push_back(BaseIndex + Index + 1); //1    0-2-1��ɵ�������
	}

	//����Mesh
	CSphereMesh* SphereMesh = new CSphereMesh;
	SphereMesh->BuildMesh(&MeshData);

	SphereMesh->Init();//ȷ��������֮���ʼ��

	return SphereMesh;
}
