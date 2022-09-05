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
	//���÷���������
	void SetFresnelF0(const fvector_3d& InF0Vector);
	//����͸����
	void SetTransparency(float InTransparency);

	//���ò�����ͼ      ������ͼ·��
	void SetBaseColor(const std::string& InAssetFilename);
	//���÷�����ͼ
	void SetNormal(const std::string& InAssetFilename);
	//�����ж���̬���������޸�״̬	�������̬���ʱ��޸Ĺ��� 
	void SetDirty(bool bNewDirty);
	//���ò�������
	void SetMaterialIndex(int InNewIndex);
	//�����Ƿ�����̬����
	void SetDynamicReflection(bool InDynamicReflection);
	//���������
	void SetRefractiveValue(float InRefractiveValue);
public:
	//��̬���� ����
	FORCEINLINE float IsDynamicReflection() const {   //�ж��Ƿ��Ƕ�̬����
		return bDynamicReflection &&     //������Щ���ʿ����ж�̬����
			(MaterialType == EMaterialType::BackLight || //�жϲ��������Ƿ�Ϊ�˱���ɢ����� ����
				MaterialType == EMaterialType::Phong || //���߲�������Ϊ ��߹�
				MaterialType == EMaterialType::BinnPhong|| //�� ���ַ�߹�
				MaterialType == EMaterialType::Transparency);}//��������

	//��ȡ͸����
	FORCEINLINE float GetTransparency()const { return Transparency; }
	//��ȡ�ֲڶ�
	FORCEINLINE float GetRoughness() const { return Roughness; }
	//��ȡ������ɫ
	FORCEINLINE fvector_4d GetBaseColor() const { return BaseColor; }
	//��ȡ�߹�
	FORCEINLINE fvector_3d GetSpecularColor()const { return SpecularColor; }
	//��ȡ����������
	FORCEINLINE fvector_3d GetFresnelF0()const { return FresnelF0; }
	//��ȡ��������
	FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
	//��ȡ�����
	FORCEINLINE float GetRefractiveValue()const { return Refractive; }
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
	fvector_3d FresnelF0;//����������

	float Roughness; //���� �ֲڶ�
	EMaterialType MaterialType;//�������� ö��

	std::string BaseColorIndexKey;//��ȡ������ɫ��ͼ������·�����ַ�����
	std::string NormalIndexKey;//��ȡ������ͼ������·�����ַ�����
	std::string SpecularIndexKey;//��ȡ�߹���ͼ������·�����ַ�����

	EMaterialDisplayStatusType MaterialDisplayStatus;//������ʾ��ʽ
	XMFLOAT4X4 MaterialTransform;//���ʱ任4x4����

	//͸����
	float Transparency;

	//�Ƿ�����̬����
	bool bDynamicReflection;

	//�����
	float Refractive;
};