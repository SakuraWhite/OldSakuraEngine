#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterfece.h"
#include "../../Core/Viewport/ViewportInfo.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"


class FRenderingResourcesUpdate;
class CMeshComponent;
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
	CMeshComponent* CreateBoxMeshComponent(
		float InHeight,			//BOX高度
		float InWidth,			//BOX宽度
		float InDepth);			//BOX深度

	CMeshComponent* CreateConeMeshComponent(
		float InRadius,						//圆锥体底部半径  因为圆锥体顶部是单个顶点 ，所以只有底部有半径
		float InHeight,						//圆锥体高度
		uint32_t InAxialSubdivision,		//轴细分
		uint32_t InHeightSubdivision);		//高度细分

	CMeshComponent* CreateCylinderMeshComponent(
		float InTopRadius,					//圆柱顶部半径
		float InBottomRadius,				//圆柱底部半径
		float InHeight,						//圆柱体的高度
		uint32_t InAxialSubdivision,		//球体的轴向细分
		uint32_t InHeightSubdivision);		//球体的高度细分

	CMeshComponent* CreatePlaneMeshComponent(
		float InHeight,						//平面高度
		float InWidth,						//平面宽度
		uint32_t InHeightSubdivide,			//平面的高度细分
		uint32_t InWidthSubdivide);			//平面宽度细分

	CMeshComponent* CreateSphereMeshComponent(
		float InRadius,						//球体的是半径
		uint32_t InAxialSubdivision,		//球体的轴向细分
		uint32_t InHeightSubdivision,		//球体的高度细分
		bool bReverse);		

	CMeshComponent* CreatePyramidMeshComponent(
		EPyramidNumberSides InPyramidNumberSidesType,//棱锥体边数
		uint32_t InHeightSubdivide,//棱锥体高度细分
		uint32_t InSize);//输入序号

	CMeshComponent* CreatePipeMeshComponent(
		float InTopRadius,   //输入圆柱体顶部半径
		float InBottomRadius,//输入圆柱体底部半径
		float InHeight,//输入圆柱体高度
		float InThickness,//输入圆柱体厚度
		uint32_t InAxialSubdivision,  //轴细分
		uint32_t InHeightSubdivision);//轴高度细分

	CMeshComponent* CreateTorusMeshComponent(
		float InRadius,	//圆环半径
		float InSectionRadius, //圆环截面半径
		uint32_t InAxialSubdivision,//圆环轴向细分
		uint32_t InHeightSubdivision);//圆环高度细分

	CMeshComponent* CreateMeshComponent(string& InPath);

	//获取渲染管线
	FRenderingPipeline& GetRenderingPipeline() { return RenderingPipeline; }


protected:
	//渲染管线
	FRenderingPipeline RenderingPipeline;

};
