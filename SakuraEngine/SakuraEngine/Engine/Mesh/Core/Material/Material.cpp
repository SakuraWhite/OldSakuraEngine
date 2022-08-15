#include "Material.h"
#include "../../../Math/EngineMath.h"

CMaterial::CMaterial()
	:bDirty(true)   //��ʼ���ж��Ƿ�Ϊ��̬���� Ĭ��Ϊtrue
	, MaterialIndex(-1)//��ʼ���������� -1Ϊ��
	,BaseColor(0.5f, 0.5f, 0.5f, 1.f) //��ʼ��������ɫ
	,SpecularColor(0.f, 0.f, 0.f)//��ʼ���߹���ɫ
	,Roughness(0.2f)  //��ʼ���ֲڶ�
	,MaterialType(EMaterialType::Lambert) //�������ͳ�ʼ��Ϊ������
	,MaterialDisplayStatus(EMaterialDisplayStatusType::TriangleDisplay) //������ʾ��ʼ����������ʾ
	,MaterialTransform(EngineMath::IdentityMatrix4x4())//��ʼ�����ʱ任����
{
}

void CMaterial::SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus)
{
	//�߿�/�����л�������ʾģʽ
	MaterialDisplayStatus = InDisplayStatus;
	//���������ʾģʽ ������ж���̬��ͼ����״̬Ϊtrue
	SetDirty(true);
}

void CMaterial::SetBaseColor(const fvector_4d& InBaseColor)
{
	//���������ɫ��RGB��������ͼ��
	BaseColor = InBaseColor;
	//�������ָ��RGB��ɫ  ������ж���̬��ͼ����״̬Ϊtrue
	SetDirty(true);
}

void CMaterial::SetMaterialType(const EMaterialType& InMaterialType)
{
	//�����������
	MaterialType = InMaterialType;

	//�������������� ������ж���̬��ͼ����״̬Ϊtrue
	SetDirty(true);
}

void CMaterial::SetRoughness(const float InRoughness)
{
	//����ֲڶ�
	Roughness = InRoughness;

	//�������ֲڶ� ������ж���̬��ͼ����״̬Ϊtrue
	SetDirty(true);
}

void CMaterial::SetSpecular(const std::string& InAssetFilename)
{
	//����߹����ͼ
	SpecularIndexKey = InAssetFilename;
	//��̬��ͼ����״̬Ϊtrue
	SetDirty(true);
}

void CMaterial::SetSpecular(const fvector_3d& InVector)
{
	//������ֵ���Ƹ߹�
	SpecularColor = InVector;
	//��̬��ͼ����״̬Ϊtrue
	SetDirty(true);
}

void CMaterial::SetBaseColor(const std::string& InAssetFilename)
{
	//�������ɫ��ͼ·��
	BaseColorIndexKey = InAssetFilename;

	//��������µ���ɫ��ͼ ������ж���̬��ͼ����״̬Ϊtrue
	SetDirty(true);
}

void CMaterial::SetNormal(const std::string& InAssetFilename)
{
	//����ķ�����ͼ·��
	NormalIndexKey = InAssetFilename;

	SetDirty(true);
}

void CMaterial::SetDirty(bool bNewDirty)
{
	//�����ж���̬������״̬
	bDirty = bNewDirty;
}

void CMaterial::SetMaterialIndex(int InNewIndex)
{
	//�����µĲ�������
	MaterialIndex = InNewIndex;
	//�����ж���̬������״̬
	SetDirty(true);
}
