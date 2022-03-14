#include "CoreMinimalObject.h"

//准备容器
vector<CCoreMinimalObject*> GObject;//全局对象

CCoreMinimalObject::CCoreMinimalObject()
{
	bTick = true;

	GObject.push_back(this);
}

CCoreMinimalObject::~CCoreMinimalObject()
{
	for (vector<CCoreMinimalObject*>::const_iterator Iter = GObject.begin();
		Iter != GObject.end();
		++Iter)     //使用遍历的方式来判断什么时候结束
	{
		if (*Iter == this)
		{
			GObject.erase(Iter);//移除
			break;
		}
	}

}
