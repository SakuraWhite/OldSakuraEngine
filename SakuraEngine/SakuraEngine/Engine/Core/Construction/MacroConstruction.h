#pragma once
#include "ActorMeshConstruction.h"

//�깹�� 

//�Լ�����ĺ� ������Ⱦ����
#define CREATE_RENDER_DATA(T,...)\
ActorMeshConstruction::CreateMeshRenderData<T>(GetMeshManage(),this,__VA_ARGS__); //��GetMeshManage��this֮�� �ǿɱ����

//__VA_ARGS__��֧��winƽ̨  ����ƽ̨ʹ��##args