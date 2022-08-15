#pragma once
#include "ActorMeshConstruction.h"

//宏构造 

//自己定义的宏 构造渲染数据
#define CREATE_RENDER_DATA(T,...)\
ActorMeshConstruction::CreateMeshRenderData<T>(GetMeshManage(),this,__VA_ARGS__); //在GetMeshManage和this之后 是可变参数

//__VA_ARGS__仅支持win平台  其他平台使用##args