#pragma once//防止重复包含

enum EMaterialType //枚举材质类型
{
	//局部光照模型
	Lambert = 0,		 //兰伯特		纸张 粗糙表面
	HalfLambert,		 //半兰伯特	粗糙表面 也可以做皮肤
	Phong,				 //冯			金属高光
	BinnPhong,			 //布林冯		金属高光
	Wrap,				 //WrapLight 早期用来模拟皮肤的材质
	Minnaert,			 //Minnaert Lighting   用来模拟布料 丝袜 纤维等
	Banded,				 //基础卡通效果
	GradualBanded,		 //具有渐变效果的卡通材质
	FinalBanded,		 //自定义的卡通材质 融合了很多效果的卡通材质
	BackLight,			 //透射  次表面散射材质
	AnisotropyKajiyaKay, //GDC 各向异性 2004年KajiyaKay 头发渲染模型
	OrenNayar,			 //GDC 粗糙表面材质  如沙漠

	//材质效果类型  
	BaseColor,			 //基础颜色模型 只有颜色 不受光照
	Normal,				 //显示法线
	WorldNormal,		 //显示世界法线	WorldspaceNormal;

	Transparency = 15,		//透明

	Fresnel = 100,		 //菲涅尔  单独的效果  不能算是材质
	Max,				 //最大

};


enum EMaterialDisplayStatusType //枚举材质状态显示  线框/材质切换
{
	DefaultDisplay = 0, //默认显示(按面显示）
	PointDisplay = 1,//显示顶点
	WireframeDisplay = 2,//线框显示
	TriangleDisplay = 4 ,//三角形显示
};