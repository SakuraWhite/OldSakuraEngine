#include "CoreMinimalObject.h"

//׼������
vector<CCoreMinimalObject*> GObject;//ȫ�ֶ���

CCoreMinimalObject::CCoreMinimalObject()
{
	bTick = true;

	GObject.push_back(this);
}

CCoreMinimalObject::~CCoreMinimalObject()
{
	for (vector<CCoreMinimalObject*>::const_iterator Iter = GObject.begin();
		Iter != GObject.end();
		++Iter)     //ʹ�ñ����ķ�ʽ���ж�ʲôʱ�����
	{
		if (*Iter == this)
		{
			GObject.erase(Iter);//�Ƴ�
			break;
		}
	}

}
