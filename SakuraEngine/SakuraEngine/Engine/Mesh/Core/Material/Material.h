#pragma once//��ֹ�ظ�����
#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

//		����		�̳�����CCoreMinimalObject
class CMaterial :public CCoreMinimalObject
{
public:
	CMaterial();

	// ���õ�ǰģ�͵���ʾ״̬ �߿�/�����л�		������ʾģʽ
	void SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus);
	// ���û�����ɫ
	void SetBaseColor(const fvector_4d& InBaseColor);
	// ���ò�������		
	void SetMaterialType(const EMaterialType& InMaterialType);
	// ���ôֲڶ�		
	void SetRoughness(const float InRoughness);
	//���ø߹�
	void SetSpecular(const std::string& InAssetFilename);
	void SetSpecular(const fvector_3d& InVector);//����ֵ�ķ�ʽ�߹�

	//���ò�����ͼ      ������ͼ·��
	void SetBaseColor(const std::string& InAssetFilename);
	//���÷�����ͼ
	void SetNormal(const std::string& InAssetFilename);
	//�����ж���̬���������޸�״̬	�������̬���ʱ��޸Ĺ��� 
	void SetDirty(bool bNewDirty);
	//���ò�������
	void SetMaterialIndex(int InNewIndex);
public:
	//��ȡ������ɫ
	FORCEINLINE fvector_4d GetBaseColor() const { return BaseColor; }
	//��ȡ�ֲڶ�
	FORCEINLINE float GetRoughness() const { return Roughness; }
	//��ȡ�߹�
	FORCEINLINE fvector_3d GetSpecularColor()const { return SpecularColor; }
	//��ȡ��������
	FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
	//��ȡ������ʾ��ʽ
	FORCEINLINE EMaterialDisplayStatusType GetMaterialDisplayStatus() const { return MaterialDisplayStatus; }
	//��ȡ���ʱ任
	FORCEINLINE XMFLOAT4X4& GetMaterialTransform() { return MaterialTransform; }

	//�ж� ��̬�����Ƿ��޸�
	FORCEINLINE bool IsDirty() const { return bDirty; }
	//��ȡ������ɫ��ͼ������·�����ַ�����
	FORCEINLINE const std::string& GetBaseColorIndexKey() const { return BaseColorIndexKey; }
	//��ȡ������ͼ������·�����ַ�����
	FORCEINLINE const std::string& GetNormalIndexKey() const { return NormalIndexKey; }
	//����߹���ͼ����·�����ַ�����
	FORCEINLINE const std::string& GetSpecularKey() const { return SpecularIndexKey; }
	//��ȡ��������
	FORCEINLINE int GetMaterialIndex() const { return MaterialIndex; }
private:
	bool bDirty;//�ж������ǲ��Ƕ�̬���޸Ĺ��ˣ��ǵĻ��͸���
	int MaterialIndex;//��������

	fvector_4d BaseColor; // ���� ������ɫ(ָ����RGB��ֵ ����ͼ)
	fvector_3d SpecularColor;//�߹���ɫ(ָ����ֵ)
	float Roughness; //���� �ֲڶ�

	std::string BaseColorIndexKey;//��ȡ������ɫ��ͼ������·�����ַ�����
	std::string NormalIndexKey;//��ȡ������ͼ������·�����ַ�����
	std::string SpecularIndexKey;//��ȡ�߹���ͼ������·�����ַ�����

	EMaterialType MaterialType;//�������� ö��
	EMaterialDisplayStatusType MaterialDisplayStatus;//������ʾ��ʽ
	XMFLOAT4X4 MaterialTransform;//���ʱ任4x4����
};