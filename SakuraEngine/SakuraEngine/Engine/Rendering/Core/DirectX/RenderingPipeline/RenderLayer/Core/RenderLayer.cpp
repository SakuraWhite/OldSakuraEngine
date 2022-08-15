#include "RenderLayer.h"
#include "../RenderLayerManage.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../../Core/Viewport/ViewportInfo.h"


FRenderLayer::FRenderLayer()
	:RenderPriority(0)//��Ⱦ������ʼ��
{

}

void FRenderLayer::RegisterRenderLayer()
{
	//����Ⱦ�㼶�Ž���Ⱦ�㼶��Ϣ�� ���з���
	FRenderLayerManage::RenderLayers.push_back(this->shared_from_this());

}


void FRenderLayer::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState)
{
	//��ֵ
	GeometryMap = InGeometryMap;//����
	DirectXPipelineState = InDirectXPipelineState;//����״̬
}

void FRenderLayer::PreDraw(float DeltaTime)
{

}

void FRenderLayer::Draw(float DeltaTime)
{
	//GPU_������_�����ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	//ģ�͹���
	for (auto& InRenderingData : RenderDatas)
	{
		//���㻺������ͼ		ͨ��������ͼ�����Ⱦ�����еļ���Key�ҵ����㻺������ͼ
		D3D12_VERTEX_BUFFER_VIEW VBV = GeometryMap->Geometrys[InRenderingData.GeometryKey].GetVertexBufferView();
		//Index������������ͼ 
		D3D12_INDEX_BUFFER_VIEW IBV = GeometryMap->Geometrys[InRenderingData.GeometryKey].GetIndexBufferView();


		//����ģ�͵�GPU��������� ÿһ֡����ȡһ��  GPU_������_���
		auto DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart());

		//���õ�ǰ��ͼ
		GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

		//����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
		GetGraphicsCommandList()->IASetVertexBuffers(
			0,//��ʼ����� 0~15 һ��16������
			1,//k k+1 ... k+n-1 //������۰󶨵����� ���㻺���������� �����������Ϊk ��ô���ǰ󶨵Ļ���������n �ǹ�ʽ����k+n-1  ������0������ۣ���ô�󶨻���������n=k+1 Ϊ1
			&VBV);//���붥��Buffer��ͼ

		//������Ⱦ״̬
		if ((*InRenderingData.Mesh->GetMaterials())[0]->GetMaterialDisplayStatus())
		{

		}


		//                                            ͨ����Ⱦ�����õ�mesh     ���õ�����Ų���      ֮�����õ�������ʾ����
		EMaterialDisplayStatusType DisplayStatus = (*InRenderingData.Mesh->GetMaterials())[0]->GetMaterialDisplayStatus();//���������ʾ����
		//�ڻ���ǰ������һ������Ҫ������һ��ͼԪ���ǻ��Ƴ� ��������� ������ 
		GetGraphicsCommandList()->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)DisplayStatus);//�������
			
		//ģ����ʼ��ַƫ��			��ƫ�Ƶķ�ʽ  ����ʼ�ص�ƫ��
		DesMeshHandle.Offset(InRenderingData.MeshObjectIndex, DescriptorOffset);//���ǵĳ���������ÿ��Ҫ���棬���ǵĳ���������������ƫ�ơ�
		//�ڻ���ǰ �������������õ���ǩ���� ������0�żĴ����Ķ������������� 
		GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);//���õ�CBV���׵�ַ����


		//��������Ⱦ�ӿ�  ���API���԰�������ȥ����������ʵ������Ԫ �����Ļ���
		GetGraphicsCommandList()->DrawIndexedInstanced(
			InRenderingData.IndexSize,//��Ⱦģ�͵Ķ�������
			1,//ʵ������������Ŀǰֻ��һ��
			InRenderingData.IndexOffsetPosition,//������Ҫ���붥�㻺���������Ƶ������ĵ�һ��
			InRenderingData.VertexOffsetPosition,//GPU��������������ȡ�ĵ�һ��������λ�á�
			0);//�ڴӶ��㻺������ȡÿһ��ʵ������֮ǰ��ӵ�ÿ��������ֵ��
	

	}

}

void FRenderLayer::PostDraw(float DeltaTime)
{

}

void FRenderLayer::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{

	for (auto& InRenderingData : RenderDatas)//������Ⱦ���ݵķ�ʽ����ģ��
	{

		//����ģ����������   ����λ�� ��ת ����
		{
			//��ȡ����������ֵ
			XMFLOAT3& Position = InRenderingData.Mesh->GetPosition();
			//��ȡ����������ֵ
			fvector_3d Scale = InRenderingData.Mesh->GetScale();

			//��ȡ����ĳ���
			XMFLOAT3 RightVector = InRenderingData.Mesh->GetRightVector();
			XMFLOAT3 UPVector = InRenderingData.Mesh->GetUPVector();
			XMFLOAT3 ForwardVector = InRenderingData.Mesh->GetForwardVector();

			//��ȡ�����������  ʹ���õ��������������ݽ��и��¸�ֵ����   �������ģ�͵���������λ����ת������
			InRenderingData.WorldMatrix = {
				RightVector.x * Scale.x,	UPVector.x,				ForwardVector.x,			0.f,
				RightVector.y,				UPVector.y * Scale.y,	ForwardVector.y,			0.f,
				RightVector.z,				UPVector.z ,			ForwardVector.z * Scale.z,	0.f,
				Position.x,					Position.y,				Position.z,					1.f };
		}
		//����������ѧ


		//����ģ��λ��
		//�ռ�ת�����
		XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData.WorldMatrix);//ת��Ϊ����ռ����� ģ�͵�λ��
		//��ͼ�任���
		XMMATRIX ATRITextureTransform = XMLoadFloat4x4(&InRenderingData.TextureTransform);//��ȡ��ǰ����任����
		//XMMATRIX WVP = ATRIXWorld * ViewMatrix * ATRIXProject;//�ü�orͶӰ����

		//�������������������
		FObjectTransformation ObjectTransformation;//��WVP�����ڳ����������� ��ʱ�̸���WVP
		XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));//����ռ���� ����ת��
		XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, XMMatrixTranspose(ATRITextureTransform));//����任 ����ת��

		//������������ֵ  �ռ�����Index
		if (auto& InMater = (*InRenderingData.Mesh->GetMaterials())[0])//����Ⱦ������ȡ��ģ���ϵĲ���
		{
			//��ȡ���Ĳ������鸳ֵ�ڳ����������еĲ���������
			ObjectTransformation.MaterialIndex = InMater->GetMaterialIndex();
		}

		//ͨ������map���ó��������� ����ģ�͸���
		GeometryMap->MeshConstantBufferViews.Update(InRenderingData.MeshObjectIndex, &ObjectTransformation);//����ģ��

		
	}
}
