#include "BoxMesh.h"
#include "Core/MeshType.h"

void CBoxMesh::Init()
{
	Super::Init();//ִ�и���
}

void CBoxMesh::BuildMesh(const FMeshRendingData* InRendingData)
{
	Super::BuildMesh(InRendingData);//ִ�и���

}

void CBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);//ִ�и���
}


CBoxMesh* CBoxMesh::CreateMesh(float InHeight, float InWidth, float InDepth)
{
	FMeshRendingData MeshData;//������ǰMesh�ṹ����

	//����ģ�����ĵ�
	float CHeight = 0.5f * InHeight;
	float CWidth = 0.5f * InWidth;
	float CDepth = 0.5f * InDepth;




	//��������
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White)));//
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, -CDepth),XMFLOAT4(Colors::AliceBlue)));//���Ƶڶ�����
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(CWidth, CHeight, -CDepth),XMFLOAT4(Colors::Aqua)));//���Ƶ�������
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::Aquamarine)));//���Ƶ��ĸ���
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, CDepth),XMFLOAT4(Colors::Bisque)));//���Ƶ������
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, CDepth),XMFLOAT4(Colors::Blue)));//���Ƶ�������
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(CWidth, CHeight, CDepth),XMFLOAT4(Colors::Chocolate)));//���Ƶ��߸���
	MeshData.VertexDeta.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, CDepth),XMFLOAT4(Colors::Chocolate)));//���Ƶڰ˸���


	//������������
	//��һ���ı��� ǰ
	MeshData.IndexDeta.push_back(0); MeshData.IndexDeta.push_back(1); MeshData.IndexDeta.push_back(2);//��һ��������
	MeshData.IndexDeta.push_back(0); MeshData.IndexDeta.push_back(2); MeshData.IndexDeta.push_back(3);//�ڶ��������沢�Һ͵�һ������һ���ı���
	//�ڶ����ı��� ��
	MeshData.IndexDeta.push_back(4); MeshData.IndexDeta.push_back(6); MeshData.IndexDeta.push_back(5);//������������
	MeshData.IndexDeta.push_back(4); MeshData.IndexDeta.push_back(7); MeshData.IndexDeta.push_back(6);//���ĸ�������

	//�������ı��� ��
	MeshData.IndexDeta.push_back(4); MeshData.IndexDeta.push_back(5); MeshData.IndexDeta.push_back(1);//�����������
	MeshData.IndexDeta.push_back(4); MeshData.IndexDeta.push_back(1); MeshData.IndexDeta.push_back(0);//������������

	//���ĸ��ı��� ��
	MeshData.IndexDeta.push_back(3); MeshData.IndexDeta.push_back(2); MeshData.IndexDeta.push_back(6);//���߸�������
	MeshData.IndexDeta.push_back(3); MeshData.IndexDeta.push_back(6); MeshData.IndexDeta.push_back(7);//�ڰ˸�������

	//������ı��� ��  
	MeshData.IndexDeta.push_back(1); MeshData.IndexDeta.push_back(5); MeshData.IndexDeta.push_back(6);//�ھŸ�������
	MeshData.IndexDeta.push_back(1); MeshData.IndexDeta.push_back(6); MeshData.IndexDeta.push_back(2);//��ʮ��������

	//�������ı��� ��
	MeshData.IndexDeta.push_back(4); MeshData.IndexDeta.push_back(0); MeshData.IndexDeta.push_back(3);//��ʮһ��������
	MeshData.IndexDeta.push_back(4); MeshData.IndexDeta.push_back(3); MeshData.IndexDeta.push_back(7);//��ʮ����������

	CBoxMesh* BoxMesh =new CBoxMesh;
	BoxMesh->BuildMesh(&MeshData);//�����������Bulidһ��

	BoxMesh->Init();//ȷ��������֮���ʼ��

	return BoxMesh;
}
