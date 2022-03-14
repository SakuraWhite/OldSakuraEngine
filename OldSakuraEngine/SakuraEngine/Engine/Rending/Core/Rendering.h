#pragma once//防止重复包含
#include "../../Core/Engine.h"
#include "../../Core/CoreObject/GuidInterface.h"
#if defined(_WIN32)   //根据引擎来采用头文件
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif


//记录提供渲染相关的内容的接口
class  IRenderingInterface :public IGuidInterface //继承GUID
{
	friend class CWindowsEngine;//让Engine可以直接访问
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
		
private:
	static vector<IRenderingInterface*> RenderingInterface;//静态函数
	
};

//创建常量缓冲区结构
class FRenderingResourcesUpdate :public enable_shared_from_this<FRenderingResourcesUpdate>//后面这段是共享指针 可以在对象里保存一个弱指针
{
public:
	FRenderingResourcesUpdate();//构造函数
	~FRenderingResourcesUpdate();//析构函数

	void Init(ID3D12Device* InDevice, UINT InElemetSize, UINT InElemetCount);//初始化资源 第一个是驱动，第二个是元素的大小,第三个是元素的数量

	void Update(int Index, const void *InData);//更新数据
	
	//快速获取我们当前常量Buffer字节Size   两种方法
	UINT GetConstantBufferByteSize(UINT InTypeSzie);  //元素类型大小 
	UINT GetConstantBufferByteSize();  //元素类型大小 直接可以获取

	ID3D12Resource* GetBuffer() { return UploadBuffer.Get(); } //获取上传的Budder 创建的上传Buffer需要暴漏出来
private:
	ComPtr<ID3D12Resource> UploadBuffer;//上传Buffer  
	UINT ElementSize;//单个元素大小
	BYTE* Data;//字节  实际数据
};