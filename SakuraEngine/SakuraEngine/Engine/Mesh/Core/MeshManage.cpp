#include "MeshManage.h"
#include "../../Config/EngineRenderConfig.h"
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CustomMesh.h"
#include "../CylinderMesh.h"
#include "../SphereMesh.h"
#include "../PlaneMesh.h"
#include "ObjectTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"

CMeshManage::CMeshManage()
	:VertexSizeInBytes(0)//初始化顶点缓冲区字节大小
	, VertexStrideInBytes(0) //初始化每顶点的字节大小
	, IndexSizeInBytes(0)//初始化Index字节大小
	, IndexFormat(DXGI_FORMAT_R16_UINT)//初始化Index的格式，这里给了一个R16的INT类型 最小的占比
	, IndexSize(0)//用于保存一份顶点数量
	, WorldMatrix(FObjectTransformation::IdentityMatrix4x4())//世界空间坐标 单位化初始化 
	, ViewMatrix(FObjectTransformation::IdentityMatrix4x4())//屏幕空间坐标单位化
	, ProjectMatrix(FObjectTransformation::IdentityMatrix4x4())//物体空间坐标单位化
{

}

void CMeshManage::Init()
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
		1000.f		//近剪裁平面到远剪裁面的距离 。
	);

	XMStoreFloat4x4(&ProjectMatrix, Project);//物体空间的转换
}

void CMeshManage::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	//构建CBV栈描述
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = 1;//多少个描述
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; //需要指定一个Type 这里指定栈的类型包含CBV的描述状态
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//设置当前标志
	HeapDesc.NodeMask = 0;
	GetD3dDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&CBVHeap));


	//常量缓冲区的构建
//////////////////////////////////////////////////////////////////////////////////
	ObjectConstants = make_shared<FRenderingResourcesUpdate>();//创建当前常量

	ObjectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);//常量缓冲区的初始化

	D3D12_GPU_VIRTUAL_ADDRESS Addr = ObjectConstants->GetBuffer()->GetGPUVirtualAddress();//拿到常量地址 可以通过GetBuffer获取到GPU的地址
	//增加描述
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
	CBVDesc.BufferLocation = Addr;//创建描述前需要获得这个Buffer地址
	CBVDesc.SizeInBytes = ObjectConstants->GetConstantBufferByteSize();//获得Buffer的size
	
	//通过获得D3D驱动来创建常量缓冲区
	GetD3dDevice()->CreateConstantBufferView(
		&CBVDesc,//输入描述
		CBVHeap->GetCPUDescriptorHandleForHeapStart());//通过CBV栈获取到CPU里的描述

	//根签名构建  //根签名：部署Shader环境 根签名可以帮助着色器提供渲染流水线里面的所有资源
	CD3DX12_ROOT_PARAMETER RootParam[1];

	//CBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCBV;//用来描述CBV的表
	//Param初始化
	DescriptorRangeCBV.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //指定范围为CBV   CBV=常量缓冲区视图
		1, 0);//第二个参数是有多少个描述符  第三个参数是基于哪一个着色器的寄存器 这里是0号

	RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeCBV);//第一个参数是当前的范围 第二个指定相应的内容

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		1,//指定有多少个参数
		RootParam,//指定的参数
		0,//有多少个静态采样
		nullptr,//指定静态采样实例
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);//根签名的布局选项

	//二进制序列化
	ComPtr<ID3DBlob> SerializeRootSignature;
	ComPtr<ID3DBlob> ErrorBlob;//序列化报错
	//序列化根签名
	D3D12SerializeRootSignature(
		&RootSignatureDesc, //根描述
		D3D_ROOT_SIGNATURE_VERSION_1,//根版本
		SerializeRootSignature.GetAddressOf(),//指定二进制序列化的内容
		ErrorBlob.GetAddressOf());//指定二进制报错

	//判断报错的签名是否是空的
	if (ErrorBlob)
	{
		Engine_Log_Error("%s", (char*)ErrorBlob->GetBufferPointer());//如果报错不是空的 那么打印报错
	}

	//通过驱动创建根签名
	GetD3dDevice()->CreateRootSignature(
		0,//这里对单个GPU进行操作
		SerializeRootSignature->GetBufferPointer(),//指定序列化的获取Buffer的指针
		SerializeRootSignature->GetBufferSize(),////指定序列化的获取Buffer的Size
		IID_PPV_ARGS(&RootSignature));//创建根签名


	//构建Shader HLSL
/////////////////////////////////////////////////////////////////////////////////
	VertexShader.BuildShaders(//构建顶点着色器
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//寻找外部的hlsl着色器语言文件
		"VertexShaderMain",//入口函数名
		"vs_5_0");//使用的版本
	
	
	PixelShader.BuildShaders(			//构建像素着色器
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",//寻找外部的hlsl着色器语言文件
		"PixelShaderMain", //入口函数名
		"ps_5_0");//构建像素着色器


	//构建层 位置颜色法线等等信息
	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},//指定位置 元素语义  指定格式  插入槽  内存中的偏移  当前的位置
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}//指定颜色 元素语义  指定格式  插入槽  内存中的偏移  当前的位置
	};

	//构建模型
//////////////////////////////////////////////////////////////////////////////////


	VertexStrideInBytes = sizeof(FVertex);//指定每顶点的大小
	IndexSize = InRenderingData->IndexData.size();//保存一份顶点数量

	//获取了模型数据的大小
	VertexSizeInBytes = InRenderingData->VertexData.size() * VertexStrideInBytes;//拿当前顶点数据的Size去乘以自己作的顶点数据
	IndexSizeInBytes = IndexSize * sizeof(uint16_t);//同样是索引的size去乘自己作的索引数据
	//创建顶点缓冲区     D3DCreateBlob可以创建二进制的缓冲区，其中传入VertexSize作为大小，然后是CPUVertexBufferPtr CPU顶点的指针
	ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
	//通过memcpy拿到一些顶点数据与CPU顶点Buffer的首地址   最后到CPUVertexBufferPtr里面
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);
	
	//创建Index缓冲区
	ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);
	//构建默认缓冲区
	//GPU顶点Buffer缓冲区
	GPUVertexBufferPtr = ConstructDefaultBuffer(
		VertexBufferTmpPtr,//顶点上传缓冲区
		InRenderingData->VertexData.data(),//顶点数据指针
		VertexSizeInBytes); //顶点数据大小
	//GPUIndex缓冲区
	GPUIndexBufferPtr = ConstructDefaultBuffer(
		IndexBufferTmpPtr,//Index上传缓冲区
		InRenderingData->IndexData.data(),//Index数据指针
		IndexSizeInBytes);//Index数据大小

	//PSO 流水线绑定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;//描述当前管线状态
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));//初始化

	//PSO流水线状态对象   流水线绑定
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();//这里是给指针
	GPSDesc.InputLayout.NumElements = (UINT)InputElementDesc.size();//绑定输入数组  这里输入的是大小

	//绑定根签名
	GPSDesc.pRootSignature = RootSignature.Get(); //根签名

	//绑定顶点着色器代码
	GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader.GetBufferPointer());//这里接收是以字节的方式接收 所以要转成字节
	GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();

	//绑定像素着色器
	GPSDesc.PS.pShaderBytecode = PixelShader.GetBufferPointer();
	GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();

	//配置光栅化状态
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//以线框方式显示

	//0000..0000
	GPSDesc.SampleMask = UINT_MAX;//混合状态下的示例蒙版 因为多重采样最多是可以采样32位样本的 这里UINT_MAX采样所有的样本

	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//图元的拓扑类型， 这里是以三角形的方式形成面
	GPSDesc.NumRenderTargets = 1;//指定渲染目标

	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);//指定混合状态
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//深度模板

	GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();//判断多重采样的数量
	GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();//判断多重采样的质量

	//RTV 和 DSV格式
	GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat(); //设置RTV的格式 纹理格式后缓冲区
	GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();//设置DSV  深度模板
	//获取D3D驱动
	ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)))//获取Graph管线状态
}

void CMeshManage::PostDraw(float DeltaTime)
{
	XMUINT3 MeshPos = XMUINT3(5.0f, 5.0f, 5.0f);//自己定的物体坐标
	//屏幕视口相关
	XMVECTOR Pos = XMVectorSet(MeshPos.x, MeshPos.y, MeshPos.z, 1.0f); //给定齐次坐标
	XMVECTOR ViewTarget = XMVectorZero();//屏幕能看到的目标
	XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);//头顶指向上方

	XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);//屏幕看到的方向
	XMStoreFloat4x4(&ViewMatrix, ViewLookAt);//转换为屏幕空间
	//空间转换相关
	XMMATRIX ATRIXWorld = XMLoadFloat4x4(&WorldMatrix);//转换为世界空间坐标
	XMMATRIX ATRIXProject = XMLoadFloat4x4(&ProjectMatrix);//转换为物体空间坐标
	XMMATRIX WVP = ATRIXWorld * ViewLookAt * ATRIXProject;//裁剪or投影矩阵
	//将WVP设置在常量缓冲区里 来时刻更新WVP
	FObjectTransformation ObjectTransformation;
	XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(WVP));//矩形转制
	ObjectConstants->Update(0, &ObjectTransformation);//更新
}

void CMeshManage::Draw(float DeltaTime)
{
	//设置描述堆
	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };//堆里是常量缓冲区
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);//设置描述堆
	//设置根签名
	GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());//根签名是每帧都需要设置的




	D3D12_VERTEX_BUFFER_VIEW VBV = GetVertexBufferView();//顶点Buffer视图 VertexBufferView简称VBV

	//绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
	GetGraphicsCommandList()->IASetVertexBuffers(
		0,//起始输入槽 0~15 一共16个索引
		1,//k k+1 ... k+n-1 //与输入槽绑定的数量 顶点缓冲区的数量 设输入槽索引为k 那么我们绑定的缓冲区就是n 那公式就是k+n-1  这里有0个输入槽，那么绑定缓冲区就是n=k+1 为1
		&VBV);//输入顶点Buffer视图


	//IndexBuffer视图 IndexBufferView简称IBV
	D3D12_INDEX_BUFFER_VIEW IBV = GetIndexBufferView();
	//绑定渲染流水线上的输入槽，可以在输入装配其阶段传入Index  只是绑定 并不是真正的渲染
	GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

	//在绘制前，定义一下我们要绘制哪一种图元，是绘制成 点或者是线 还是面 
	GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//定义绘制成
	//在绘制前 将描述附表设置到根签名中
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());//设置到CBV的首地址就行

	//真正的渲染接口  这个API可以帮助我们去绘制索引，实例化基元 真正的绘制
	GetGraphicsCommandList()->DrawIndexedInstanced(
		IndexSize,//渲染模型的顶点数量
		1,//实例化的数量，目前只有一个
		0,//这里需要输入顶点缓冲区被绘制的索引的第一个 就是0
		0,//GPU从索引缓冲区读取的第一个索引的位置。
		0);//在从顶点缓冲区读取每一个实例数据之前添加到每个索引的值。
}

void CMeshManage::PreDraw(float DeltaTime)
{
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

D3D12_VERTEX_BUFFER_VIEW CMeshManage::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV;//创建Buffer
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();//指定Buffer位置
	VBV.SizeInBytes = VertexSizeInBytes;//指定缓冲区字节大小
	VBV.StrideInBytes = VertexStrideInBytes;//指定每顶点的大小

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW CMeshManage::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV;//创建INDEX
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();//指定INDEXBuffer位置
	IBV.SizeInBytes = IndexSizeInBytes;//指定INDEX字节大小
	IBV.Format = IndexFormat;//Index格式

	return IBV;
}

CMesh* CMeshManage::CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<CSphereMesh>(InRadius, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreateMesh(string& InPath)
{
	return CreateMesh<CCustomMesh>(InPath);
}

CMesh* CMeshManage::CreateBoxMesh(float InHeight, float InWidth, float InDepth)
{
	return CreateMesh<CBoxMesh>(InHeight, InWidth, InDepth);
}

CMesh* CMeshManage::CreateConeMesh(float InRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<CConeMesh>(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<CCylinderMesh>(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	return CreateMesh<CPlaneMesh>(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

template<class T, typename ...ParamTypes>
T* CMeshManage::CreateMesh(ParamTypes && ...Params)
{
	T* MyMesh = new T();

	//提取模型资源
	FMeshRenderingData MeshData;
	MyMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

	MyMesh->BeginInit();

	//构建mesh
	BuildMesh(&MeshData);

	MyMesh->Init();

	return MyMesh;
}