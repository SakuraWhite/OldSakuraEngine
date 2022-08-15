#pragma once
#include "../Core/CoreObject/CoreMinimalObject.h"

class CLightComponent;

class CLightManage :public CCoreMinimalObject
{

	
	friend struct FGeometryMap;//有源 直接被调用
public:
	void AddLight(CLightComponent* InLightStance);//添加灯光
protected:
	//储存灯光相关的信息
	vector<CLightComponent*> Lights;
};