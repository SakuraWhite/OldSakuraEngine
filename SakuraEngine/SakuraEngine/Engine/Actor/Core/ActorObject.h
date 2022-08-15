#pragma  once 
#include "../../EngineMinimal.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"

class CTransformationComponent;

// G 是 game的意思  gameplay  继承自CCoreMinimalObject核心对象
class GActorObject : public CCoreMinimalObject
{

	CVARIABLE()  //反射宏
	CTransformationComponent* TransformationComponent;//我们希望摄像机能够移动 就需要控制组件

public:
	GActorObject();

public:
	FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }
public:
	//设置物体的位置数据
	void SetPosition(const XMFLOAT3& InNewPosition);
	//设置物体的旋转数据  欧拉角
	void SetRotation(const fvector_3d& InRotation);
	//设置物体的缩放数据
	void SetScale(const fvector_3d& InNewScale);
public:
	//获取物体位置信息
	XMFLOAT3& GetPosition();
	//获取物体旋转信息
	fvector_3d GetRotation()const;
	//获取物体缩放信息
	fvector_3d GetScale()const;


	//获得物体朝向向量  物体有自己的朝向 不需要修改里面的东西
	XMFLOAT3& GetForwardVector();
	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUPVector();
};