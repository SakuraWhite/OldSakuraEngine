#pragma once//防止重复包含
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"


//需要继承一下继承来自C开头的核心                      这里是接口
class CMesh :public CCoreMinimalObject ,public IRenderingInterface
{
public:
	CMesh();//初始化Index与Vertex的一些缓冲区数据

	virtual void Init(); //Rendeing.h里的的渲染初始化

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);//用来构建Mesh接口

	virtual void PreDraw(float DeltaTime); //在模型里使用Rendeing.h的预渲染接口 
	virtual void Draw(float DeltaTime);  //在模型里使用Rendeing.h的渲染接口  进行时
	virtual void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw

	//static CMesh*CreateMesh(const FMeshRenderingData *InRenderingData); //静态结构来描述顶点数据 静态方式来创建Mesh


};