#include "CoreMinimalObject.h"

//׼������
vector<CCoreMinimalObject*> GObjects;//ȫ�ֶ���

CCoreMinimalObject::CCoreMinimalObject()
{
	bTick = true;

	GObjects.push_back(this);
}

CCoreMinimalObject::~CCoreMinimalObject()
{
	for (vector<CCoreMinimalObject*>::const_iterator Iter = GObjects.begin();
		Iter != GObjects.end();
		++Iter)     //ʹ�ñ����ķ�ʽ���ж�ʲôʱ�����
	{
		if (*Iter == this)
		{
			GObjects.erase(Iter);//�Ƴ�
			break;
		}
	}

}
