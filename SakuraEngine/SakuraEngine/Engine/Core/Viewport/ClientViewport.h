#pragma once//防止重复包含
#include "../../Core/Engine.h"
#include "Viewport.h"
#include "../../Actor/Core/ActorObject.h"



class GClientViewport :
	public GActorObject//继承自GActorObject
	, public FViewport//继承自Fviewport
{

	typedef FViewport SuperV;    //定义父类
	typedef GActorObject SuperA; //定义父类

public:
	GClientViewport();

	//设置摄像机视锥体	  输入FOV		宽高比			近剪裁面		远剪裁面
	void SetFrustum(float InYFOV, float InAspect, float InZNear, float InZFar);

	//是否开启第一人称视口			输入位置							目标位置						给一个向上方向  默认值
	void FaceTarget(const fvector_3d& InPosition, const fvector_3d& InTargetPosition, const fvector_3d& InUP = fvector_3d(0.f, 1.f, 0.f));

	//帧执行
	virtual void Tick(float DeltaTime);
	//构建视口矩阵
	virtual void BuildViewMatrix(float DeltaTime);
public:
	FORCEINLINE float GetFOV() const { return YFOV; } //获取FOV
	FORCEINLINE float GetAspect() const { return Aspect; }//获取宽高比
	FORCEINLINE float GetNear() const { return ZNear; }//获取近剪裁面	
	FORCEINLINE float GetFar() const { return ZFar; }//获取远剪裁面
	FORCEINLINE bool GetDirty() const { return bDirty; }//获取 标记 脏

	//设置标记 脏
	void SetDirty(bool bNewDirty) { bDirty = bNewDirty; }
private:
	//保存摄像机参数
	float YFOV;   //FOV
	float Aspect; //宽高比
	float ZNear;  //近剪裁面
	float ZFar;   //远剪裁面


	bool bDirty;//标记 脏
}; 