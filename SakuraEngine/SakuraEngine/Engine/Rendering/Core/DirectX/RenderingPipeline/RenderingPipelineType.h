#pragma once//防止重复包含


//渲染管线状态描述

//枚举不同的管线状态  线框 渲染等状态
enum EPipelineState
{
	Transparent = 0,//指定为透明模式  序号为0

	AlphaTest,//Alpha测试模式 

	Background,//指定为背景模式

	
	Reflector,//指定为反射模式

	//状态更新 指定实体渲染模式 初始状态为灰色标准显示   
	GrayModel = 4, //指定实体渲染模式模型序号为4

	//线框模式更新
	Wireframe = 5,//线框渲染模式序号为5




};