//控制玩家视口或者相机  传入指令使用的
#pragma once//防止重复包含
#include "core/Component.h"

class CTransformationComponent :public CComponent
{

public:
	CTransformationComponent();
	
	//设置物体位置
	void SetPosition(const XMFLOAT3& InNewPosition);
	//设置物体旋转
	void SetRotation(const fvector_3d& InNewRotation);
	//设置物体缩放
	void SetScale(const fvector_3d& InNewScale);

	//设置物体朝向向量
	void SetForwardVector(const XMFLOAT3& InForwardVector);
	void SetRightVector(const XMFLOAT3& InRightVevtor);
	void SetUPVector(const XMFLOAT3& InUPVevtor);

public:
	//获得物体位置
	XMFLOAT3 &GetPosition()  { return Position; }
	//获得物体旋转
	fvector_3d GetRotation() const { return fvector_3d(Rotation.x, Rotation.y, Rotation.z); }
	//获得物体缩放
	fvector_3d GetScale() const { return fvector_3d(Scale.x, Scale.y, Scale.z); }

	//获得物体朝向向量
	XMFLOAT3 &GetForwardVector()  { return ForwardVector; }
	XMFLOAT3 &GetRightVector()  { return RightVector; }
	XMFLOAT3 &GetUPVector()  { return UPVector; }

public:
	//世界空间转换到屏幕空间的矫正运算之后 之后会返回一个POS位置  fvector_3d是封装好的数学库里的计算
	void CorrectionVector();

	//获取世界空间转到屏幕空间后的位置 两种方式
	fvector_3d GetCorrectionPosition();//通过这种方式返回
	void GetCorrectionPosition(fvector_3d& InPos3);//这种方式可以直接取值
private:
	//物体的位置 P
	XMFLOAT3 Position;
	//物体旋转 R
	XMFLOAT3 Rotation;
	//物体缩放
	XMFLOAT3 Scale;

	//物体正面朝向方向 F  在左手坐标系中Z轴是正方向 所以是0.f, 0.f, 1.f
	XMFLOAT3 ForwardVector;
	//物体右面朝向方向 R  X轴是右方向 所以是1.f, 0.f, 0.f
	XMFLOAT3 RightVector;
	//物体朝上的方向 UP   Y轴朝上所以是0.f, 1.f, 0.f
	XMFLOAT3 UPVector;


};