#pragma once//防止重复包含
#include "../../Rendering/Core/Rendering.h"
#include "../../Actor/Core/ActorObject.h"
#include "MeshType.h"

class CTransformationComponent;
//				继承来自GActorObject           这里是接口
class GMesh :public GActorObject,public IRenderingInterface
{
	CVARIABLE()  //反射宏
	CTransformationComponent* TransformationComponent;//我们希望模型物体能够移动 就需要控制组件
public:
	GMesh();//初始化Index与Vertex的一些缓冲区数据

	virtual void Init(); //Rendeing.h里的的渲染初始化

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);//用来构建Mesh接口

	virtual void PreDraw(float DeltaTime); //在模型里使用Rendeing.h的预渲染接口 
	virtual void Draw(float DeltaTime);  //在模型里使用Rendeing.h的渲染接口  进行时
	virtual void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw

	//static GMesh*CreateMesh(const FMeshRenderingData *InRenderingData); //静态结构来描述顶点数据 静态方式来创建Mesh



};