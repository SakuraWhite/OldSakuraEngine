#pragma once
#include "../Core/CoreObject/CoreMinimalObject.h"

class CLightComponent;

class CLightManage :public CCoreMinimalObject
{

	
	friend struct FGeometryMap;//��Դ ֱ�ӱ�����
public:
	void AddLight(CLightComponent* InLightStance);//��ӵƹ�
protected:
	//����ƹ���ص���Ϣ
	vector<CLightComponent*> Lights;
};