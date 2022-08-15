#include "Viewport.h"
#include "../../Mesh/Core/ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Math/EngineMath.h"

FViewport::FViewport() 
	:ViewMatrix(EngineMath::IdentityMatrix4x4())//初始化摄像机位置 屏幕空间坐标单位化
	,ProjectMatrix(EngineMath::IdentityMatrix4x4())//初始化可视范围 物体空间坐标单位化
{

}

void FViewport::ViewportInit()
{
	//当前屏幕的横纵比 一般是16比9
	//这里是定义引擎视口的屏幕的宽除以屏幕视口的高 同时记得从INT类型转换为Float类型
	float AspectRatio = (float)FEngineRenderConfig::GetRenderConfig()->ScrrenWidth / (float)FEngineRenderConfig::GetRenderConfig()->ScrrenHight;
	//(1,1,0) (-1,1,0) (-1,-1,0) (1,-1,0) (1,1,1) (-1,1,1) (-1,-1,1) (1,-1,1)

	//投影矩阵
	//不是将3D空间给转换到2D还是转换到3D  这是基于视野构建左手透视投影矩阵
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI, //以弧度为单位的自上而下的视场角。这里是45°
		AspectRatio,//视图or屏幕空间 X:Y 的纵横比。
		1.0f,		//到近剪裁平面的距离 必须大于零
		10000.f		//近剪裁平面到远剪裁面的距离 。
	);

	XMStoreFloat4x4(&ProjectMatrix, Project);//物体空间的转换
}
