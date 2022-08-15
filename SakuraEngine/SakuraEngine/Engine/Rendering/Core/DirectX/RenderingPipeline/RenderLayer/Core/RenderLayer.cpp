#include "RenderLayer.h"
#include "../RenderLayerManage.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../../Core/Viewport/ViewportInfo.h"


FRenderLayer::FRenderLayer()
	:RenderPriority(0)//渲染索引初始化
{

}

void FRenderLayer::RegisterRenderLayer()
{
	//把渲染层级放进渲染层级信息里 进行访问
	FRenderLayerManage::RenderLayers.push_back(this->shared_from_this());

}


void FRenderLayer::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState)
{
	//赋值
	GeometryMap = InGeometryMap;//几何
	DirectXPipelineState = InDirectXPipelineState;//管线状态
}

void FRenderLayer::PreDraw(float DeltaTime)
{

}

void FRenderLayer::Draw(float DeltaTime)
{
	//GPU_描述符_句柄的偏移
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	//模型构建
	for (auto& InRenderingData : RenderDatas)
	{
		//顶点缓冲区视图		通过几何视图里的渲染数据中的几何Key找到顶点缓冲区视图
		D3D12_VERTEX_BUFFER_VIEW VBV = GeometryMap->Geometrys[InRenderingData.GeometryKey].GetVertexBufferView();
		//Index索引缓冲区视图 
		D3D12_INDEX_BUFFER_VIEW IBV = GeometryMap->Geometrys[InRenderingData.GeometryKey].GetIndexBufferView();


		//创建模型的GPU描述符句柄 每一帧都获取一遍  GPU_描述符_句柄
		auto DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart());

		//设置当前视图
		GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

		//绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
		GetGraphicsCommandList()->IASetVertexBuffers(
			0,//起始输入槽 0~15 一共16个索引
			1,//k k+1 ... k+n-1 //与输入槽绑定的数量 顶点缓冲区的数量 设输入槽索引为k 那么我们绑定的缓冲区就是n 那公式就是k+n-1  这里有0个输入槽，那么绑定缓冲区就是n=k+1 为1
			&VBV);//输入顶点Buffer视图

		//定义渲染状态
		if ((*InRenderingData.Mesh->GetMaterials())[0]->GetMaterialDisplayStatus())
		{

		}


		//                                            通过渲染数据拿到mesh     再拿到第零号材质      之后再拿到材质显示类型
		EMaterialDisplayStatusType DisplayStatus = (*InRenderingData.Mesh->GetMaterials())[0]->GetMaterialDisplayStatus();//定义材质显示类型
		//在绘制前，定义一下我们要绘制哪一种图元，是绘制成 点或者是线 还是面 
		GetGraphicsCommandList()->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)DisplayStatus);//定义绘制
			
		//模型起始地址偏移			用偏移的方式  按起始地点偏移
		DesMeshHandle.Offset(InRenderingData.MeshObjectIndex, DescriptorOffset);//我们的常量缓冲区每次要储存，我们的常量缓冲区都会做偏移。
		//在绘制前 将描述附表设置到根签名中 绘制在0号寄存器的对象常量缓冲区中 
		GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);//设置到CBV的首地址就行


		//真正的渲染接口  这个API可以帮助我们去绘制索引，实例化基元 真正的绘制
		GetGraphicsCommandList()->DrawIndexedInstanced(
			InRenderingData.IndexSize,//渲染模型的顶点数量
			1,//实例化的数量，目前只有一个
			InRenderingData.IndexOffsetPosition,//这里需要输入顶点缓冲区被绘制的索引的第一个
			InRenderingData.VertexOffsetPosition,//GPU从索引缓冲区读取的第一个索引的位置。
			0);//在从顶点缓冲区读取每一个实例数据之前添加到每个索引的值。
	

	}

}

void FRenderLayer::PostDraw(float DeltaTime)
{

}

void FRenderLayer::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{

	for (auto& InRenderingData : RenderDatas)//遍历渲染数据的方式更新模型
	{

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
				RightVector.y,				UPVector.y * Scale.y,	ForwardVector.y,			0.f,
				RightVector.z,				UPVector.z ,			ForwardVector.z * Scale.z,	0.f,
				Position.x,					Position.y,				Position.z,					1.f };
		}
		//用来计算数学


		//更新模型位置
		//空间转换相关
		XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData.WorldMatrix);//转换为世界空间坐标 模型的位置
		//贴图变换相关
		XMMATRIX ATRITextureTransform = XMLoadFloat4x4(&InRenderingData.TextureTransform);//获取当前纹理变换矩阵
		//XMMATRIX WVP = ATRIXWorld * ViewMatrix * ATRIXProject;//裁剪or投影矩阵

		//更新物体对象常量缓冲区
		FObjectTransformation ObjectTransformation;//将WVP设置在常量缓冲区里 来时刻更新WVP
		XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));//世界空间矩阵 矩形转制
		XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, XMMatrixTranspose(ATRITextureTransform));//纹理变换 矩形转制

		//给材质索引赋值  收集材质Index
		if (auto& InMater = (*InRenderingData.Mesh->GetMaterials())[0])//从渲染数据中取出模型上的材质
		{
			//将取出的材质数组赋值在常量缓冲区中的材质索引里
			ObjectTransformation.MaterialIndex = InMater->GetMaterialIndex();
		}

		//通过几何map调用常量缓冲区 进行模型更新
		GeometryMap->MeshConstantBufferViews.Update(InRenderingData.MeshObjectIndex, &ObjectTransformation);//更新模型

		
	}
}
