#include "DynamicCubeMap.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../../.././../Core/Viewport/ClientViewport.h"
#include "../../RenderLayer/RenderLayerManage.h"
#include "../../../../../../Config/EngineRenderConfig.h"
#include "../../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"

FDynamicCubeMap::FDynamicCubeMap()
	:GeometryMap(NULL)//初始化几何map  为空
	, DirectXPipelineState(NULL)//初始化管线对象状态 为空
	, RenderLayer(NULL)//初始化渲染层级 为空
	, Width(256) //初始化宽
	, Height(256) //初始化高
{
	//渲染目标实例  初始化为Cubemap渲染目标层
	RenderTarget = std::make_unique<FCubeMapRenderTarget>();
}

void FDynamicCubeMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	if (CubeMapViewport.size() == 6) //判断Cubemap视口里是否有值
	{
		for (int i = 0; i < GeometryMap->DynamicReflectionMeshComponents.size(); i++) //便利获取到几何Map里的动态反射模型组件
		{
			CMeshComponent* Tmp = GeometryMap->DynamicReflectionMeshComponents[i];//获取动态反射模型组件
			XMFLOAT3 F3 = Tmp->GetPosition(); //获取位置
			SetCubeMapViewportPosition(fvector_3d(F3.x, F3.y, F3.z));//设置立方体贴图视口位置

			for (size_t j = 0; j < 6; j++) //遍历6个
			{
				FViewportInfo MyViewportInfo; //视口信息

				XMFLOAT3 ViewPosition = CubeMapViewport[j]->GetPosition(); //视口位置
				MyViewportInfo.ViewPosition = XMFLOAT4(ViewPosition.x, ViewPosition.y, ViewPosition.z, 1.f);//构建视口位置
				MyViewportInfo.ViewMatrix = CubeMapViewport[j]->ViewMatrix; //构建视口矩阵
				MyViewportInfo.ProjectMatrix = CubeMapViewport[j]->ProjectMatrix;//构建项目矩阵

				//通过几何MAP去更新计算视口							视口信息			
				GeometryMap->UpdateCalculationsViewport(DeltaTime, MyViewportInfo,
					j + i * 6 	//给动态摄像机
					+ 1);	//给主相机 视口偏移
			}
		}
	}
}

void FDynamicCubeMap::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer)
{
	//直接进行赋值
	GeometryMap = InGeometryMap;//几何map
	DirectXPipelineState = InDirectXPipelineState;//管线对象状态
	RenderLayer = InRenderLayer;//渲染层级

}

void FDynamicCubeMap::PreDraw(float DeltaTime)
{

	for (int j = 0; j < GeometryMap->DynamicReflectionMeshComponents.size(); j++) //根据需要反射的模型数量来进行动态反射模型组件
	{

		//指向哪个资源 转换其状态
		//D3D12_RESOURCE_STATE_RENDER_TARGET：我们要渲染的目标里面写入的状态
		CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
			RenderTarget->GetRenderTarget(), //输入渲染目标
			D3D12_RESOURCE_STATE_GENERIC_READ, //这里是可读
			D3D12_RESOURCE_STATE_RENDER_TARGET);//转换为渲染目标

		//通过图形命令列表去设置资源屏障
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresent);

		//需要每帧执行
		//绑定矩形框
		auto RenderTargetViewport = RenderTarget->GetViewport(); //渲染目标视口信息
		auto RenderTargetScissorRect = RenderTarget->GetScissorRect(); //渲染目标FOV矩形视口信息
		GetGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewport);//1代表着绑定的数量，之后是绑定视口信息
		GetGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);//1代表绑定数量，之后是绑定视口矩形大小

		//获取CBV的常量缓冲区的size 作为当前的偏移
		UINT CBVSize = GeometryMap->ViewportConstantBufferViews.GetConstantBufferByteSize();

		for (size_t i = 0; i < 6; i++) //我们需要清除6个画布，所以这里进行遍历
		{

			//清除画布
			GetGraphicsCommandList()->ClearRenderTargetView(//我们可以使用ClearRenderTargetView来吧画布清楚为各种颜色，比如红色白色蓝色等等
				RenderTarget->CPURenderTargetView[i],//获取CPU渲染目标视图
				DirectX::Colors::Black,		//把画布给清除为各种颜色的API
				0, nullptr);//0与nullptr是与视口相关

			//清除深度模板缓冲区
			//将当前的一个或多个的渲染目标与深度模板缓冲区绑定在输出的合并阶段
			GetGraphicsCommandList()->ClearDepthStencilView(
				DSVDes, //获取深度模板视图描述
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,//清除深度与模板 位运算
				1.f,//代表着我们用1来清除深度缓冲区
				0, //代表着我们用0来清除模板缓冲区
				0, NULL);//最后两个参数保持与DX12的API一致即可

			//指定渲染缓冲区 输出合并阶段
			GetGraphicsCommandList()->OMSetRenderTargets(
				1,//指定绑定的数量
				&RenderTarget->CPURenderTargetView[i], //指定渲染目标  CPU渲染目标视图
				true,//代表传入的句柄指向一个连续的描述符指针
				&DSVDes);//指定深度目标  深度模板视图描述


			//更新6个摄像机 绑定6个摄像机
			auto ViewprotAddr = GeometryMap->ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();//获取视口常量缓冲区的GPU首地址
			ViewprotAddr += (
				1 +	//主摄像机
				i + j * 6 //i是渲染目标的Cubemap摄像机 J是遍历到动态反射模型组件 然后做偏移
				) * CBVSize; //首地址+1之后再加上I再乘以偏移值 进行偏移
			//将图形根设置为常量缓冲区视图								根签名	偏移值
			GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewprotAddr);


			//各类层级渲染
			RenderLayer->Draw(RENDERLAYER_BACKGROUND, DeltaTime);//渲染背景层
			RenderLayer->Draw(RENDERLAYER_OPAQUE, DeltaTime); //渲染不透明层
			RenderLayer->Draw(RENDERLAYER_TRANSPARENT, DeltaTime);//渲染透明层
		}



		//设置新的状态准备渲染
		CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
			RenderTarget->GetRenderTarget(),//输入渲染目标
			D3D12_RESOURCE_STATE_RENDER_TARGET,//D3D12资源状态呈现目标
			D3D12_RESOURCE_STATE_GENERIC_READ);//D3D12资源状态通用读取
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

		//开始设置主视口渲染目标
		StartSetMainViewportRenderTarget();

		//渲染主视口
		GeometryMap->DrawViewport(DeltaTime);
		//调用渲染
		Draw(DeltaTime);

		RenderLayer->FindObjectDraw(  //调用渲染层级层级 来进行指定对象渲染
			DeltaTime,
			RENDERLAYER_OPAQUE_REFLECTOR,//指定渲染层 
			GeometryMap->DynamicReflectionMeshComponents[j]);//指定动态反射模型组件

		//重置CubeMap
		GeometryMap->DrawCubeMapTexture(DeltaTime);

		//End 结束设置主视口渲染目标
		EndSetMainViewportRenderTarget();

	}
}

void FDynamicCubeMap::Draw(float DeltaTime)
{
	//CubeMap  更新当前的CubeMap  绑定在渲染管线上		CubeMap在跟签名的第6个
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, RenderTarget->GPUShaderResourceView);
}

void FDynamicCubeMap::SetCubeMapViewportPosition(const fvector_3d& InCenterPoint)
{
	//捕获摄像机四个面
	FTmpViewportCapture Capture(InCenterPoint);

	for (size_t i = 0; i < 6; i++) //遍历摄像机
	{
		CubeMapViewport[i]->SetPosition(XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z)); //对CubeMap视口进行设置位置
		CubeMapViewport[i]->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.UP[i]);//对CubeMap视口进行指向目标
		CubeMapViewport[i]->BuildViewMatrix(0.3f);//对CubeMap视口进行构建视图矩阵
	}
}

bool FDynamicCubeMap::IsExitDynamicReflectionMesh()
{
	//判断动态反射视图数量是否大于零
	return GeometryMap->GetDynamicReflectionViewportNum() > 0;
}

void FDynamicCubeMap::BuildViewport(const fvector_3d& InCenterPoint)
{
	//捕获摄像机四个面 视口捕获结构体
	FTmpViewportCapture Capture(InCenterPoint);

	for (size_t i = 0; i < 6; i++)//我们需要6个面 所以就构建6个摄像机
	{
		//通过push_back来创建客户端视口
		CubeMapViewport.push_back(CreateObject<GClientViewport>(new GClientViewport()));
		//客户端视口引用					立方体贴图视口size-1返回的是一个引用
		GClientViewport* Viewport = CubeMapViewport[CubeMapViewport.size() - 1];

		Viewport->SetPosition(XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z));//输入坐标原点
		Viewport->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.UP[i]);//看向目标  输入看向目标的位置 输入捕获目标 输入UP点
		Viewport->SetFrustum(0.5f * XM_PI, 1.f, 0.1f, 10000.f);//构建视锥  输入FOV这里是90°(Π/2) 宽高比(1:1正方形)  近剪裁面0.1  远剪裁面1W    
		Viewport->BuildViewMatrix(0.3f);//构建视口矩阵
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
	//深度与模板缓冲
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = Width;   //宽	
	ResourceDesc.Height = Height;	//高
	ResourceDesc.Alignment = 0; //对齐
	ResourceDesc.MipLevels = 1; //mipmap等级
	ResourceDesc.DepthOrArraySize = 1;//深度或数组大小
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//维度  这里是2D贴图

	ResourceDesc.SampleDesc.Count = 1; //样本描述数量
	ResourceDesc.SampleDesc.Quality =  0;//样本质量
	ResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //格式
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//布局

	D3D12_CLEAR_VALUE ClearValue;   //清除值
	ClearValue.DepthStencil.Depth = 1.f;//深度
	ClearValue.DepthStencil.Stencil = 0;//模板
	ClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//格式

	//CD3DX12堆属性
	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	GetD3dDevice()->CreateCommittedResource(  //通过D3D来创建提交的资源
		&Properties,	//属性
		D3D12_HEAP_FLAG_NONE, &ResourceDesc, //属性描述
		D3D12_RESOURCE_STATE_COMMON, &ClearValue,//清除值
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));//深度模板缓冲区

	//创建深度模板视图							输入深度模板缓冲区	这里为空  深度模板描述
	GetD3dDevice()->CreateDepthStencilView(DepthStencilBuffer.Get(), NULL, DSVDes);
	
	//资源转换
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);
	//通过图形命令列表 去设置资源围栏
	GetGraphicsCommandList()->ResourceBarrier(1, &Barrier);
}

void FDynamicCubeMap::BuildDepthStencilDescriptor()
{
	//深度模板描述符的大小 用作偏移量
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	//对描述的一个偏移
	DSVDes = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),//获取到当前的深度模板视图
		1,	//偏移1 这里第一个位置是给场景使用的
		DescriptorHandleIncrementSize);//偏移量 深度模板描述符的大小
}

void FDynamicCubeMap::BuildRenderTargetDescriptor()
{
	//创建渲染目标视图 
	BuildRenderTargetRTV();
	
	//创建Shader着色器资源视图赋值
	BuildRenderTargetSRV();

	//初始化Target 渲染目标  宽 高  还有贴图格式
	RenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void FDynamicCubeMap::BuildRenderTargetRTV()
{
	//获取RTV的偏移 渲染目标视图
	UINT RTVDescriptorSize = GetDescriptorHandleIncrementSizeByRTV();

	//RTV渲染目标视图的起始
	auto RTVDesHeapStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();

	//偏移的地址记录 渲染目标视图
	for (size_t i = 0; i < 6; i++)
	{
		RenderTarget->CPURenderTargetView[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE( //在VPU渲染目标视图中CPU描述符偏移
			RTVDesHeapStart,	//输入渲染目标视图描述的起始点
			FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,//交换链
			RTVDescriptorSize);//渲染目标视图描述符大小 进行偏移
	}
}

void FDynamicCubeMap::BuildRenderTargetSRV()
{
	//获取CBV_SRV_UAV的偏移
	UINT CBVDescriptorSize = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	//通过几何map拿到CPU的描述符堆的首地址  CPU SRV首地址  着色器资源视图
	auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	//通过几何map拿到GPU的描述符堆的首地址  GPU SRV首地址  着色器资源视图
	auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

	RenderTarget->CPUShaderResourceView =		//通过渲染目标，对CPU着色器资源视图赋值
		CD3DX12_CPU_DESCRIPTOR_HANDLE(CPUSRVDesHeapStart, //通过CPU描述句柄进行赋值  首先输入CPU的SRV首地址
			GeometryMap->GetDrawTexture2DResourcesNumber() //输入2D纹理贴图的资源数量 
			+ GeometryMap->GetDrawCubeMapResourcesNumber(), //输入静态立方体贴图的资源数量 
			CBVDescriptorSize); //CBV描述符大小 之后进行偏移

	RenderTarget->GPUShaderResourceView =		//通过渲染目标，对GPU着色器资源视图赋值
		CD3DX12_GPU_DESCRIPTOR_HANDLE(GPUSRVDesHeapStart, //通过GPU描述句柄进行赋值  首先输入GPU的SRV首地址
			GeometryMap->GetDrawTexture2DResourcesNumber()  //输入2D纹理贴图的资源数量 
			+  GeometryMap->GetDrawCubeMapResourcesNumber(), //输入静态立方体贴图的资源数量 
			CBVDescriptorSize); ////CBV描述符大小 之后进行偏移
}



FDynamicCubeMap::FTmpViewportCapture::FTmpViewportCapture(const fvector_3d& InCenterPoint)
{
	//构建视口捕获结构体
	BuildViewportCapture(InCenterPoint);
}

void FDynamicCubeMap::FTmpViewportCapture::BuildViewportCapture(const fvector_3d& InCenterPoint)
{
	//捕获摄像机四个面  计算顺序要与立方体贴图顺序一致 X -X Y -Y Z -Z
	TargetPoint[0] = fvector_3d(InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z); //x轴偏移
	TargetPoint[1] = fvector_3d(InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z); //-x轴偏移
	TargetPoint[2] = fvector_3d(InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z); //y轴偏移
	TargetPoint[3] = fvector_3d(InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z); //-y轴偏移
	TargetPoint[4] = fvector_3d(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f); //z轴偏移
	TargetPoint[5] = fvector_3d(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f); //-z轴偏移

	UP[0] = fvector_3d(0.f, 1.f, 0.f); // x
	UP[1] = fvector_3d(0.f, 1.f, 0.f); // -x
	UP[2] = fvector_3d(0.f, 0.f, -1.f);// y
	UP[3] = fvector_3d(0.f, 0.f, 1.f); // -y
	UP[4] = fvector_3d(0.f, 1.f, 0.f); // z
	UP[5] = fvector_3d(0.f, 1.f, 0.f); // -z

}
