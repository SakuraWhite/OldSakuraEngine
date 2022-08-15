#pragma once//防止重复包含

#include "core/Mesh.h"

class GPlaneMesh :public GMesh   //生成一个标准化的BOX 继承自FMesh
{
	typedef GMesh Super;
public:
	GPlaneMesh();

	virtual void Init();//继承自Mesh.h里的Rendeing.h里的的渲染初始化 

	virtual void Draw(float DeltaTime);//继承自Mesh.h里的在模型里使用Rendeing.h的渲染接口

	void CreateMesh(
		float InHeight,   //平面高度
		float InWidth, //平面宽度
		uint32_t InHeightSubdivide,//平面的高度细分
		uint32_t InWidthSubdivide);//平面宽度细分

};