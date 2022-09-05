#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Mesh.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../Component/Light/SpotLightComponent.h"
#include "../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../Manage/LightManage.h"
#include "../../../../../Component/Light/Core/LightComponent.h"
#include "../../../RenderingTextureResourcesUpdate.h"
#include "../RenderLayer/RenderLayerManage.h"
#include "../../../../../Component/Sky/SkyConstantBuffer.h"
#include "../../../../../Component/Sky/FogComponent.h"

UINT MeshObjectCount = 0;//�ۼ� ģ�Ͷ������ 

FGeometryMap::FGeometryMap()
{
	Geometrys.insert(pair<int, FGeometry>(0, FGeometry()) );

	RenderingTexture2DResources = std::make_shared<FRenderingTextureResourcesUpdate>();//��ʼ��  ������ͼ����������ʵ��
	RenderingTexture2DResources->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURE2D);//������ͼ����Ϊ2D������ͼ


	RenderingCubeMapResources = std::make_shared<FRenderingTextureResourcesUpdate>();//��ʼ��  ��������ͼ����������ʵ��
	RenderingCubeMapResources->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURECUBE);//������ͼ����ΪCubeMap��������ͼ

	Fog = NULL;//��ʱû��������
}

FGeometryMap::~FGeometryMap()
{
	//��ģ�Ͷ��������ۼ�ֵ��������
	MeshObjectCount = 0;
}

void FGeometryMap::PreDraw(float DeltaTime)
{	
	//Ԥ��ʼ��
	//����������
	//DescriptorHeap.PreDraw(DeltaTime);
 	ID3D12DescriptorHeap* DescriptorHeap[] = { GetHeap() };//�����ǳ���������
	GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);//����������
}

void FGeometryMap::Draw(float DeltaTime)
{
	//��Ⱦ�����ӿ�
	//DrawViewport(DeltaTime);

	//��Ⱦ���Ƶƹ�
	DrawLight(DeltaTime);

	//��Ⱦ����������ͼ
	Draw2DTexture(DeltaTime);

	//������������ͼ
	DrawCubeMapTexture(DeltaTime);

	//���Ʋ���
	DrawMaterial(DeltaTime);

	//��������
	DrawFog(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{

}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{

	UpdateMaterialShaderResourceView(DeltaTime, ViewportInfo);//�������²�����ɫ����Դ��ͼ

	//���µƹ�
	FLightConstantBuffer LightConstantBuffer;//����(����)�ƹ�
	for (int i = 0; i < GetLightManage()->Lights.size(); i++)//�����ƹ� 
	{
		//�õ��ƹ����
		if (CLightComponent* InLightComponent = GetLightManage()->Lights[i])
		{
			//����ǿ�ȵ�fvector_3d���� ת��XMFLOAT3���� ��������ļ���
			fvector_3d LightIntensity = InLightComponent->GetLightIntensity();
			//��ȡ�ƹ�ǿ����Ϣ			ͨ���������ȡ�ƹ�ǿ��		���ƹ�ǿ�ȸ�ֵ
			LightConstantBuffer.SceneLights[i].LightIntensity = XMFLOAT3(LightIntensity.x, LightIntensity.y, LightIntensity.z);
			//��ȡ�ƹⷽ����Ϣ							ͨ���������ȡ�ƹ�ķ���
			LightConstantBuffer.SceneLights[i].LightDirection = InLightComponent->GetForwardVector();

			//��ȡ�ƹ�λ����Ϣ
			LightConstantBuffer.SceneLights[i].Position = InLightComponent->GetPosition();
			//��ȡ�ƹ�������Ϣ
			LightConstantBuffer.SceneLights[i].LightType = InLightComponent->GetLightType();
			//�жϵƹ�����
			switch (InLightComponent->GetLightType())
			{
			case ELightType::PointLight://����ǵ��Դ
			case ELightType::SpotLight://����Ǿ۹��
			{
				if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(InLightComponent))//ת������
				{
					//��ȡ���Դ˥����ʼ��
					LightConstantBuffer.SceneLights[i].StartAttenuation = InRangeLightComponent->GetStartAttenuation();
					//��ȡ���Դ˥���յ�
					LightConstantBuffer.SceneLights[i].EndAttenuation = InRangeLightComponent->GetEndAttenuation();
				}

				if (InLightComponent->GetLightType() == ELightType::SpotLight)//����ж�Ϊ�۹��
				{
					if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(InLightComponent))//ת������
					{
						//��þ۹���ڽǽǶ�										�Ƕ�ת��Ϊ���ȵ�API  �ѽǶ�ת��Ϊ���Ȳ���ʹ��																				
						LightConstantBuffer.SceneLights[i].ConicalInnerCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalInnerCorner());
						//��þ۹����ǽǶ�
						LightConstantBuffer.SceneLights[i].ConicalOuterCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalOuterCorner());
					}
				}

				break;
			}
			}
		}
	}
	LightConstantBufferViews.Update(0, &LightConstantBuffer);//���µƹ�(��ʱ)


	//�����ӿ�								�ӿ���Ϣ		��0ƫ��
	UpdateCalculationsViewport(DeltaTime, ViewportInfo, 0);
	
	//������Ч��
	if (Fog)
	{
		if (Fog->IsDirty()) //�ж��� �Ƿ񱻸��Ĺ�
		{

			FFogConstantBuffer FogConstantBuffer;//��ĳ���������
			{
				//���������ɫ
				fvector_color FogColor = Fog->GetFogColor();
				FogConstantBuffer.FogColor = XMFLOAT3(FogColor.r, FogColor.g, FogColor.b);

				FogConstantBuffer.FogStart = Fog->GetFogStart();//���������ʼ��
				FogConstantBuffer.FogRange = Fog->GetFogRange();//������ķ�Χ�뾶
				FogConstantBuffer.FogHeight = Fog->GetFogHeight();//������ĸ߶�
				FogConstantBuffer.FogTransparentCoefficient = Fog->GetFogTransparentCoefficient();//�������͸��ϵ��
			}
			FogConstantBufferViews.Update(0, &FogConstantBuffer);//���³���������

			Fog->SetDirty(false);//���и��ĺ� �������û�ȥ
		}
	}

}

void FGeometryMap::UpdateCalculationsViewport(float DeltaTime, const FViewportInfo& ViewportInfo, UINT InConstantBufferOffset)
{

	//�ӿ�ͶӰ����
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);//ת���ɾ��� �ӿ�ͶӰ �����������������������ǰ����һЩ��������
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);//ת���ɾ���   ��������Ĳü����� ��Զ�����棬FOV�ȵ�����


	//�����ӿ�ͶӰ����������
	//XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);//ת���ɾ��� �ӿ�ͶӰ
	XMMATRIX ViewProject = XMMatrixMultiply(ViewMatrix, ProjectMatrix);//���������������ӿ�ͶӰ�������
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProject));//�洢���Ǽ���Ľ��

	//�õ��ӿ�λ�� �������λ�ø�ֵ  
	ViewportTransformation.ViewportPosition = ViewportInfo.ViewPosition;

	//�����ӿ�
	ViewportConstantBufferViews.Update(InConstantBufferOffset, &ViewportTransformation);


}

void FGeometryMap::BuildDynamicReflectionMesh()
{
	for (auto& Tmp : GObjects)  //������ȡ���ж���������Դ
	{
		if (CMeshComponent* InMeshComponent = dynamic_cast<CMeshComponent*>(Tmp))//���������ת������
		{
			if (InMeshComponent->IsDynamicReflection())//�ж��Ƿ�Ϊ��̬����
			{
				DynamicReflectionMeshComponents.push_back(InMeshComponent);//�ռ���̬����
			}
		}
	}
}

void FGeometryMap::BuildFog()
{
	//��������  �ҵ����ʵ��
	for (auto& Tmp : GObjects) 
	{
		if (CFogComponent* InFogComponent = dynamic_cast<CFogComponent*>(Tmp))//������������ת������
		{
			Fog = InFogComponent;//�ֻ�����������
			break;
		}
	}
}

void FGeometryMap::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{

	for (auto& Tmp : Geometrys)//������������
	{
		//����ģ��			����ģ�͹�ϣ	ģ��		ģ������   ģ��key
		Tmp.second.BuildMesh(InMeshHash, InMesh, MeshData, Tmp.first);
	}
}

void FGeometryMap::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData)
{
	for (auto& Tmp : Geometrys)//������������
	{
		//����ģ��				����ģ��  ģ������   ģ��key
		Tmp.second.DuplicateMesh(InMesh, MeshData, Tmp.first);
	}
}


bool FGeometryMap::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	for (auto& Tmp : Geometrys)//����Geometrys����
	{
		
		if (Tmp.second.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex))//Ѱ��ͨ����ϣ��ģ����Ⱦ����
		{
			//�ҵ���Ⱦ���ݷ���true
			return true;
		}
	}

	return false;
}



void FGeometryMap::UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	//��ģ���и��²���
	FMaterialConstantBuffer MaterialConstantBuffer = {};

	//����������Ϣ
	for (size_t i = 0; i < Materials.size(); i++)
	{
		//������²���
		if (CMaterial* InMaterial = Materials[i])  //��ȡ�����Ĳ�����Ϣ 
		{
			//�жϲ����Ƿ���Ϊ������Դ�仯 ���������Դ�����仯�������ʸ��� 
			if (InMaterial->IsDirty())
			{
				//��ȡ�����еĻ�����ɫ
				fvector_4d InBaseColor = InMaterial->GetBaseColor();
				//���ʳ�����������ֵ ������ɫ
				MaterialConstantBuffer.BaseColor = XMFLOAT4(InBaseColor.x, InBaseColor.y, InBaseColor.z, InBaseColor.w);
				//���ʳ�����������ֵ �߹�
				fvector_3d InSpecularColor = InMaterial->GetSpecularColor();
				MaterialConstantBuffer.SpecularColor = XMFLOAT3(InSpecularColor.x, InSpecularColor.y, InSpecularColor.z);

				//���ʳ�����������ֵ �ֲڶ�
				MaterialConstantBuffer.Roughness = InMaterial->GetRoughness();

				//���ʳ�����������ֵ ������
				MaterialConstantBuffer.Refraction = InMaterial->GetRefractiveValue();

				//���ʳ�����������ֵ ��������
				MaterialConstantBuffer.MaterialType = InMaterial->GetMaterialType();

				//���ʳ�����������ֵ  ����������
				fvector_3d F0 = InMaterial->GetFresnelF0();//��ȡ���ʵķ���������
				MaterialConstantBuffer.FresnelF0 = XMFLOAT3(F0.x, F0.y, F0.z);

				//���ʳ�����������ֵ ͸����
				MaterialConstantBuffer.Transparency = InMaterial->GetTransparency();

				//�ⲿ��Դ����
				{
					//������ɫ��ͼ
					//�ⲿ����BaseColor��Դ	����ͨ����ǰ����Ⱦ������ԴѰ����Դ·����key	   ��ȡ��������ɫ��ͼ����key
					if (auto BaseColorTextureResourcesPtr = FindRenderingTexture(InMaterial->GetBaseColorIndexKey()))
					{
						//������������ֵ ������ɫ��ͼ����					ͨ�������жϺ��Ψһָ���������ͼID
						MaterialConstantBuffer.BaseColorIndex = (*BaseColorTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						//���û���ҵ��ⲿ��Դ·����key ��Ϊ�� -1
						MaterialConstantBuffer.BaseColorIndex = -1;
					}

					//������ͼ
					//�ⲿ���뷨����Դ							��ȡ��������ͼ����key
					if (auto NormalTextureResourcesPtr = FindRenderingTexture(InMaterial->GetNormalIndexKey()))
					{
						//������������ֵ ����������ͼ����					ͨ�������жϺ��Ψһָ���������ͼID
						MaterialConstantBuffer.NormalIndex = (*NormalTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						//���û���ҵ��ⲿ��Դ·����key ��Ϊ�� -1
						MaterialConstantBuffer.NormalIndex = -1;
					}

					//�߹���ͼ
					//�ⲿ����߹���Դ							��ȡ���߹���ͼ����key
					if (auto SpecularTextureResourcesPtr = FindRenderingTexture(InMaterial->GetSpecularKey()))
					{
						//������������ֵ �����߹���ͼ����					ͨ�������жϺ��Ψһָ���������ͼID
						MaterialConstantBuffer.SpecularIndex = (*SpecularTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						//���û���ҵ��ⲿ��Դ·����key ��Ϊ�� -1
						MaterialConstantBuffer.SpecularIndex = -1;
					}

				}

				//���ʾ���ת��
				XMMATRIX MaterialTransform = XMLoadFloat4x4(&InMaterial->GetMaterialTransform());
				//�����ʱ任������о���ת��Ϊ�о���
				XMStoreFloat4x4(&MaterialConstantBuffer.TransformInformation,
					XMMatrixTranspose(MaterialTransform));

				//һ�����¹����� �Ͱ��жϲ��ʶ�̬���¸�����Ϊfalse
				InMaterial->SetDirty(false);

				//���²��ʳ���������ͼ		��ȡ��������		���ʳ���������
				MaterialConstantBufferViews.Update(i, &MaterialConstantBuffer);
			}
		}
	}
}

void FGeometryMap::LoadTexture()
{
	def_c_paths Paths;//����һ��Ĭ�ϵ�C·�� api�ǽ̳��з�װ��
	init_def_c_paths(&Paths);//��ʼ�����·��

	char RootPath[] = "../SakuraEngine/Asset";//��·��
	find_files(RootPath, &Paths, true);//Ѱ��·���ļ�  �ֱ�����ָ����·��  ָ��������·��  �Ƿ�ݹ�

	for (int i = 0; i < Paths.index; i++)//����·���е��ļ���
	{
		//......./filename_cubemap.  �����ͼ�к�׺_cubemapΪ��������ͼ ʶ��ΪCubeMap��Ϊ����

		if (find_string(Paths.paths[i], ".dds", 0) != -1)//�ж��ļ���ʽ�Ƿ�Ϊ".dds" �����0�ſ�ʼ����Ѱ�� ����������Դ�����Ϊ�����������
		{
			//��λ��·����Դ
			normalization_path(Paths.paths[i]);

			wchar_t TexturePath[1024] = { 0 };
			char_to_wchar_t(TexturePath, 1024, Paths.paths[i]);//��charת��Ϊtchar תΪ���ַ�

			//������ͼ�Ƿ�Ϊcubemap
			if (wfind_string(TexturePath, L"_CubeMap.") != -1 ||
				wfind_string(TexturePath, L"_cubemap.") != -1) //�ж�������ͼ�Ƿ���_cubemap��׺
			{
				//������ΪCubeMap
				RenderingCubeMapResources->LoadTextureResources(TexturePath);
			}
			else
			{
				//�����ⲻ�����ȡΪTexture2D
				RenderingTexture2DResources->LoadTextureResources(TexturePath);
			}

		}

	}
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
	//����������		              	       
	DescriptorHeap.Build(
		GetDrawTexture2DResourcesNumber()//��ȡ2D������ͼ��Դ����
		+ GetDrawCubeMapResourcesNumber()//��ȡ��̬��������ͼ����
		+ 1);//�����Ƕ�̬��������ͼ

}

void FGeometryMap::BuildMeshConstantBuffer()
{	//����ģ�ͳ���������

	//����������������					    ���ֽ��������  ģ�Ͷ���任          ��û���ģ�Ͷ��������
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshObjectNumber());

	////CPU_������_���																					��ȡ��������CPU���������
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//
	////��������������								CPU�������	    ��û���ģ�Ͷ��������
	//MeshConstantBufferViews.BuildMeshConstantBuffer(DesHandle, GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildFogConstantBuffer()
{
	//����������������	 ��
	FogConstantBufferViews.CreateConstant(sizeof(FFogConstantBuffer), 1);
}

void FGeometryMap::BuildMaterialShaderResourceView()
{	//�������ʳ���������

	//�ռ�����
	//����shader����
	int ShaderIndex = 0;
	//��������Shader-Index�ĵط�
	//������Ⱦ�㼶
	for(auto& Tmp : FRenderLayerManage::RenderLayers)
	{
		for (auto& InData : Tmp->RenderDatas)//�ٱ��� �õ���Ⱦ����
		{
			//�Ƿ�������л�ȡ����
			if (auto InMaterials = InData.Mesh->GetMaterials())
			{
				//���������е�size �������е�size�ռ�
				for (size_t j = 0; j < InMaterials->size(); j++)
				{
					//���ò���index
					(*InMaterials)[j]->SetMaterialIndex(ShaderIndex);
					//�����������ռ���������
					Materials.push_back((*InMaterials)[j]);

					ShaderIndex++;//ÿ����һ�� shader��������
				}
			}
		}
	}
	//�Ƚ����ռ����� �ٽ��д���
	//����������������					            				
	MaterialConstantBufferViews.CreateConstant(
		sizeof(FMaterialConstantBuffer), //���ֽ��������  ���ʶ���任  
		GetDrawMaterialObjectNumber(), //��û��Ʋ��ʶ��������
		false);//���ﲻ���ڴ���� ���ﲢ���ǳ���������
}


	/*
	//CPU_������_���																					��ȡ��������CPU���������
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//��������������								CPU�������	    
	MaterialConstantBufferViews.BuildMeshConstantBuffer(
		DesHandle, 
		GetDrawMaterialObjectNumber(),//��û��Ʋ��ʶ��������
		GetDrawMeshObjectNumber());//���Ϸ�ģ�ͳ�����������ͬ���� ���ʳ�����������ģ��Ҫ���һ��ƫ��,��������ģ�͵Ļ����Ͻ���ƫ��
	*/


void FGeometryMap::BuildLightConstantBuffer()
{

	
		//����������������					    ���ֽ��������  �ƹ����任    ��û��Ʋĵƹ���������
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectNumber());

	////CPU_������_���																					��ȡ��������CPU���������
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//
	////��������������								CPU�������	    
	//LightConstantBufferViews.BuildMeshConstantBuffer(
	//	DesHandle,
	//	GetDrawLightObjectNumber(),//��û�ƹ���������
	//	GetDrawMeshObjectNumber());//��������ģ�ͻ������Ļ����Ͻ���ƫ��
	//
	//	//+ GetDrawMaterialObjectNumber() //�ڼ��ϲ��ʻ�����ƫ��
}

UINT FGeometryMap::GetDrawMeshObjectNumber()
{
	return Geometrys[0].GetDrawMeshObjectNumber();//��ȡ��Ⱦ���������
	
}

UINT FGeometryMap::GetDrawMaterialObjectNumber()
{
	//ֱ�ӷ��ز���size
	return Materials.size();
}

UINT FGeometryMap::GetDrawLightObjectNumber()
{
	return 1;
}

UINT FGeometryMap::GetDrawTexture2DResourcesNumber()
{
	return RenderingTexture2DResources->Size();//��ȡ��Ⱦ������Դ����  2d
}

UINT FGeometryMap::GetDrawCubeMapResourcesNumber()
{
	return RenderingCubeMapResources->Size();//��ȡ��Ⱦ������Դ���� CubeMap
}

UINT FGeometryMap::GetDynamicReflectionViewportNum()
{
	//6����̬���������
	return DynamicReflectionMeshComponents.size() * 6;
}

void FGeometryMap::BuildTextureConstantBuffer()
{
	//����������������  Texture2D
	RenderingTexture2DResources->BuildTextureConstantBuffer(
		DescriptorHeap.GetHeap(),0);		//������������

	//����CubeMap ������������
	RenderingCubeMapResources->BuildTextureConstantBuffer(
		DescriptorHeap.GetHeap(),        //������������
		GetDrawTexture2DResourcesNumber());//2D������ͼ���� ������ƫ��
}

void FGeometryMap::BuildViewportConstantBufferView(UINT InViewportOffset)
{
	//�����ӿڳ���������					    ���ֽ��������  �ӿڱ任          ����
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 
		1 +//��������ӿ�
		GetDynamicReflectionViewportNum() + //��ȡ��̬�����ӿ����� ��������ÿ��ģ�Ͷ���6����
		InViewportOffset);	//������ӿ�ƫ��
		

	////CPU_������_���																					��ȡ��������CPU���������
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//
	////�����ӿڳ���������						          
	//ViewportConstantBufferViews.BuildMeshConstantBuffer(
	//	DesHandle,  //CPU�������
	//	1,			//ƫ��	
	//	GetDrawMeshObjectNumber()//��ȡ���ƶ������� ����ģ�Ͷ�������
	//	+ GetDrawLightObjectNumber());//���Ƶƹ���������Ļ����Ͻ���ƫ��
	//
	//	//+GetDrawMeshObjectNumber()//���Ʋ��ʶ�������

}

bool FGeometryMap::IsStartUPFog()
{
	//�ж����Ƿ���ֵ
	return Fog != NULL;
}

std::unique_ptr<FRenderingTexture>* FGeometryMap::FindRenderingTexture(const std::string& InKey)
{
	//ͨ���ļ�����Ѱ����ͼ
	if (auto RenderingTexture2DPtr = RenderingTexture2DResources->FindRenderingTexture(InKey))
	{
		return RenderingTexture2DPtr;
	}
	else if (auto RenderingCubeMapPtr = RenderingCubeMapResources->FindRenderingTexture(InKey))
	{
		return RenderingCubeMapPtr;
	}
	return nullptr;
}

void FGeometryMap::DrawLight(float DeltaTime)
{
	////ƫ��  ��ǰ������ƫ��
	//UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//
	////��ȡGpu��ص����������
	//auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	//DesHandle.Offset(
	//	GetDrawMeshObjectNumber()//��ģ�ͻ���֮��ƫ��
	//	,DescriptorOffset);//��Gpu�����������ƫ��
	//
	////����ͼ�� ��������
	//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesHandle);
	// 
	//ͨ��ͼ�������б�ȥ������ͼ�������ĸ�����
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		2,//�ƹⳣ��������λ��Ϊ2
		LightConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());//ָ���ƹⳣ����������ַ
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	////ƫ��  ��ǰ������ƫ��
	//UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//
	////��ȡGpu��ص����������
	//auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	//DesHandle.Offset(
	//	GetDrawMeshObjectNumber() //��ȡģ������
	//	+ GetDrawLightObjectNumber(), //��ȡ�ƹ�����֮�����ƫ��  ��������Ⱦ�ӿ�
	//	DescriptorOffset);//��Gpu�����������ƫ��
	//
	////����ͼ�� ��������
	//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);

	//ͨ��ͼ�������б�ȥ������ͼ�������ĸ�����
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		1, //�ӿڳ���������λ��Ϊ1
		ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());//ָ���ӿڳ�����������ַ
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	

}

void FGeometryMap::DrawMaterial(float DeltaTime)
{
	//���ø���  ����ͼ�θ�Դ��ɫ����Դ��ͼ
	GetGraphicsCommandList()->SetGraphicsRootShaderResourceView(
		4,//������� �������е����
		MaterialConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());//�ڳ������������õ��ϴ�������  Ȼ���ȡ��GPU�ĵ�ַ
}

void FGeometryMap::Draw2DTexture(float DeltaTime)
{
	//ƫ��  ��ǰ������ƫ��  ��ȡ���������size
	UINT DescriptorOffset = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	//����2d��ͼ
		//��ȡGpu��ص����������
	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(0, DescriptorOffset);//��Gpu�����������ƫ��

	//����ͼ�� ��������
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, DesHandle);


}

void FGeometryMap::DrawCubeMapTexture(float DeltaTime)
{
	//ƫ��  ��ǰ������ƫ��	��ȡ���������size
	UINT DescriptorOffset = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	//������������ͼ
	//��ȡGpu��ص����������
	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	DesHandle.Offset(GetDrawTexture2DResourcesNumber(), DescriptorOffset);//��Gpu�����������ƫ��

	//����ͼ�� ��������
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, DesHandle);
	

}

void FGeometryMap::DrawFog(float DeltaTime)
{
	//����ͼ�� ��������   
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		3,
		FogConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

bool FGeometry::IsRenderingDataExistence(CMeshComponent* InKey)
{
	//�ҵ���Ӧ��Ⱦ�㼶											
	if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InKey->GetRenderLayerType()))
	{
		for (auto& Tmp : InRenderLayer->RenderDatas) //������Ϣ
		{
			if (Tmp.Mesh == InKey)  //�ҵ���Ϣ�ͷ���True
			{
				return true;
			}

		}
	}
	return false;
}

void FGeometry::BuildMesh(
	const size_t InMeshHash, //�����ϣ
	CMeshComponent* InMesh,	//����ģ��
	const FMeshRenderingData& MeshData,//����ģ������
	int InKey)//����ģ��Key
{
	if (!IsRenderingDataExistence(InMesh))   //�ж�ģ���Ƿ��Ѿ�����ӹ���
	{
		//�ҵ���Ӧ��Ⱦ�㼶											
		if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
		{
			//����Ⱦ�㼶���ȡ��Ⱦ��Ϣ
			InRenderLayer->RenderDatas.push_back(FRenderingData());//����µ�Mesh
			//�õ����ʼ������
			FRenderingData& InRenderingData = InRenderLayer->RenderDatas[InRenderLayer->RenderDatas.size() - 1];

			//����ע��
			InRenderingData.MeshObjectIndex = MeshObjectCount++;//ע��ģ�Ͷ�������
			InRenderingData.Mesh = InMesh;//ע��Mesh
			InRenderingData.MeshHash = InMeshHash;//ע���ϣ
			InRenderingData.GeometryKey = InKey;//ע��ģ��key

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

}

//������ڲ�����ģ��                      ����ģ��						ģ������				ģ��Key
void FGeometry::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData , int InKey)
{
	//����ģ��

	if (!IsRenderingDataExistence(InMesh))//�ж�ģ���Ƿ��Ѿ�����ӹ���
	{
		//�ҵ���Ӧ��Ⱦ�㼶											
		if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
		{
			//����Ⱦ�㼶���ȡģ����Ϣ
			InRenderLayer->RenderDatas.push_back(MeshData);//����µ�Mesh
			//�õ����ʼ������
			FRenderingData& InRenderingData = InRenderLayer->RenderDatas[InRenderLayer->RenderDatas.size() - 1];

			//����ע��
			InRenderingData.Mesh = InMesh;
			InRenderingData.MeshObjectIndex = MeshObjectCount++;//��Ⱦ�������ģ�Ͷ�����������ͬ���ۼ�
			InRenderingData.GeometryKey = InKey;//ע��ģ��key

		}

	}
}

bool FGeometry::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	//Ѱ��RenderData ����ҵ���Ⱦ�����������һ��
	auto FindMeshRenderingDataByHashSub = [&](std::shared_ptr<FRenderLayer> InRenderLayer)->FRenderingData*
	{
		for (auto& SubTmp : InRenderLayer->RenderDatas)//��ģ����Ⱦ���������ѽ��б���
		{
			if (SubTmp.MeshHash == InHash)//�ж�ģ�͵Ĺ�ϣ�Ƿ�һ��
			{
				//��������
				return &SubTmp;
			}
		}

		return NULL;
	};

	if (InRenderLayerIndex == -1)//������������
	{
		//ÿһ���㼶�����б���
		for (auto& Tmp : FRenderLayerManage::RenderLayers)
		{
			//���ͨ��Sub�����Ⱦ�㼶�ҵ���Ⱦdata
			if (FRenderingData* InRenderingData = FindMeshRenderingDataByHashSub(Tmp))
			{
				//����Ⱦ���ݸ�ģ��������
				MeshData = *InRenderingData;
				return true;
			}
		}
	}
		//��׼Ѱ����Ⱦ�㼶����	
	else if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer(InRenderLayerIndex))
	{
		//���ͨ��Sub�����Ⱦ�㼶�ҵ���Ⱦdata
		if (FRenderingData* InRenderingData = FindMeshRenderingDataByHashSub(InRenderLayer))
		{
			//����Ⱦ���ݸ�ģ��������
			MeshData = *InRenderingData;
			return true;
		}
	}

	return false;
}



UINT FGeometry::GetDrawMeshObjectNumber() const //��ȡ��Ⱦ��������
{
	//��ȡ��Ⱦ���� �ۼӣ�����һ������ Ȼ�����++��
	return MeshObjectCount;

	int Count = 0;//����
	for (auto& Tmp : FRenderLayerManage::RenderLayers) //ͨ����Ⱦ�㼶���б���
	{
		for (auto& SubTmp : Tmp->RenderDatas)
		{
			Count++;
		}
	}

	return Count;
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


