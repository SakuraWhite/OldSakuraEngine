#include "MeshType.h"
#include "MeshManage.h"

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor)
	:Position(InPos) //��ʼ��λ��
	,Color(InColor)  //��ʼ��������ɫ
{
}

FVertex::FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3& InNormal, const XMFLOAT2& InTexCoord)
	: Position(InPos) //��ʼ��λ��
	, Color(InColor)  //��ʼ��������ɫ
	, Normal(InNormal) //��ʼ������
	, TexCoord(InTexCoord)//��ʼ��UV����

{
}
