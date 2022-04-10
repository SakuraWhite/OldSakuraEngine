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
{	//Ԥ��ʼ��
	//����������
//	DescriptorHeap.PreDraw(DeltaTime);
 	ID3D12DescriptorHeap* DescriptorHeap[] = { GetHeap() };//�����ǳ���������
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);//����������
}

void FGeometryMap::Draw(float DeltaTime)
{
	//��Ⱦ�ӿ�
	DrawViewport(DeltaTime);
	//��Ⱦģ��Mesh
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
	//�ӿ�ͶӰ����
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);//ת���ɾ��� �ӿ�ͶӰ �����������������������ǰ����һЩ��������
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);//ת���ɾ���   ��������Ĳü����� ��Զ�����棬FOV�ȵ�����

	for (auto &Tmp : Geometrys)//�����ķ�ʽ����ģ��
	{
		for (size_t i = 0; i < Tmp.second.DescribeMeshRenderingData.size(); i++) //��������ģ����Ⱦ����
		{
			FRenderingData &InRenderingData = Tmp.second.DescribeMeshRenderingData[i];//��ȡ���ǵ���Ⱦ����

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
					RightVector.y,				UPVector.y * Scale.x,	ForwardVector.y,			0.f,
					RightVector.z,				UPVector.z ,			ForwardVector.z * Scale.x,	0.f,
					Position.x,					Position.y,				Position.z,					1.f };
			}
			//����������ѧ

			//�ռ�ת�����
			XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData.WorldMatrix);//ת��Ϊ����ռ����� ģ�͵�λ��
			//XMMATRIX WVP = ATRIXWorld * ViewMatrix * ATRIXProject;//�ü�orͶӰ����

			//�������������������
			FObjectTransformation ObjectTransformation;//��WVP�����ڳ����������� ��ʱ�̸���WVP
			XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));//����ת��
			ObjectConstantBufferViews.Update(i, &ObjectTransformation);//����
		}
	}


	//�����ӿ�ͶӰ����������
	//XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);//ת���ɾ��� �ӿ�ͶӰ
	XMMATRIX ViewProject = XMMatrixMultiply(ViewMatrix, ProjectMatrix);//���������������ӿ�ͶӰ�������
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProject));//�洢���Ǽ���Ľ��
	
	ViewportConstantBufferViews.Update(0, &ViewportTransformation);//����


}

void FGeometryMap::Build()
{
	//����ģ��
	for (auto &Tmp : Geometrys)//����
	{
		Tmp.second.Build();// ��ÿ�����󶼿�ʼ����

	}
	

}

void FGeometryMap::BuildDescriptorHeap()
{
	//����������				Ҫ������������      ����+1�������
	DescriptorHeap.Build(GetDrawObjectNumber() + 1);

}

void FGeometryMap::BuildConstantBuffer()
{
	//����������������					    ���ֽ��������  ����任          ����
	ObjectConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawObjectNumber());

	//CPU_������_���																					��ȡ��������CPU���������
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//��������������								CPU�������	    ��ȡ���ƶ�������
	ObjectConstantBufferViews.BuildConstantBuffer(DesHandle, GetDrawObjectNumber());
}

UINT FGeometryMap::GetDrawObjectNumber()
{
	return Geometrys[0].GetDrawObjectNumber();//��ȡ��Ⱦ���������
	
}

void FGeometryMap::BuildViewportConstantBufferView()
{
	//�����ӿڳ���������					    ���ֽ��������  �ӿڱ任          ����
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	//CPU_������_���																					��ȡ��������CPU���������
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//�����ӿڳ���������						     CPU�������  ƫ��	   ��ȡ���ƶ�������
	ViewportConstantBufferViews.BuildConstantBuffer(DesHandle, 1, GetDrawObjectNumber());

}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	//ƫ��  ��ǰ������ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//��ȡGpu��ص����������
	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(GetDrawObjectNumber(), DescriptorOffset);//��Gpu�����������ƫ��

	//����ͼ�� ��������
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	//GPU_������_�����ƫ��
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	//ģ�͹���
	for (auto& Tmp : Geometrys)
	{
		//���㻺������ͼ
		D3D12_VERTEX_BUFFER_VIEW VBV = Tmp.second.GetVertexBufferView();
		//Index������������ͼ
		D3D12_INDEX_BUFFER_VIEW IBV = Tmp.second.GetIndexBufferView();

		

		for (int i = 0; i < Tmp.second.DescribeMeshRenderingData.size(); i++)//����ģ����Ⱦ����
		{
			//ÿһ֡����ȡһ�� GPU_������_���
			auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

			//�õ���ǰ�����ľ������ ��Ⱦ����
			FRenderingData& InRenderingData = Tmp.second.DescribeMeshRenderingData[i];
			//���õ�ǰ��ͼ
			GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

			//����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
			GetGraphicsCommandList()->IASetVertexBuffers(
				0,//��ʼ����� 0~15 һ��16������
				1,//k k+1 ... k+n-1 //������۰󶨵����� ���㻺���������� �����������Ϊk ��ô���ǰ󶨵Ļ���������n �ǹ�ʽ����k+n-1  ������0������ۣ���ô�󶨻���������n=k+1 Ϊ1
				&VBV);//���붥��Buffer��ͼ

			//�ڻ���ǰ������һ������Ҫ������һ��ͼԪ���ǻ��Ƴ� ��������� ������ 
			GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//�������

			//��ƫ�Ƶķ�ʽ  ����ʼ�ص�ƫ��
			DesHandle.Offset(i, DescriptorOffset);//���ǵĳ���������ÿ��Ҫ���棬���ǵĳ���������������ƫ�ơ�
			//�ڻ���ǰ �������������õ���ǩ���� ������0�żĴ����Ķ������������� 
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesHandle);//���õ�CBV���׵�ַ����

			//��������Ⱦ�ӿ�  ���API���԰�������ȥ����������ʵ������Ԫ �����Ļ���
			GetGraphicsCommandList()->DrawIndexedInstanced(
				InRenderingData.IndexSize,//��Ⱦģ�͵Ķ�������
				1,//ʵ������������Ŀǰֻ��һ��
				InRenderingData.IndexOffsetPosition,//������Ҫ���붥�㻺���������Ƶ������ĵ�һ��
				InRenderingData.VertexOffsetPosition,//GPU��������������ȡ�ĵ�һ��������λ�á�
				0);//�ڴӶ��㻺������ȡÿһ��ʵ������֮ǰ��ӵ�ÿ��������ֵ��

		}

	}


}

bool FGeometry::bRenderingDataExistence(GMesh* InKey)
{
	for (auto &Tmp : DescribeMeshRenderingData) //������Ϣ
	{
		if (Tmp.Mesh == InKey)  //�ҵ���Ϣ�ͷ���True
		{
			return true;
		}

	}

	return false;
}

void FGeometry::BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData)
{
	if (!bRenderingDataExistence(InMesh))   //�ж�ģ���Ƿ��Ѿ�����ӹ���
	{
		DescribeMeshRenderingData.push_back(FRenderingData());//����µ�Mesh

		//�õ����ʼ������
		FRenderingData& InRenderingData = DescribeMeshRenderingData[DescribeMeshRenderingData.size() - 1];

		InRenderingData.Mesh = InMesh;//ע��Mesh
		InRenderingData.IndexSize = MeshData.IndexData.size();//��¼Index
		InRenderingData.VertexSize = MeshData.VertexData.size();//��¼����

		//��¼Inedxsizeλ��ƫ��
		InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
		//��¼����sizeλ��ƫ��
		InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();

		//�������ݵĺϲ�
		MeshRenderingData.IndexData.insert( //��У�Ĳ���
			MeshRenderingData.IndexData.end(), //���ݽ�β�ĵ�ַ
			MeshData.IndexData.begin(),//Ҫ����ĵ�ַ��ʼ
			MeshData.IndexData.end()); //Ҫ�����ַ�Ľ�β

		//�������ݵĺϲ�
		MeshRenderingData.VertexData.insert(
			MeshRenderingData.VertexData.end(),
			MeshData.VertexData.begin(),
			MeshData.VertexData.end());
	
	}

}

void FGeometry::Build()
{
	//����ģ��
	//��������
	UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();//��������Size���ֽ�
	UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();//����Index��Size�ֽ�
	//�������㻺����																	 
	ANALYSIS_HRESULT(D3DCreateBlob(
		VertexSizeInBytes,//����Size���ֽ�
		&CPUVertexBufferPtr));//CPU�����ָ��
	//ͨ��MeshRenderingData�õ�һЩ����Size���ֽڵ�ָ����CPU����Buffer���׵�ַ	���CPUVertexBufferPtr����
	memcpy(CPUVertexBufferPtr->GetBufferPointer(),
		MeshRenderingData.VertexData.data(),//ͨ��MeshRenderingData�õ�VertexData
		VertexSizeInBytes);//����Size���ֽ�

	//����Index������
	ANALYSIS_HRESULT(D3DCreateBlob(
		IndexSizeInBytes,//IndexSize���ֽ�
		&CPUIndexBufferPtr));//CPU�����ָ��
	memcpy(CPUIndexBufferPtr->GetBufferPointer(),   //����ֵ��CPUVertexBufferPtr����
		MeshRenderingData.IndexData.data(),//ͨ��MeshRenderingData�õ�IndexData
		IndexSizeInBytes);//Index��Size�ֽ�

	//����Ĭ�ϻ�����
	//GPU����Buffer������
	ConstructBuffer::FConstructBuffer ConstructBuffer;//����BUFF
	GPUVertexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		VertexBufferTmpPtr,//�����ϴ�������
		MeshRenderingData.VertexData.data(),//��������ָ��
		VertexSizeInBytes); //�������ݴ�С
	//GPUIndex������
	GPUIndexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		IndexBufferTmpPtr,//Index�ϴ�������
		MeshRenderingData.IndexData.data(),//Index����ָ��
		IndexSizeInBytes);//Index���ݴ�С

}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV;//����Buffer
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();//ָ��Bufferλ��
	VBV.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();//ָ�����㻺�����ֽڴ�С
	VBV.StrideInBytes = sizeof(FVertex);//ָ��ÿ�����Size��С

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV;//����INDEX
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();//ָ��INDEXBufferλ��
	IBV.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();//ָ��INDEX�ֽڴ�С
	IBV.Format = DXGI_FORMAT_R16_UINT;//Index��ʽ

	return IBV;
}


