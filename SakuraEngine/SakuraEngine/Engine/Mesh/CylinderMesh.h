#pragma once//防止重复包含

#include "core/Mesh.h"

class CCylinderMesh :public CMesh   //生成一个标准化的球体 继承自FMesh
{
	typedef CMesh Super;
public:
	virtual void Init();//继承自Mesh.h里的Rendeing.h里的的渲染初始化 

	virtual void BuildMesh(const FMeshRenderingData* InRenderIngData);//继承自Mesh.h里的用来构建Mesh接口

	virtual void Draw(float DeltaTime);//继承自Mesh.h里的在模型里使用Rendeing.h的渲染接口

	//继承自Mesh.h里的静态结构来描述顶点数据 静态方式来创建CCylinderMesh
	void CreateMesh(
		FMeshRenderingData& MeshData,//创建当前Mesh结构数据
		float InTopRadius, //圆柱顶部半径
		float InBottomRadius, //圆柱底部半径
		float InHeight,//圆柱体的高度
		uint32_t InAxialSubdivision,//球体的轴向细分
		uint32_t InHeightSubdivision); //球体的高度细分
};