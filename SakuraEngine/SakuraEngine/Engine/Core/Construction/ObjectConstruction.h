#pragma once
//对象构造


//核心对象
class CCoreMinimalObject;

template<class T> //制作模板
T* CreateObject(CCoreMinimalObject* NewObject)//创建对象
{
	return dynamic_cast<T*>(NewObject);
}

template<class T, typename ...ParamTypes>
T* ConstructionObject(ParamTypes &&...Params)//构造对象  传参使用
{
	return CreateObject<T>(new T(Params...));
}