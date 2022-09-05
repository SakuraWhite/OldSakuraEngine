#include "ClientViewport.h"
#include "../../Component/TransformationComponent.h"
#include "../../Math/EngineMath.h"

GClientViewport::GClientViewport()
	: SuperV()		//初始化父类 FViewport
	, SuperA()		//初始化父类 GActorObject
	, YFOV(0.f)     //初始化fov
	, Aspect(0.f)	//初始化宽高比
	, ZNear(0.f)	//初始化近剪裁面
	, ZFar(0.f)		//初始化远剪裁面
	, bDirty(true)	//初始化标记 脏
{
}

void GClientViewport::SetFrustum(float InYFOV, float InAspect, float InZNear, float InZFar)
{
	
	YFOV = InYFOV;//采集fov
	Aspect = InAspect;//采集宽高比
	ZNear = InZNear;//采集近剪裁面
	ZFar = InZFar;//采集远剪裁面

	//投影矩阵
	//不是将3D空间给转换到2D还是转换到3D  这是基于视野构建左手透视投影矩阵
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		InYFOV, //以弧度为单位的自上而下的视场角。这里是45° FOV
		InAspect,//视图or屏幕空间 X:Y 的纵横比。
		InZNear,		//到近剪裁平面的距离 必须大于零
		InZFar		//近剪裁平面到远剪裁面的距离 。
	);

	XMStoreFloat4x4(&ProjectMatrix, Project);//物体空间的转换

	//脏标记
	SetDirty(true);
}

void GClientViewport::FaceTarget(
	const fvector_3d& InPosition,     //输入位置	
	const fvector_3d& InTargetPosition, //目标位置	
	const fvector_3d& InUP)	//给一个向上方向  默认值
{
	//面向目标朝向					 目标位置  - 原本的位置 
	fvector_3d TmpFaceVector = InTargetPosition - InPosition;
	TmpFaceVector.normalize();//单位化朝向

	//朝右方向								叉乘			默认的上方向叉乘正面朝向
	fvector_3d TmpRightVector = fvector_3d::cross_product(InUP, TmpFaceVector);
	TmpRightVector.normalize();//单位化朝向

	//朝上方向(矫正上方向 并不是默认的上方向)		叉乘       正面朝向叉乘朝右方向
	fvector_3d TmpUPVector = fvector_3d::cross_product(TmpFaceVector, TmpRightVector);
	TmpUPVector.normalize();//单位化朝向

	//转换					通过EngineMath进行转换
	XMFLOAT3 XMFaceVector = EngineMath::ToFloat3(TmpFaceVector);
	XMFLOAT3 XMRightVector = EngineMath::ToFloat3(TmpRightVector);
	XMFLOAT3 XMUPVector = EngineMath::ToFloat3(TmpUPVector);

	//赋值操作
	GetForwardVector() = XMFaceVector;
	GetRightVector() = XMRightVector;
	GetUPVector() = XMUPVector;

	//脏标记
	SetDirty(true);
}

void GClientViewport::Tick(float DeltaTime)
{
	//执行构建屏幕空间
	BuildViewMatrix(DeltaTime);
}

void GClientViewport::BuildViewMatrix(float DeltaTime)
{
	//计算世界空间转换到屏幕空间运算 矫正轴
	GetTransformationComponent()->CorrectionVector();
	//计算按自身方向移动意图
	fvector_3d V3;	// fvector_3d是封装好的数学库里的计算
	//获得从世界空间转换到屏幕空间后的矫正位置
	GetTransformationComponent()->GetCorrectionPosition(V3);

	//构建屏幕空间VewMatrix
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();//提取右朝向方向
	XMFLOAT3 UPVector = GetTransformationComponent()->GetUPVector();//提取上朝向方向
	XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();//提取正朝向方向

	//这里矩阵是列式  屏幕空间矩阵
	ViewMatrix = {
		RightVector.x,	UPVector.x,	ForwardVector.x,	0.f,
		RightVector.y,	UPVector.y,	ForwardVector.y,	0.f,
		RightVector.z,	UPVector.z,	ForwardVector.z,	0.f,
		V3.x,			V3.y,		V3.z,				1.f };
}
