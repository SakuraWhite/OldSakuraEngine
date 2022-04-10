#include "RenderingData.h"
#include "../../../../../Mesh/Core/MeshType.h"
FRenderingData::FRenderingData()//��ʼ��
	: IndexSize(0) //Index������ʼ��
	, VertexSize(0) //����������ʼ��
	, IndexOffsetPosition(0) //Indexƫ�Ƶ�λ�ó�ʼ��
	, VertexOffsetPosition(0) //����ƫ�Ƶ�λ�ó�ʼ��
	, VertexTypeSize(sizeof(FVertex)) //���������ַ���С��ʼ��
	, IndexTypeSize(sizeof(uint16_t)) //Index�����ַ���С��ʼ��
	, IndexFormat(DXGI_FORMAT_R16_UINT)//Index��ʽ��ʼ��
	, WorldMatrix(EngineMath::IdentityMatrix4x4())//�ռ����굥λ��
	, Mesh(NULL)
{

}
