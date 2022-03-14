#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"
CMesh::CMesh() 
	:VertexSizeInBytes(0) //��ʼ�����㻺�����ֽڴ�С
	,VertexStrideInBytes(0) //��ʼ��ÿ������ֽڴ�С
	,IndexSizeInBytes(0)     //��ʼ��Index�ֽڴ�С
	,IndexFormat(DXGI_FORMAT_R16_UINT) //��ʼ��Index�ĸ�ʽ���������һ��R16��INT���� ��С��ռ��
	,IndexSize(0)//���ڱ���һ�ݶ�������
	,WorldMatrix(FObjectTransformation::IdentityMatrix4x4()) //����ռ����� ��λ����ʼ�� 
	,ViewMatrix(FObjectTransformation::IdentityMatrix4x4()) //��Ļ�ռ����굥λ��
	,ProjectMatrix(FObjectTransformation::IdentityMatrix4x4())//����ռ����굥λ��
{

}
const float PI = 3.1415926535f; //���妰 3.14
void CMesh::Init()
{
	//��ǰ��Ļ�ĺ��ݱ� һ����16��9
	//�����Ƕ��������ӿڵ���Ļ�Ŀ�������Ļ�ӿڵĸ� ͬʱ�ǵô�INT����ת��ΪFloat����
	float AspectRatio = (float)FEngineRenderConfig::GetRenderConfig()->ScrrenWidth / (float)FEngineRenderConfig::GetRenderConfig()->ScrrenHight;
	
	//ͶӰ����
	//���ǽ�3D�ռ��ת����2D����ת����3D  ���ǻ�����Ұ��������͸��ͶӰ����
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * PI, //�Ի���Ϊ��λ�����϶��µ��ӳ��ǡ�������45��
		AspectRatio,//��ͼor��Ļ�ռ� X:Y ���ݺ��
		1.0f,    //��������ƽ��ľ��� ���������
		1000.f);  //�����ƽ�浽Զ������ľ��� 
	XMStoreFloat4x4(&ProjectMatrix,Project);//����ռ��ת��
}

void CMesh::BuildMesh(const FMeshRendingData* InRendingData)
{ 
	//����CBV������
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
	HeapDesc.NumDescriptors = 1;//���ٸ�����
	HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; //��Ҫָ��һ��Type ����ָ��ջ�����Ͱ���CBV������״̬
	HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//���õ�ǰ��־
	HeapDesc.NodeMask = 0;
	//����������
	GetD3dDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&CBVHeap));
	
	//�����������Ĺ���
//////////////////////////////////////////////////////////////////////////////////
	objectConstants = make_shared<FRenderingResourcesUpdate>(); //������ǰ����
	
	objectConstants->Init(GetD3dDevice().Get(),sizeof(FObjectTransformation),1);//�����������ĳ�ʼ��
	
	D3D12_GPU_VIRTUAL_ADDRESS ObAddr = objectConstants.get()->GetBuffer()->GetGPUVirtualAddress();   //�õ�������ַ ����ͨ��GetBuffer��ȡ��GPU�ĵ�ַ
 	
	//��������
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
	CBVDesc.BufferLocation = ObAddr;//��������ǰ��Ҫ������Buffer��ַ
	CBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize();//���Buffer��size
	//ͨ�����D3D��������������������
	GetD3dDevice()->CreateConstantBufferView( 
		&CBVDesc,  //��������
		CBVHeap->GetCPUDescriptorHandleForHeapStart()); //ͨ��CBVջ��ȡ��CPU�������

	//��ǩ������  //��ǩ��������Shader���� ��ǩ�����԰�����ɫ���ṩ��Ⱦ��ˮ�������������Դ
	CD3DX12_ROOT_PARAMETER RootParam[1];

	//CBV������
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCBV;     //��������CBV�ı�
	//Param��ʼ��
	DescriptorRangeCBV.Init(					
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV, //ָ����ΧΪCBV   CBV=������������ͼ
		1, 0);//�ڶ����������ж��ٸ�������  �����������ǻ�����һ����ɫ���ļĴ��� ������0��
	RootParam[0].InitAsDescriptorTable(1,&DescriptorRangeCBV);//��һ�������ǵ�ǰ�ķ�Χ �ڶ���ָ����Ӧ������
	//�������ṹ
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		1,//ָ���ж��ٸ�����
		RootParam,//ָ���Ĳ���
		0,//�ж��ٸ���̬����
		nullptr,//ָ����̬����ʵ��
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);//��ǩ���Ĳ���ѡ��
	//���������л�
	ComPtr<ID3DBlob> SerializeRootSignature;
	ComPtr<ID3DBlob> ErrorBlob;//���л�����
	//���л���ǩ��
	D3D12SerializeRootSignature(
		&RootSignatureDesc,    //������
		D3D_ROOT_SIGNATURE_VERSION_1,	//���汾
		SerializeRootSignature.GetAddressOf(),//ָ�����������л�������
		ErrorBlob.GetAddressOf());   //ָ�������Ʊ���
	//�жϱ�����ǩ���Ƿ��ǿյ�
	if (ErrorBlob)  
	{
		Engine_Log_Error("%s", (char*)ErrorBlob->GetBufferPointer());//����������ǿյ� ��ô��ӡ����
		
	}
									 
	//ͨ������������ǩ��
	GetD3dDevice()->CreateRootSignature(
		0, //����Ե���GPU���в���
		SerializeRootSignature->GetBufferPointer(),//ָ�����л��Ļ�ȡBuffer��ָ��
		SerializeRootSignature->GetBufferSize(),////ָ�����л��Ļ�ȡBuffer��Size
		IID_PPV_ARGS(&RootSignature));//������ǩ��


	//����Shader HLSL
/////////////////////////////////////////////////////////////////////////////////
	VertexShader.BuildShaders(				//����������ɫ��
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",  //Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"VertexShaderMain",//��ں�����
		"vs_5_0");			//ʹ�õİ汾

	PixelShader.BuildShaders(				//����������ɫ��
		L"../SakuraEngine/Shader/HelloHLSL.hlsl",  //Ѱ���ⲿ��hlsl��ɫ�������ļ�
		"PixelShaderMain",//��ں�����
		"ps_5_0");//����������ɫ��
	
	//������ λ����ɫ���ߵȵ���Ϣ
	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},//ָ��λ�� Ԫ������  ָ����ʽ  �����  �ڴ��е�ƫ��  ��ǰ��λ��
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}//ָ����ɫ Ԫ������  ָ����ʽ  �����  �ڴ��е�ƫ��  ��ǰ��λ��
		
	};

	//����ģ��
//////////////////////////////////////////////////////////////////////////////////



	VertexStrideInBytes = sizeof(FVertex);//ָ��ÿ����Ĵ�С
	IndexSize = InRendingData->IndexDeta.size();//����һ�ݶ�������
	//��ȡģ�����ݵĴ�С
	VertexSizeInBytes = InRendingData->VertexDeta.size() * VertexStrideInBytes;//�õ�ǰ�������ݵ�Sizeȥ�����Լ����Ķ�������
	IndexSizeInBytes = IndexSize * sizeof(uint16_t);//ͬ����������sizeȥ���Լ�������������

	//�������㻺����     D3DCreateBlob���Դ��������ƵĻ����������д���VertexSize��Ϊ��С��Ȼ����CPUVertexBufferPtr CPU�����ָ��
	ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
	//ͨ��memcpy�õ�һЩ����������CPU����Buffer���׵�ַ   ���CPUVertexBufferPtr����
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRendingData->VertexDeta.data(), VertexSizeInBytes);
	
	//����Index������
	ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRendingData->IndexDeta.data(), IndexSizeInBytes);

	//����Ĭ�ϻ�����
	//GPU����Buffer������
	GPUVertexBufferPtr = ConstructDefaultBuffer(
		VertexBufferTmpPtr,			//�����ϴ�������
		InRendingData->VertexDeta.data(), //��������ָ��
		VertexSizeInBytes); //�������ݴ�С
	//GPUIndex������
	GPUIndexBufferPtr = ConstructDefaultBuffer(
		IndexBufferTmpPtr,		    //Index�ϴ�������
		InRendingData->IndexDeta.data(), //Index����ָ��
		IndexSizeInBytes); //Index���ݴ�С

	//PSO��ˮ��״̬����   ��ˮ�߰�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  GPSDesc;  //������ǰ����״̬
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));//��ʼ��
	//�����벼��
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc.data();//�����Ǹ�ָ��
	GPSDesc.InputLayout.NumElements = (UINT)InputElementDesc.size(); //����������  ����������Ǵ�С
	//�󶨸�ǩ��
	GPSDesc.pRootSignature = RootSignature.Get(); //��ǩ��
	//�󶨶���Shader
	GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader.GetBufferPointer());//������������ֽڵķ�ʽ���� ����Ҫת���ֽ�
	GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
	//������Shader
	GPSDesc.PS.pShaderBytecode = PixelShader.GetBufferPointer();
	GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();
	//���ù�դ��״̬
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	
	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//���ֿ�ģʽ��ʾ
	
	GPSDesc.SampleMask = UINT_MAX;//���״̬�µ�ʾ���ɰ� ��Ϊ���ز�������ǿ��Բ���32λ������ ����UINT_MAX�������е�����
	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//ͼԪ���������ͣ� �������������εķ�ʽ�γ���
	GPSDesc.NumRenderTargets = 1;//ָ����ȾĿ��
	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); //ָ�����״̬
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//���ģ��
	

	GPSDesc.SampleDesc.Count = GetEngine()->GetDXGISampleCount();//�ж϶��ز���������
	GPSDesc.SampleDesc.Quality = GetEngine()->GetDXGISampleQuality();//�ж϶��ز���������
	

	GPSDesc.RTVFormats[0] = GetEngine()->GetBackBufferFormat();   //����RTV�ĸ�ʽ ������ʽ�󻺳���
	GPSDesc.DSVFormat = GetEngine()->GetDepthStencilFormat();//����DSV  ���ģ��
	//��ȡD3D����
	ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)));//��ȡGraph����״̬

}
void CMesh::PostDraw(float DeltaTime)
{
	XMUINT3 MeshPos = XMUINT3(5.0f, 5.0f, 5.0f); //�Լ�������������

	XMVECTOR Pos = XMVectorSet(MeshPos.x, MeshPos.y, MeshPos.z,1.0f); //�����������
	//��Ļ�ӿ����
	XMVECTOR ViewTarget =  XMVectorZero();//��Ļ�ܿ�����Ŀ��
	XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);//ͷ��ָ���Ϸ�
	XMMATRIX viewLookAt = XMMatrixLookAtLH(Pos,ViewTarget, ViewUp); //��Ļ�����ķ���
	XMStoreFloat4x4(&ViewMatrix,viewLookAt);//ת��Ϊ��Ļ�ռ�
	//�ռ�ת�����
	XMMATRIX ATRIXWorld = XMLoadFloat4x4(&WorldMatrix);  //ת��Ϊ����ռ�����
	XMMATRIX ATRIXProject = XMLoadFloat4x4(&ProjectMatrix);//ת��Ϊ����ռ�����
	XMMATRIX WVP = ATRIXWorld * viewLookAt * ATRIXProject;//�ü�orͶӰ����
	//��WVP�����ڳ����������� ��ʱ�̸���WVP
	FObjectTransformation ObjectTransformation;
	XMStoreFloat4x4(&ObjectTransformation.World,XMMatrixTranspose(WVP));//����ת��
	objectConstants->Update(0, &ObjectTransformation);//����
}

void CMesh::Draw(float DeltaTime)
{
	//����������
	ID3D12DescriptorHeap* DescriptorHeap[] = {CBVHeap.Get()};//�����ǳ���������
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);//����������
	//���ø�ǩ��
	GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());//��ǩ����ÿ֡����Ҫ���õ�
	


	D3D12_VERTEX_BUFFER_VIEW VBV = GetVertexBufferView();//����Buffer��ͼ VertexBufferView���VBV
	//����Ⱦ��ˮ���ϵ�����ۣ�����������װ����׶δ��붥������
	GetGraphicsCommandList()->IASetVertexBuffers(
		0, //��ʼ����� 0~15 һ��16������
		1, //������۰󶨵����� ���㻺���������� �����������Ϊk ��ô���ǰ󶨵Ļ���������n �ǹ�ʽ����k+n-1  ������0������ۣ���ô�󶨻���������n=k+1 Ϊ1
		&VBV);//���붥��Buffer��ͼ


	D3D12_INDEX_BUFFER_VIEW IBV = GetIndexBufferView();//IndexBuffer��ͼ IndexBufferView���IBV
	//����Ⱦ��ˮ���ϵ�����ۣ�����������װ����׶δ���Index  ֻ�ǰ� ��������������Ⱦ
	GetGraphicsCommandList()->IASetIndexBuffer(&IBV);
	
	//�ڻ���ǰ������һ������Ҫ������һ��ͼԪ���ǻ��Ƴ� ��������� ������ 
	GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//������Ƴ�
	//�ڻ���ǰ �������������õ���ǩ����  
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());//���õ�CBV���׵�ַ����

	//��������Ⱦ�ӿ�  ���API���԰�������ȥ����������ʵ������Ԫ �����Ļ���
	GetGraphicsCommandList()->DrawIndexedInstanced(   
		IndexSize, //��Ⱦģ�͵Ķ�������
		1,  //ʵ������������Ŀǰֻ��һ��
		0,  //������Ҫ���붥�㻺���������Ƶ������ĵ�һ�� ����0
		0,	//GPU��������������ȡ�ĵ�һ��������λ��
		0); //�ڴӶ��㻺������ȡÿһ��ʵ������֮ǰ���ӵ�ÿ��������ֵ��
}



void CMesh::PreDraw(float DeltaTime)
{
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

CMesh* CMesh::CreateMesh(const FMeshRendingData* InRendingData)
{
	CMesh*InMesh = new CMesh();
   InMesh->BuildMesh(InRendingData);
   return InMesh;
}

D3D12_VERTEX_BUFFER_VIEW CMesh::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV; //����Buffer
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();//ָ��Bufferλ��
	VBV.SizeInBytes = VertexSizeInBytes;//ָ���������ֽڴ�С
	VBV.StrideInBytes = VertexStrideInBytes;//ָ��ÿ����Ĵ�С
	return VBV;
}

D3D12_INDEX_BUFFER_VIEW CMesh::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV; //����INDEX
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();//ָ��INDEXBufferλ��
	IBV.SizeInBytes = IndexSizeInBytes;//ָ��INDEX�ֽڴ�С
	IBV.Format = IndexFormat;//Index��ʽ
	return IBV;
}

FObjectTransformation::FObjectTransformation()
	:World(FObjectTransformation::IdentityMatrix4x4())//��λ����
{
}

XMFLOAT4X4 FObjectTransformation::IdentityMatrix4x4()
{
	return XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);//4x4��λ����
}