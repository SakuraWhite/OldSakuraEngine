#include "CubeMapRenderTarget.h"
#include "../../../../../Debug/EngineDebug.h"

FCubeMapRenderTarget::FCubeMapRenderTarget()
	: Width(256)   //初始化渲染目标视口的宽
	, Height(256) //初始化渲染目标视口的高
	, Format(DXGI_FORMAT_R8G8B8A8_UNORM)//初始化存储渲染目标视口的贴图格式
{
	//重置渲染目标 视口相关信息
	ResetViewport(Width, Height);
	//重置视口裁剪相关信息
	ResetScissorRect(Width, Height);

	//分配6个CPU渲染目标视口  用作储存6个摄像机的视口
	CPURenderTargetView.resize(6);

}

void FCubeMapRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
{
	//初始化的时候进行：

	Width = InWidth;  //渲染目标视口的宽赋值
	Height = InHeight; //渲染目标视口的高赋值
	Format = InFormat; //渲染目标视口的贴图格式赋值

	//重置视口信息 裁剪信息
	ResetViewport(Width, Height);
	ResetScissorRect(Width, Height);

	//初始化的时候 进行构建渲染目标贴图
	BuildRenderTargetMap();

	//进行构建shader资源视图 描述符
	BuildSRVDescriptors();
	//进行构建渲染目标视图 描述符
	BuildRTVDescriptors();
}

void FCubeMapRenderTarget::ResetViewport(UINT InWidth, UINT InHeight)
{
	Viewport =
	{
		0.0f,//TopLeftX 左上角X位置
		0.0f,//TopLeftY 左上角Y位置
		(float)InWidth,//Width 宽
		(float)InHeight,//Height 高
		0.0f,//MinDepth  最小深度
		1.0f//MaxDepth  最大深度
	};
}

void FCubeMapRenderTarget::ResetScissorRect(UINT InWidth, UINT InHeight)
{
	ScissorRect =
	{
		0,//left 左起始点
		0,//top  顶部起始点
		(LONG)InWidth,//right  宽度
		(LONG)InHeight//bottom 高度
	};
}

void FCubeMapRenderTarget::ResetRenderTarget(UINT InWidth, UINT InHeight)
{
	if (InWidth != Width || InHeight != Height) //判断输入的宽高与设定的宽高是否一致
	{
		Width = InWidth;		//重新赋值
		Height = InHeight;		//重新赋值

		
		BuildRenderTargetMap();	//重新构建渲染目标贴图

		BuildSRVDescriptors();	//重新构建CPU着色器资源视图
		BuildRTVDescriptors();	//重新构建CPU渲染目标视图
	}
}

void FCubeMapRenderTarget::BuildRenderTargetMap()
{
	//创建缓冲资源描述
	D3D12_RESOURCE_DESC ResourceDesc;
	memset(&ResourceDesc, 0, sizeof(ResourceDesc));//将资源描述中的东西给清掉

	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; //指定当前资源的格式 当前资源是什么样子的资源 这里是2D贴图
	ResourceDesc.Alignment = 0;    //对齐方式
	ResourceDesc.Width = Width;    //指定资源的宽
	ResourceDesc.Height = Height;  //指定资源的高
	ResourceDesc.DepthOrArraySize = 6; //资源深度数组的大小 如果是3D资源则是深度
	ResourceDesc.MipLevels = 1;		//Mipmap层级
	ResourceDesc.Format = Format;	//指定贴图资源格式
	ResourceDesc.SampleDesc.Count = 1;	//采样描述数量
	ResourceDesc.SampleDesc.Quality = 0;	//采样描述质量
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; //资源布局方式 这里指定的是未知模式 根据传入的资源属性自动分配
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; //标志 资源是否可以被允许设置为渲染目标和被转换类型

	//创建默认堆
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(  //使用D3D驱动指针创建一个Resource资源
		&BufferProperties,   //创建默认的一个性能 默认堆
		D3D12_HEAP_FLAG_NONE, //堆标签  这里是无
		&ResourceDesc,       //缓冲资源描述
		D3D12_RESOURCE_STATE_COMMON,//资源的初始状态 这里是COMMON
		NULL,	 //
		IID_PPV_ARGS(RenderTargetMap.GetAddressOf())));//资源指针

}

void FCubeMapRenderTarget::BuildSRVDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};//着色器资源视图描述

	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//默认着色器组件映射  采样图片RGB像素时可以交换分量 比如RG交换 或者GB交换等
	SRVDesc.Format = Format;//描述当前shader视图格式信息
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;//描述加载资源的维度 这里是立方体贴图纹理格式

	SRVDesc.TextureCube.MostDetailedMip = 0; //详细信息
	SRVDesc.TextureCube.MipLevels = 1;   //mipmap层级
	SRVDesc.TextureCube.ResourceMinLODClamp = 0.f; //当前的资源最小LOD钳制


	GetD3dDevice()->CreateShaderResourceView(  //通过D3D驱动来创建着色器资源视图
		RenderTargetMap.Get(), //输入渲染目标生成的MAP
		&SRVDesc,			//输入着色器资源视图描述
		CPUShaderResourceView);//输入CPU着色器资源视图
}

void FCubeMapRenderTarget::BuildRTVDescriptors()
{
	for (size_t i = 0; i < 6; i++)  //遍历 因为我们要渲染模型6个方向的环境贴图
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = { };//渲染目标视图描述

		RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;//描述加载资源的维度 这里是2D贴图纹理格式
		RTVDesc.Format = Format;	//描述当前shader视图格式信息
		RTVDesc.Texture2DArray.MipSlice = 0;	//Mipmap切片
		RTVDesc.Texture2DArray.PlaneSlice = 0;	//平面切片
		//MipSlice + ArraySlice * MipLevels  资源索引算法(切片) MipSlice PlaneSlice
		RTVDesc.Texture2DArray.FirstArraySlice = i;		//数组切片
		RTVDesc.Texture2DArray.ArraySize = 1;	//数组大小  代表每个RTV里只有一个视图

		GetD3dDevice()->CreateRenderTargetView(   //通过D3D驱动来创建渲染目标视图
			RenderTargetMap.Get(),    //输入渲染目标生成的MAP
			&RTVDesc,			//输入渲染目标视图描述
			CPURenderTargetView[i]); //输入CPU渲染目标视图
	}

}
