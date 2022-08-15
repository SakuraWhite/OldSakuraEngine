#pragma  once 
#include "../../EngineMinimal.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"

class CTransformationComponent;

// G �� game����˼  gameplay  �̳���CCoreMinimalObject���Ķ���
class GActorObject : public CCoreMinimalObject
{

	CVARIABLE()  //�����
	CTransformationComponent* TransformationComponent;//����ϣ��������ܹ��ƶ� ����Ҫ�������

public:
	GActorObject();

public:
	FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }
public:
	//���������λ������
	void SetPosition(const XMFLOAT3& InNewPosition);
	//�����������ת����  ŷ����
	void SetRotation(const fvector_3d& InRotation);
	//�����������������
	void SetScale(const fvector_3d& InNewScale);
public:
	//��ȡ����λ����Ϣ
	XMFLOAT3& GetPosition();
	//��ȡ������ת��Ϣ
	fvector_3d GetRotation()const;
	//��ȡ����������Ϣ
	fvector_3d GetScale()const;


	//������峯������  �������Լ��ĳ��� ����Ҫ�޸�����Ķ���
	XMFLOAT3& GetForwardVector();
	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUPVector();
};