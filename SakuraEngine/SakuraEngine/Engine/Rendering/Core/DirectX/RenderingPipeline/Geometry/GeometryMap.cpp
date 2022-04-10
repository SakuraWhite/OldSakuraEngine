#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Mesh.h"

FGeometryMap::FGeometryMap()
{
	Geometrys.insert(pair<int, FGeometry>(0, FGeometry()) );
}

void FGeometryMap::PreDraw(float DeltaTime)
{	//预初始化
	//设置描述堆
//	DescriptorHeap.PreDraw(DeltaTime);
 	ID3D12DescriptorHeap* DescriptorHeap[] = { GetHeap() };//堆里是常量缓冲区
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);//设置描述堆
}

void FGeometryMap::Draw(float DeltaTime)
{
	//渲染视口
	DrawViewport(DeltaTime);
	//渲染模型Mesh
	DrawMesh(DeltaTime);

}

void FGeometryMap::PostDraw(float DeltaTime)
{

}

void FGeometryMap::BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData)
{
	FGeometry &Geometry = Geometrys[0];
	
	Geometry.BuildMesh(InMesh, MeshData);
}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//视口投影矩阵
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);//转换成矩阵 视口投影 定义摄像机向上向左向右向前向后的一些方向设置
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);//转换成矩阵   对摄像机的裁剪距离 近远剪裁面，FOV等的设置

	for (auto &Tmp : Geometrys)//遍历的方式更新模型
	{
		for (size_t i = 0; i < Tmp.second.DescribeMeshRenderingData.size(); i++) //遍历描述模型渲染数据
		{
			FRenderingData &InRenderingData = Tmp.second.DescribeMeshRenderingData[i];//获取我们的渲染数据

			//构造模型世界坐标   更新位置 旋转 缩放
			{
				//获取物体坐标数值
				XMFLOAT3& Position = InRenderingData.Mesh->GetPosition();
				//获取物体缩放数值
				fvector_3d Scale = InRenderingData.Mesh->GetScale();

				//获取物体的朝向
				XMFLOAT3 RightVector = InRenderingData.Mesh->GetRightVector();
				XMFLOAT3 UPVector = InRenderingData.Mesh->GetUPVector();
				XMFLOAT3 ForwardVector = InRenderingData.Mesh->GetForwardVector();

				//获取世界坐标矩阵  使用拿到的物体坐标数据进行更新赋值计算   构造更新模型的世界坐标位置旋转与缩放
				InRenderingData.WorldMatrix = {
					RightVector.x * Scale.x,	UPVector.x,				ForwardVector.x,			0.f,
					RightVector.y,				UPVector.y * Scale.x,	ForwardVector.y,			0.f,
					RightVector.z,				UPVector.z ,			ForwardVector.z * Scale.x,	0.f,
					Position.x,					Position.y,				Position.z,					1.f };
			}
			//用来计算数学

			//空间转换相关
			XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData.WorldMatrix);//转换为世界空间坐标 模型的位置
			//XMMATRIX WVP = ATRIXWorld * ViewMatrix * ATRIXProject;//裁剪or投影矩阵

			//更新物体对象常量缓冲区
			FObjectTransformation ObjectTransformation;//将WVP设置在常量缓冲区里 来时刻更新WVP
			XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));//矩形转制
			ObjectConstantBufferViews.Update(i, &ObjectTransformation);//更新
		}
	}


	//更新视口投影常量缓冲区
	//XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);//转换成矩阵 视口投影
	XMMATRIX ViewProject = XMMatrixMultiply(ViewMatrix, ProjectMatrix);//把上面对象矩阵与视口投影矩阵相乘
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProject));//存储我们计算的结果
	
	ViewportConstantBufferViews.Update(0, &ViewportTransformation);//更新


}

void FGeometryMap::Build()
{
	//构建模型
	for (auto &Tmp : Geometrys)//遍历
	{
		Tmp.second.Build();// 对每个对象都开始构建

	}
	

}

void FGeometryMap::BuildDescriptorHeap()
{
	//构建描述堆				要创建的描述堆      这里+1是摄像机
	DescriptorHeap.Build(GetDrawObjectNumber() + 1);

}

void FGeometryMap::BuildConstantBuffer()
{
	//创建对象常量缓冲区					    求字节数运算符  对象变换          数量
	ObjectConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawObjectNumber());

	//CPU_描述符_句柄																					获取堆启动的CPU描述符句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//构建常量缓冲区								CPU描述句柄	    获取绘制对象数量
	ObjectConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawObjectNumber());
}

UINT FGeometryMap::GetDrawObjectNumber()
{
	return Geometrys[0].GetDrawObjectNumber();//获取渲染对象的数量
	
}

void FGeometryMap::BuildViewportConstantBufferView()
{
	//创建视口常量缓冲区					    求字节数运算符  视口变换          数量
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	//CPU_描述符_句柄																					获取堆启动的CPU描述符句柄
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//构建视口常量缓冲区						     CPU描述句柄  偏移	   获取绘制对象数量
	ViewportConstantBufferViews.BuildConstantBuffer(DesHandle, 1, GetDrawObjectNumber());

}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	//偏移  当前描述符偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//获取Gpu相关的描述符句柄
	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(GetDrawObjectNumber(), DescriptorOffset);//对Gpu相关描述符做偏移

	//设置图形 根描述符
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	//GPU_描述符_句柄的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	//模型构建
	for (auto& Tmp : Geometrys)
	{
		//顶点缓冲区视图
		D3D12_VERTEX_BUFFER_VIEW VBV = Tmp.second.GetVertexBufferView();
		//Index索引缓冲区视图
		D3D12_INDEX_BUFFER_VIEW IBV = Tmp.second.GetIndexBufferView();

		

		for (int i = 0; i < Tmp.second.DescribeMeshRenderingData.size(); i++)//遍历模型渲染数据
		{
			//每一帧抖获取一遍 GPU_描述符_句柄
			auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

			//拿到当前描述的具体对象 渲染数据
			FRenderingData& InRenderingData = Tmp.second.DescribeMeshRenderingData[i];
			//设置当前视图
			GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

			//绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
			GetGraphicsCommandList()->IASetVertexBuffers(
				0,//起始输入槽 0~15 一共16个索引
				1,//k k+1 ... k+n-1 //与输入槽绑定的数量 顶点缓冲区的数量 设输入槽索引为k 那么我们绑定的缓冲区就是n 那公式就是k+n-1  这里有0个输入槽，那么绑定缓冲区就是n=k+1 为1
				&VBV);//输入顶点Buffer视图

			//在绘制前，定义一下我们要绘制哪一种图元，是绘制成 点或者是线 还是面 
			GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//定义绘制

			//用偏移的方式  按起始地点偏移
			DesHandle.Offset(i, DescriptorOffset);//我们的常量缓冲区每次要储存，我们的常量缓冲区都会做偏移。
			//在绘制前 将描述附表设置到根签名中 绘制在0号寄存器的对象常量缓冲区中 
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesHandle);//设置到CBV的首地址就行

			//真正的渲染接口  这个API可以帮助我们去绘制索引，实例化基元 真正的绘制
			GetGraphicsCommandList()->DrawIndexedInstanced(
				InRenderingData.IndexSize,//渲染模型的顶点数量
				1,//实例化的数量，目前只有一个
				InRenderingData.IndexOffsetPosition,//这里需要输入顶点缓冲区被绘制的索引的第一个
				InRenderingData.VertexOffsetPosition,//GPU从索引缓冲区读取的第一个索引的位置。
				0);//在从顶点缓冲区读取每一个实例数据之前添加到每个索引的值。

		}

	}


}

bool FGeometry::bRenderingDataExistence(GMesh* InKey)
{
	for (auto &Tmp : DescribeMeshRenderingData) //遍历信息
	{
		if (Tmp.Mesh == InKey)  //找到信息就返回True
		{
			return true;
		}

	}

	return false;
}

void FGeometry::BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData)
{
	if (!bRenderingDataExistence(InMesh))   //判断模型是否已经被添加过了
	{
		DescribeMeshRenderingData.push_back(FRenderingData());//添加新的Mesh

		//拿到最初始的数据
		FRenderingData& InRenderingData = DescribeMeshRenderingData[DescribeMeshRenderingData.size() - 1];

		InRenderingData.Mesh = InMesh;//注册Mesh
		InRenderingData.IndexSize = MeshData.IndexData.size();//记录Index
		InRenderingData.VertexSize = MeshData.VertexData.size();//记录顶点

		//记录Inedxsize位置偏移
		InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
		//记录顶点size位置偏移
		InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();

		//索引数据的合并
		MeshRenderingData.IndexData.insert( //高校的插入
			MeshRenderingData.IndexData.end(), //数据结尾的地址
			MeshData.IndexData.begin(),//要插入的地址开始
			MeshData.IndexData.end()); //要插入地址的结尾

		//顶点数据的合并
		MeshRenderingData.VertexData.insert(
			MeshRenderingData.VertexData.end(),
			MeshData.VertexData.begin(),
			MeshData.VertexData.end());
	
	}

}

void FGeometry::Build()
{
	//构建模型
	//声明参数
	UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();//声明顶点Size的字节
	UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();//声明Index的Size字节
	//创建顶点缓冲区																	 
	ANALYSIS_HRESULT(D3DCreateBlob(
		VertexSizeInBytes,//顶点Size的字节
		&CPUVertexBufferPtr));//CPU顶点的指针
	//通过MeshRenderingData拿到一些顶点Size的字节的指针与CPU顶点Buffer的首地址	最后到CPUVertexBufferPtr里面
	memcpy(CPUVertexBufferPtr->GetBufferPointer(),
		MeshRenderingData.VertexData.data(),//通过MeshRenderingData拿到VertexData
		VertexSizeInBytes);//顶点Size的字节

	//创建Index缓冲区
	ANALYSIS_HRESULT(D3DCreateBlob(
		IndexSizeInBytes,//IndexSize的字节
		&CPUIndexBufferPtr));//CPU顶点的指针
	memcpy(CPUIndexBufferPtr->GetBufferPointer(),   //拷贝值到CPUVertexBufferPtr里面
		MeshRenderingData.IndexData.data(),//通过MeshRenderingData拿到IndexData
		IndexSizeInBytes);//Index的Size字节

	//构建默认缓冲区
	//GPU顶点Buffer缓冲区
	ConstructBuffer::FConstructBuffer ConstructBuffer;//构造BUFF
	GPUVertexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		VertexBufferTmpPtr,//顶点上传缓冲区
		MeshRenderingData.VertexData.data(),//顶点数据指针
		VertexSizeInBytes); //顶点数据大小
	//GPUIndex缓冲区
	GPUIndexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		IndexBufferTmpPtr,//Index上传缓冲区
		MeshRenderingData.IndexData.data(),//Index数据指针
		IndexSizeInBytes);//Index数据大小

}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV;//创建Buffer
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();//指定Buffer位置
	VBV.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();//指定顶点缓冲区字节大小
	VBV.StrideInBytes = sizeof(FVertex);//指定每顶点的Size大小

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV;//创建INDEX
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();//指定INDEXBuffer位置
	IBV.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();//指定INDEX字节大小
	IBV.Format = DXGI_FORMAT_R16_UINT;//Index格式

	return IBV;
}


