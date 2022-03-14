#pragma once//防止重复包含

#include "core/Mesh.h"

//自定义Mesh
class CCustomMesh :public CMesh   //自定义Mesh
{
	typedef CMesh Super;
public:
	virtual void Init();//继承自Mesh.h里的Rendeing.h里的的渲染初始化 

	virtual void BuildMesh(const FMeshRenderingData* InRenderIngData);//继承自Mesh.h里的用来构建Mesh接口

	virtual void Draw(float DeltaTime);//继承自Mesh.h里的在模型里使用Rendeing.h的渲染接口

	//继承自Mesh.h里的静态结构来描述顶点数据 静态方式来创建自定义Mesh
	void CreateMesh(FMeshRenderingData& MeshData, string& InPath); //用来读取FBX

	//用来判断读取是否为OBJ
	static bool LoadObjFormBuff(char *InBuff, uint32_t InBuffSize, FMeshRenderingData &MeshData);
};