#pragma once//防止重复包含

#include "core/Mesh.h"

//自定义Mesh
class CConeMesh :public CMesh   //自定义Mesh
{
	typedef CMesh Super;
public:
	virtual void Init();//继承自Mesh.h里的Rendeing.h里的的渲染初始化 

	virtual void BuildMesh(const FMeshRenderingData* InRenderIngData);//继承自Mesh.h里的用来构建Mesh接口

	virtual void Draw(float DeltaTime);//继承自Mesh.h里的在模型里使用Rendeing.h的渲染接口

	//继承自Mesh.h里的静态结构来描述顶点数据
	void CreateMesh(
		FMeshRenderingData& MeshData,//创建当前Mesh结构数据
		float InRadius,  //圆锥体底部半径  因为圆锥体顶部是单个顶点 ，所以只有底部有半径
		float InHeight,  //圆锥体高度
		uint32_t InAxialSubdivision,//轴细分
		uint32_t InHeightSubdivision); //高度细分
};