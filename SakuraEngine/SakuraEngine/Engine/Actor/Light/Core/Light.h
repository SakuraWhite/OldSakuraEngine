#pragma  once 
#include "../../Core/ActorObject.h"

class CLightComponent;

// ���Դ����		  �̳���GActorObject���Ķ���
class GLight : public GActorObject
{
	typedef GActorObject Super;

	CVARIABLE()  //�����
	CLightComponent* LightComponent;//����ĵ�

public:
	GLight();

	virtual void Tick(float DeltaTime);//����ִ֡��

public:
	//���õƹ��λ������
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	//���õƹ����ת����  ŷ����
	virtual void SetRotation(const fvector_3d& InRotation);
	//���õƹ����������
	virtual void SetScale(const fvector_3d& InNewScale);
	
public:
	//���õƹ�ǿ��
	void SetLightIntensity(const fvector_3d& InNewLightIntensity);

protected:
	//���õƹ����
	virtual void SetLightComponent(CLightComponent* InNewComponent);

public:
	//��ݻ�ȡ�ƹ����
	const CLightComponent* GetLightComponent() const { return  LightComponent; }
};