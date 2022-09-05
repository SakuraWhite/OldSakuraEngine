#include "DynamicCubeMap.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../../.././../Core/Viewport/ClientViewport.h"
#include "../../RenderLayer/RenderLayerManage.h"
#include "../../../../../../Config/EngineRenderConfig.h"
#include "../../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"

FDynamicCubeMap::FDynamicCubeMap()
	:GeometryMap(NULL)//��ʼ������map  Ϊ��
	, DirectXPipelineState(NULL)//��ʼ�����߶���״̬ Ϊ��
	, RenderLayer(NULL)//��ʼ����Ⱦ�㼶 Ϊ��
	, Width(256) //��ʼ����
	, Height(256) //��ʼ����
{
	//��ȾĿ��ʵ��  ��ʼ��ΪCubemap��ȾĿ���
	RenderTarget = std::make_unique<FCubeMapRenderTarget>();
}

void FDynamicCubeMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	if (CubeMapViewport.size() == 6) //�ж�Cubemap�ӿ����Ƿ���ֵ
	{
		for (int i = 0; i < GeometryMap->DynamicReflectionMeshComponents.size(); i++) //������ȡ������Map��Ķ�̬����ģ�����
		{
			CMeshComponent* Tmp = GeometryMap->DynamicReflectionMeshComponents[i];//��ȡ��̬����ģ�����
			XMFLOAT3 F3 = Tmp->GetPosition(); //��ȡλ��
			SetCubeMapViewportPosition(fvector_3d(F3.x, F3.y, F3.z));//������������ͼ�ӿ�λ��

			for (size_t j = 0; j < 6; j++) //����6��
			{
				FViewportInfo MyViewportInfo; //�ӿ���Ϣ

				XMFLOAT3 ViewPosition = CubeMapViewport[j]->GetPosition(); //�ӿ�λ��
				MyViewportInfo.ViewPosition = XMFLOAT4(ViewPosition.x, ViewPosition.y, ViewPosition.z, 1.f);//�����ӿ�λ��
				MyViewportInfo.ViewMatrix = CubeMapViewport[j]->ViewMatrix; //�����ӿھ���
				MyViewportInfo.ProjectMatrix = CubeMapViewport[j]->ProjectMatrix;//������Ŀ����

				//ͨ������MAPȥ���¼����ӿ�							�ӿ���Ϣ			
				GeometryMap->UpdateCalculationsViewport(DeltaTime, MyViewportInfo,
					j + i * 6 	//����̬�����
					+ 1);	//������� �ӿ�ƫ��
			}
		}
	}
}

void FDynamicCubeMap::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer)
{
	//ֱ�ӽ��и�ֵ
	GeometryMap = InGeometryMap;//����map
	DirectXPipelineState = InDirectXPipelineState;//���߶���״̬
	RenderLayer = InRenderLayer;//��Ⱦ�㼶

}

void FDynamicCubeMap::PreDraw(float DeltaTime)
{

	for (int j = 0; j < GeometryMap->DynamicReflectionMeshComponents.size(); j++) //������Ҫ�����ģ�����������ж�̬����ģ�����
	{

		//ָ���ĸ���Դ ת����״̬
		//D3D12_RESOURCE_STATE_RENDER_TARGET������Ҫ��Ⱦ��Ŀ������д���״̬
		CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
			RenderTarget->GetRenderTarget(), //������ȾĿ��
			D3D12_RESOURCE_STATE_GENERIC_READ, //�����ǿɶ�
			D3D12_RESOURCE_STATE_RENDER_TARGET);//ת��Ϊ��ȾĿ��

		//ͨ��ͼ�������б�ȥ������Դ����
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresent);

		//��Ҫÿִ֡��
		//�󶨾��ο�
		auto RenderTargetViewport = RenderTarget->GetViewport(); //��ȾĿ���ӿ���Ϣ
		auto RenderTargetScissorRect = RenderTarget->GetScissorRect(); //��ȾĿ��FOV�����ӿ���Ϣ
		GetGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewport);//1�����Ű󶨵�������֮���ǰ��ӿ���Ϣ
		GetGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);//1�����������֮���ǰ��ӿھ��δ�С

		//��ȡCBV�ĳ�����������size ��Ϊ��ǰ��ƫ��
		UINT CBVSize = GeometryMap->ViewportConstantBufferViews.GetConstantBufferByteSize();

		for (size_t i = 0; i < 6; i++) //������Ҫ���6������������������б���
		{

			//�������
			GetGraphicsCommandList()->ClearRenderTargetView(//���ǿ���ʹ��ClearRenderTargetView���ɻ������Ϊ������ɫ�������ɫ��ɫ��ɫ�ȵ�
				RenderTarget->CPURenderTargetView[i],//��ȡCPU��ȾĿ����ͼ
				DirectX::Colors::Black,		//�ѻ��������Ϊ������ɫ��API
				0, nullptr);//0��nullptr�����ӿ����

			//������ģ�建����
			//����ǰ��һ����������ȾĿ�������ģ�建������������ĺϲ��׶�
			GetGraphicsCommandList()->ClearDepthStencilView(
				DSVDes, //��ȡ���ģ����ͼ����
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,//��������ģ�� λ����
				1.f,//������������1�������Ȼ�����
				0, //������������0�����ģ�建����
				0, NULL);//�����������������DX12��APIһ�¼���

			//ָ����Ⱦ������ ����ϲ��׶�
			GetGraphicsCommandList()->OMSetRenderTargets(
				1,//ָ���󶨵�����
				&RenderTarget->CPURenderTargetView[i], //ָ����ȾĿ��  CPU��ȾĿ����ͼ
				true,//������ľ��ָ��һ��������������ָ��
				&DSVDes);//ָ�����Ŀ��  ���ģ����ͼ����


			//����6������� ��6�������
			auto ViewprotAddr = GeometryMap->ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();//��ȡ�ӿڳ�����������GPU�׵�ַ
			ViewprotAddr += (
				1 +	//�������
				i + j * 6 //i����ȾĿ���Cubemap����� J�Ǳ�������̬����ģ����� Ȼ����ƫ��
				) * CBVSize; //�׵�ַ+1֮���ټ���I�ٳ���ƫ��ֵ ����ƫ��
			//��ͼ�θ�����Ϊ������������ͼ								��ǩ��	ƫ��ֵ
			GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewprotAddr);


			//����㼶��Ⱦ
			RenderLayer->Draw(RENDERLAYER_BACKGROUND, DeltaTime);//��Ⱦ������
			RenderLayer->Draw(RENDERLAYER_OPAQUE, DeltaTime); //��Ⱦ��͸����
			RenderLayer->Draw(RENDERLAYER_TRANSPARENT, DeltaTime);//��Ⱦ͸����
		}



		//�����µ�״̬׼����Ⱦ
		CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
			RenderTarget->GetRenderTarget(),//������ȾĿ��
			D3D12_RESOURCE_STATE_RENDER_TARGET,//D3D12��Դ״̬����Ŀ��
			D3D12_RESOURCE_STATE_GENERIC_READ);//D3D12��Դ״̬ͨ�ö�ȡ
		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

		//��ʼ�������ӿ���ȾĿ��
		StartSetMainViewportRenderTarget();

		//��Ⱦ���ӿ�
		GeometryMap->DrawViewport(DeltaTime);
		//������Ⱦ
		Draw(DeltaTime);

		RenderLayer->FindObjectDraw(  //������Ⱦ�㼶�㼶 ������ָ��������Ⱦ
			DeltaTime,
			RENDERLAYER_OPAQUE_REFLECTOR,//ָ����Ⱦ�� 
			GeometryMap->DynamicReflectionMeshComponents[j]);//ָ����̬����ģ�����

		//����CubeMap
		GeometryMap->DrawCubeMapTexture(DeltaTime);

		//End �����������ӿ���ȾĿ��
		EndSetMainViewportRenderTarget();

	}
}

void FDynamicCubeMap::Draw(float DeltaTime)
{
	//CubeMap  ���µ�ǰ��CubeMap  ������Ⱦ������		CubeMap�ڸ�ǩ���ĵ�6��
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, RenderTarget->GPUShaderResourceView);
}

void FDynamicCubeMap::SetCubeMapViewportPosition(const fvector_3d& InCenterPoint)
{
	//����������ĸ���
	FTmpViewportCapture Capture(InCenterPoint);

	for (size_t i = 0; i < 6; i++) //���������
	{
		CubeMapViewport[i]->SetPosition(XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z)); //��CubeMap�ӿڽ�������λ��
		CubeMapViewport[i]->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.UP[i]);//��CubeMap�ӿڽ���ָ��Ŀ��
		CubeMapViewport[i]->BuildViewMatrix(0.3f);//��CubeMap�ӿڽ��й�����ͼ����
	}
}

bool FDynamicCubeMap::IsExitDynamicReflectionMesh()
{
	//�ж϶�̬������ͼ�����Ƿ������
	return GeometryMap->GetDynamicReflectionViewportNum() > 0;
}

void FDynamicCubeMap::BuildViewport(const fvector_3d& InCenterPoint)
{
	//����������ĸ��� �ӿڲ���ṹ��
	FTmpViewportCapture Capture(InCenterPoint);

	for (size_t i = 0; i < 6; i++)//������Ҫ6���� ���Ծ͹���6�������
	{
		//ͨ��push_back�������ͻ����ӿ�
		CubeMapViewport.push_back(CreateObject<GClientViewport>(new GClientViewport()));
		//�ͻ����ӿ�����					��������ͼ�ӿ�size-1���ص���һ������
		GClientViewport* Viewport = CubeMapViewport[CubeMapViewport.size() - 1];

		Viewport->SetPosition(XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z));//��������ԭ��
		Viewport->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.UP[i]);//����Ŀ��  ���뿴��Ŀ���λ�� ���벶��Ŀ�� ����UP��
		Viewport->SetFrustum(0.5f * XM_PI, 1.f, 0.1f, 10000.f);//������׶  ����FOV������90��(��/2) ��߱�(1:1������)  ��������0.1  Զ������1W    
		Viewport->BuildViewMatrix(0.3f);//�����ӿھ���
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
	//�����ģ�建��
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = Width;   //��	
	ResourceDesc.Height = Height;	//��
	ResourceDesc.Alignment = 0; //����
	ResourceDesc.MipLevels = 1; //mipmap�ȼ�
	ResourceDesc.DepthOrArraySize = 1;//��Ȼ������С
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//ά��  ������2D��ͼ

	ResourceDesc.SampleDesc.Count = 1; //������������
	ResourceDesc.SampleDesc.Quality =  0;//��������
	ResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //��ʽ
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//����

	D3D12_CLEAR_VALUE ClearValue;   //���ֵ
	ClearValue.DepthStencil.Depth = 1.f;//���
	ClearValue.DepthStencil.Stencil = 0;//ģ��
	ClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//��ʽ

	//CD3DX12������
	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	GetD3dDevice()->CreateCommittedResource(  //ͨ��D3D�������ύ����Դ
		&Properties,	//����
		D3D12_HEAP_FLAG_NONE, &ResourceDesc, //��������
		D3D12_RESOURCE_STATE_COMMON, &ClearValue,//���ֵ
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));//���ģ�建����

	//�������ģ����ͼ							�������ģ�建����	����Ϊ��  ���ģ������
	GetD3dDevice()->CreateDepthStencilView(DepthStencilBuffer.Get(), NULL, DSVDes);
	
	//��Դת��
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);
	//ͨ��ͼ�������б� ȥ������ԴΧ��
	GetGraphicsCommandList()->ResourceBarrier(1, &Barrier);
}

void FDynamicCubeMap::BuildDepthStencilDescriptor()
{
	//���ģ���������Ĵ�С ����ƫ����
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	//��������һ��ƫ��
	DSVDes = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),//��ȡ����ǰ�����ģ����ͼ
		1,	//ƫ��1 �����һ��λ���Ǹ�����ʹ�õ�
		DescriptorHandleIncrementSize);//ƫ���� ���ģ���������Ĵ�С
}

void FDynamicCubeMap::BuildRenderTargetDescriptor()
{
	//������ȾĿ����ͼ 
	BuildRenderTargetRTV();
	
	//����Shader��ɫ����Դ��ͼ��ֵ
	BuildRenderTargetSRV();

	//��ʼ��Target ��ȾĿ��  �� ��  ������ͼ��ʽ
	RenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void FDynamicCubeMap::BuildRenderTargetRTV()
{
	//��ȡRTV��ƫ�� ��ȾĿ����ͼ
	UINT RTVDescriptorSize = GetDescriptorHandleIncrementSizeByRTV();

	//RTV��ȾĿ����ͼ����ʼ
	auto RTVDesHeapStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();

	//ƫ�Ƶĵ�ַ��¼ ��ȾĿ����ͼ
	for (size_t i = 0; i < 6; i++)
	{
		RenderTarget->CPURenderTargetView[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE( //��VPU��ȾĿ����ͼ��CPU������ƫ��
			RTVDesHeapStart,	//������ȾĿ����ͼ��������ʼ��
			FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,//������
			RTVDescriptorSize);//��ȾĿ����ͼ��������С ����ƫ��
	}
}

void FDynamicCubeMap::BuildRenderTargetSRV()
{
	//��ȡCBV_SRV_UAV��ƫ��
	UINT CBVDescriptorSize = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	//ͨ������map�õ�CPU���������ѵ��׵�ַ  CPU SRV�׵�ַ  ��ɫ����Դ��ͼ
	auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	//ͨ������map�õ�GPU���������ѵ��׵�ַ  GPU SRV�׵�ַ  ��ɫ����Դ��ͼ
	auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

	RenderTarget->CPUShaderResourceView =		//ͨ����ȾĿ�꣬��CPU��ɫ����Դ��ͼ��ֵ
		CD3DX12_CPU_DESCRIPTOR_HANDLE(CPUSRVDesHeapStart, //ͨ��CPU����������и�ֵ  ��������CPU��SRV�׵�ַ
			GeometryMap->GetDrawTexture2DResourcesNumber() //����2D������ͼ����Դ���� 
			+ GeometryMap->GetDrawCubeMapResourcesNumber(), //���뾲̬��������ͼ����Դ���� 
			CBVDescriptorSize); //CBV��������С ֮�����ƫ��

	RenderTarget->GPUShaderResourceView =		//ͨ����ȾĿ�꣬��GPU��ɫ����Դ��ͼ��ֵ
		CD3DX12_GPU_DESCRIPTOR_HANDLE(GPUSRVDesHeapStart, //ͨ��GPU����������и�ֵ  ��������GPU��SRV�׵�ַ
			GeometryMap->GetDrawTexture2DResourcesNumber()  //����2D������ͼ����Դ���� 
			+  GeometryMap->GetDrawCubeMapResourcesNumber(), //���뾲̬��������ͼ����Դ���� 
			CBVDescriptorSize); ////CBV��������С ֮�����ƫ��
}



FDynamicCubeMap::FTmpViewportCapture::FTmpViewportCapture(const fvector_3d& InCenterPoint)
{
	//�����ӿڲ���ṹ��
	BuildViewportCapture(InCenterPoint);
}

void FDynamicCubeMap::FTmpViewportCapture::BuildViewportCapture(const fvector_3d& InCenterPoint)
{
	//����������ĸ���  ����˳��Ҫ����������ͼ˳��һ�� X -X Y -Y Z -Z
	TargetPoint[0] = fvector_3d(InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z); //x��ƫ��
	TargetPoint[1] = fvector_3d(InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z); //-x��ƫ��
	TargetPoint[2] = fvector_3d(InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z); //y��ƫ��
	TargetPoint[3] = fvector_3d(InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z); //-y��ƫ��
	TargetPoint[4] = fvector_3d(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f); //z��ƫ��
	TargetPoint[5] = fvector_3d(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f); //-z��ƫ��

	UP[0] = fvector_3d(0.f, 1.f, 0.f); // x
	UP[1] = fvector_3d(0.f, 1.f, 0.f); // -x
	UP[2] = fvector_3d(0.f, 0.f, -1.f);// y
	UP[3] = fvector_3d(0.f, 0.f, 1.f); // -y
	UP[4] = fvector_3d(0.f, 1.f, 0.f); // z
	UP[5] = fvector_3d(0.f, 1.f, 0.f); // -z

}
