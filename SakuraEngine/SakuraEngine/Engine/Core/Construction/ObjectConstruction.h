#pragma once
//������


//���Ķ���
class CCoreMinimalObject;

template<class T> //����ģ��
T* CreateObject(CCoreMinimalObject* NewObject)//��������
{
	return dynamic_cast<T*>(NewObject);
}

template<class T, typename ...ParamTypes>
T* ConstructionObject(ParamTypes &&...Params)//�������  ����ʹ��
{
	return CreateObject<T>(new T(Params...));
}