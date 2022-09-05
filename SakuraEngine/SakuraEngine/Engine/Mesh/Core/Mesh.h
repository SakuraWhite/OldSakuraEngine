#pragma once//防止重复包含
#include "../../Rendering/Core/Rendering.h"
#include "../../Actor/Core/ActorObject.h"
#include "MeshType.h"
#include "../../Interface/DirectXDeviceInterfece.h"

class CTransformationComponent;
class CMeshComponent;
class CMaterial;
enum EMeshRenderLayerType;

class GMesh
	:public GActorObject,    //继承来自GActorObject 
	public IRenderingInterface,//这里是接口
	public IDirectXDeviceInterfece
{
	typedef GActorObject Super;

	CVARIABLE()  //反射宏
		CTransformationComponent* TransformationComponent;//我们希望模型物体能够移动 就需要控制组件

	CVARIABLE()
		CMeshComponent* MeshComponent;

public:
	GMesh();//初始化Index与Vertex的一些缓冲区数据

	virtual void Init(); //Rendeing.h里的的渲染初始化

	virtual void PreDraw(float DeltaTime); //在模型里使用Rendeing.h的预渲染接口 
	virtual void Draw(float DeltaTime);  //在模型里使用Rendeing.h的渲染接口  进行时
	virtual void PostDraw(float DeltaTime);//在模型里使用Rendeing.h的交付渲染接口 poatDraw

	//static CMeshComponent*CreateMesh(const FMeshRenderingData *InRenderingData); //静态结构来描述顶点数据 静态方式来创建Mesh

public:
	virtual void SetPosition(const XMFLOAT3& InNewPosition);  //设置位置
	virtual void SetRotation(const fvector_3d& InRotation);   //设置旋转	
	virtual void SetScale(const fvector_3d& InNewScale);      //设置缩放
public:
	virtual CMeshComponent* GetMeshComponent() { return MeshComponent; }//接口 获取组件

	//使用模板的方式获得组件
	template<class T>
	T* GetMeshComponent()
	{
		return dynamic_cast<T*>(MeshComponent);//转换到指定类型
	}
public:
	//设置模型渲染层级类型
	virtual void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);

protected:
	//设置组件
	virtual void SetMeshComponent(CMeshComponent* InMeshComponent); 

public:
	//获取材质的数量 
	UINT GetMaterialNum()const;
	//获取材质
	vector<CMaterial*>* GetMaterials();


};