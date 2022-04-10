#include "Core/MeshType.h"
#include "SphereMesh.h"

void GSphereMesh::Init()
{
	Super::Init();//ִ�и���
}

void GSphereMesh::BuildMesh(const FMeshRenderingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//ִ�и���

}

void GSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}

void GSphereMesh::CreateMesh(FMeshRenderingData& MeshData,//������ǰMesh�ṹ����
	float InRadius, //������ǰ뾶
	uint32_t InAxialSubdivision,//���������ϸ��
	uint32_t InHeightSubdivision) //����ĸ߶�ϸ��
{

	//��������ļ���¦�
	float ThetaValue = XM_2PI / InHeightSubdivision;//�����Ǽ�����Ǧ� �߶���
	float BetaValue = XM_PI / InAxialSubdivision; //���������Ǧ� ����
	//������ʼ��
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f,InRadius,0.f),XMFLOAT4(Colors::White))); //������ɫ


	//��ʼ���㹹�����嶥��
	for (uint32_t i = 1; i< InAxialSubdivision; ++i)
	{
		float Beta = i * BetaValue;  //������ ������ת360��  ÿ��תһ�ܾ�������һ��
		//����������ĸ߶���
		for (uint32_t j = 0; j<= InHeightSubdivision; ++j)//�жϸ߶���
 		{
			float Theta = j * ThetaValue; //�����߶��� ����������ת180�� 
			//��������ת��Ϊ�ѿ�������
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InRadius * sinf(Beta) * cosf(Theta),//����X�� x = r * sin�� * cos�� r����뾶InRadius
					InRadius * cosf(Beta), //����Y�� y = r * cos��
					InRadius * sinf(Beta) * sinf(Theta)),//����Z�� z = r * sin�� * sin��
				XMFLOAT4(Colors::White))); //������ɫ

			int TopIndex = MeshData.VertexData.size() - 1; //��ȡ����

			//���㷨�� �õ���DX��API 
			XMVECTOR Pos = XMLoadFloat3(&MeshData.VertexData[TopIndex].Position);//����תһ�½ṹ 
			XMStoreFloat3(&MeshData.VertexData[TopIndex].Normal, XMVector3Normalize(Pos));//�ڶ������ɷ���
 		}
	} 

	//��������ײ���ֹ��
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, -InRadius, 0.f), XMFLOAT4(Colors::White))); //������ɫ


	//���Ʊ���
	for (uint32_t Index = 0; Index <= InAxialSubdivision; ++Index)//��������ʱ�� ��ʱ�뷨��������
	{
	
		MeshData.IndexData.push_back(0);//��һ��һ����0 
		MeshData.IndexData.push_back(Index +1);//2
		MeshData.IndexData.push_back(Index); //1    0-2-1��ɵ�������
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
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			//������2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);

		}

	}




	//�����ϼ�  �ϼ�����ʼ�������ǵ�ǰ�Ķ�������ȥ1
	uint32_t SouthBaseIndex =  MeshData.VertexData.size() - 1;//�ϼ�����ʼ�������ǵ�ǰ�Ķ�������ȥ1
	BaseIndex = SouthBaseIndex - VertexCircleNum;//�ϼ���Index������͵��� ��ǰ���ϼ���ʼ���ȥ�����ת���һȦ�����ټ�1������1��Ϊ����Index��������������
	for (uint32_t Index = 0; Index < InAxialSubdivision; ++Index)//��������ʱ�� ��ʱ�뷨��������
	{
	
		MeshData.IndexData.push_back(SouthBaseIndex);//��һ��һ����0 
		MeshData.IndexData.push_back(BaseIndex + Index);//2
		MeshData.IndexData.push_back(BaseIndex + Index + 1); //1    0-2-1��ɵ�������
	}


	//����
	//MeshData.IndexData[SouthBaseIndex - 2];
	//MeshData.IndexData[SouthBaseIndex - 3];

}
