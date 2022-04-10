#include "BoxMesh.h"
#include "Core/MeshType.h"

void GBoxMesh::Init()
{
	Super::Init();//ִ�и���
}

void GBoxMesh::BuildMesh(const FMeshRenderingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//ִ�и���

}

void GBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}


void GBoxMesh::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth)
{
	

	//����ģ�����ĵ�
	float CHeight = 0.5f * InHeight;
	float CWidth = 0.5f * InWidth;
	float CDepth = 0.5f * InDepth;




	//��������
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White)));//
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, -CDepth),XMFLOAT4(Colors::White)));//���Ƶڶ�����
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, CHeight, -CDepth),XMFLOAT4(Colors::White)));//���Ƶ�������
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White)));//���Ƶ��ĸ���
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, CDepth),XMFLOAT4(Colors::White)));//���Ƶ������
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, CDepth),XMFLOAT4(Colors::White)));//���Ƶ�������
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, CHeight, CDepth),XMFLOAT4(Colors::White)));//���Ƶ��߸���
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, CDepth),XMFLOAT4(Colors::White)));//���Ƶڰ˸���


	//������������
	//��һ���ı��� ǰ
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(2);//��һ��������
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(3);//�ڶ��������沢�Һ͵�һ������һ���ı���
	//�ڶ����ı��� ��
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(5);//������������
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(7); MeshData.IndexData.push_back(6);//���ĸ�������

	//�������ı��� ��
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(1);//�����������
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(0);//������������

	//���ĸ��ı��� ��
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(6);//���߸�������
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(7);//�ڰ˸�������

	//������ı��� ��  
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(6);//�ھŸ�������
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(2);//��ʮ��������

	//�������ı��� ��
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(3);//��ʮһ��������
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(7);//��ʮ����������
}
