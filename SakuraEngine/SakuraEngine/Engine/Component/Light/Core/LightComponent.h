#pragma once
#include "../../TransformationComponent.h"
#include "../../../Interface/DirectXDeviceInterfece.h"
#include "LightType.h"

class CMeshComponent;

/// �ƹ�������					�̳�����ҿ������			   �̳���Ⱦ���ݵĽӿ�
class CLightComponent :public CTransformationComponent, public IDirectXDeviceInterfece
{
	typedef CTransformationComponent Super;

	CVARIABLE()
		CMeshComponent* LightMesh; //�������ӻ�ƽ�й��ģ��
public:

	//����
	CLightComponent();

	//�Ƴ�
	virtual ~CLightComponent();

	//���õƹ�ǿ��
	void SetLightIntensity(const fvector_3d& InNewLightIntensity);
public:
	//��ȡ���ƹ�ģ�ͽӿ�
	CMeshComponent* GetLightMesh() { return LightMesh; }
	//��ȡ�ƹ�ǿ��
	const fvector_3d& GetLightIntensity() { return LightIntensity; }
	//��ȡ�ƹ�����
	ELightType GetLightType() const { return LightType; }
public:
	//���õƹ�λ��
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	//���õƹ���ת
	virtual void SetRotation(const fvector_3d& InNewRotation);
	//���õƹ�����
	virtual void SetScale(const fvector_3d& InNewScale);

	//���õƹ⳯������
	virtual void SetForwardVector(const XMFLOAT3& InForwardVector);
	virtual void SetRightVector(const XMFLOAT3& InRightVector);
	virtual void SetUPVector(const XMFLOAT3& InUPVector);

protected:
	//���õƹ�
	void SetLightMesh(CMeshComponent* InLightMesh);

protected:
	//���õƹ�ǿ��
	fvector_3d LightIntensity;
	//���õƹ�����
	ELightType LightType;
};