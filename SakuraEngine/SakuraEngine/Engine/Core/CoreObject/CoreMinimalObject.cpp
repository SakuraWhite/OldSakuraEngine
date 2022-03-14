#include "CoreMinimalObject.h"

//准备容器
vector<CCoreMinimalObject*> GObjects;//全局对象

CCoreMinimalObject::CCoreMinimalObject()
{
	bTick = true;

	GObjects.push_back(this);
}

CCoreMinimalObject::~CCoreMinimalObject()
{
	for (vector<CCoreMinimalObject*>::const_iterator Iter = GObjects.begin();
		Iter != GObjects.end();
		++Iter)     //使用遍历的方式来判断什么时候结束
	{
		if (*Iter == this)
		{
			GObjects.erase(Iter);//移除
			break;
		}
	}

}
