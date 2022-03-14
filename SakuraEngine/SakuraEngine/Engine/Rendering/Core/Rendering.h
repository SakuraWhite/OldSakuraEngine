#pragma once//防止重复包含
#include "../../Core/Engine.h"
#include "../../Core/CoreObject/GuidInterface.h"
#if defined(_WIN32)   //根据引擎来采用头文件
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif


//记录提供渲染相关的内容的接口
class  IRenderingInterface   //:public IGuidInterface //继承GUID
{
	friend class CDirectXRenderingEngine;//让Engine可以直接访问
public:
	IRenderingInterface();  //构造
	virtual ~IRenderingInterface();	//析构函数

	virtual void Init(); //渲染初始化

	virtual void PreDraw(float DeltaTime); //预渲染
	virtual void Draw(float DeltaTime); //渲染相关 进行时
	virtual void PostDraw(float DeltaTime);//交付渲染 poatDraw

protected:
	//创建默认缓冲区
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(
		ComPtr<ID3D12Resource>& OutTmpBuffer, //我们要输出的上传缓冲区
		const void*InData,	 //当前的数据
		UINT64 InDataSize);  //当前数据的大小
protected:
	ComPtr<ID3D12Device>  GetD3dDevice(); //拿到在WindowsEngine.h里的D3D驱动
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();//拿到在WindowsEngine.h里用于同步CPU和一个或多个GPU的对象
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();//接口
#if defined(_WIN32)	 //做运行环境的区分
	CWindowsEngine* GetEngine();
#else
	CEngine* GetEngine();  //如果是win32就走默认的
#endif
		
	/*
private:
	static vector<IRenderingInterface*> RenderingInterface;//静态函数
	*/
};
