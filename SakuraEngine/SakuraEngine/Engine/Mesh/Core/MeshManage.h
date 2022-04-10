#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterfece.h"
#include "../../Core/Viewport/ViewportInfo.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"


class FRenderingResourcesUpdate;

class CMeshManage 
	:public CCoreMinimalObject, 
	public IRenderingInterface,
	public IDirectXDeviceInterfece
{
public:
	CMeshManage();//初始化Index与Vertex的一些缓冲区数据

	virtual void Init(); //Rendeing.h里的的渲染初始化

	virtual void BuildMesh();//用来构建Mesh接口
	//更新MeshManage的数学的方法
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);



	virtual void PreDraw(float DeltaTime);//在模型里使用Rendeing.h的预渲染接口 
	virtual void Draw(float DeltaTime);//在模型里使用Rendeing.h的渲染接口  进行时
	virtual void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw


public:
	GMesh* CreateBoxMesh(
		float InHeight,			//BOX高度
		float InWidth,			//BOX宽度
		float InDepth);			//BOX深度

	GMesh* CreateConeMesh(
		float InRadius,						//圆锥体底部半径  因为圆锥体顶部是单个顶点 ，所以只有底部有半径
		float InHeight,						//圆锥体高度
		uint32_t InAxialSubdivision,		//轴细分
		uint32_t InHeightSubdivision);		//高度细分

	GMesh* CreateCylinderMesh(
		float InTopRadius,					//圆柱顶部半径
		float InBottomRadius,				//圆柱底部半径
		float InHeight,						//圆柱体的高度
		uint32_t InAxialSubdivision,		//球体的轴向细分
		uint32_t InHeightSubdivision);		//球体的高度细分

	GMesh* CreatePlaneMesh(
		float InHeight,						//平面高度
		float InWidth,						//平面宽度
		uint32_t InHeightSubdivide,			//平面的高度细分
		uint32_t InWidthSubdivide);			//平面宽度细分

	GMesh* CreateSphereMesh(
		float InRadius,						//球体的是半径
		uint32_t InAxialSubdivision,		//球体的轴向细分
		uint32_t InHeightSubdivision);		//球体的高度细分

	GMesh* CreateMesh(string& InPath);

protected:
	//制作模板  作用是可以往里面传任意参数  根据传入参数与指定的类型然后生成
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);

protected:

	FRenderingPipeline RenderingPipeline;

};
