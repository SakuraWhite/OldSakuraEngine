#pragma once//防止重复包含

#include "core/Mesh.h"

class CBoxMesh :public CMesh   //生成一个标准化的BOX 继承自FMesh
{
	typedef CMesh Super;
public:
	virtual void Init();//继承自Mesh.h里的Rendeing.h里的的渲染初始化 
	
	virtual void BuildMesh(const FMeshRendingData* InRenderIngData);//继承自Mesh.h里的用来构建Mesh接口
	
	virtual void Draw(float DeltaTime);//继承自Mesh.h里的在模型里使用Rendeing.h的渲染接口

	static CBoxMesh* CreateMesh(
		float InHeight,   //BOX高度
		float InWidth,   //BOX宽度
		float InDepth);//BOX深度
};