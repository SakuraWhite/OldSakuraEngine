#include "RenderLayerManage.h"
#include "RenderLayer/AlphaTestRenderLayer.h"
#include "RenderLayer/OpaqueRenderLayer.h"
#include "RenderLayer/TransparentRenderLayer.h"
#include "RenderLayer/BackgroundRenderLayer.h"
#include "RenderLayer/OpaqueReflectorRenderLayer.h"

//������̬����
std::vector<std::shared_ptr<FRenderLayer>> FRenderLayerManage::RenderLayers;

FRenderLayerManage::FRenderLayerManage()
{
	RenderLayers.clear();//��Ⱦ�㼶�������ɹ������

	//ע����Ⱦ�㼶
	CreateRenderLayer<FBackgroundRenderLayer>();//�����㼶
	//CreateRenderLayer<FAlphaTestRenderLayer>();//Alpha����
	CreateRenderLayer<FOpaqueRenderLayer>();//��͸���㼶
	CreateRenderLayer<FTransparentRenderLayer>();//͸���㼶
	CreateRenderLayer<FOpaqueReflectorRenderLayer>();//��͸�������

}

FRenderLayerManage::~FRenderLayerManage()
{
	RenderLayers.clear();//��Ⱦ�㼶�������ɹ������
}

void FRenderLayerManage::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState)
{
	for (auto& Tmp : RenderLayers)//������Ⱦ�㼶
	{
		//ע����Ⱦ�㼶
		Tmp->Init(InGeometryMap, InDirectXPipelineState);
	}
}

void FRenderLayerManage::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (auto& Tmp : RenderLayers)//������Ⱦ�㼶
	{
		//ע����¼���
		Tmp->UpdateCalculations(DeltaTime, ViewportInfo);
	}
}

void FRenderLayerManage::BuildShader()
{
	for (auto& Tmp : RenderLayers)//������Ⱦ�㼶
	{
		//����shder
		Tmp->BuildShader();
	}
}

void FRenderLayerManage::BuildPSO()
{
	for (auto& Tmp : RenderLayers)//������Ⱦ�㼶
	{
		//��������״̬PSO
		Tmp->BuildPSO();
	}
}

void FRenderLayerManage::Sort()
{
	//��������ʽ  
	auto CompRenderLayer =   //�Ƚ���Ⱦ�㼶
		[&](const std::shared_ptr<FRenderLayer>& InA, const std::shared_ptr<FRenderLayer>& InB)  //������Ⱦ�㼶
		->bool  //���ز���
	{
		//�ж���Ⱦ������
		return InA->GetRenderPriority() < InB->GetRenderPriority();
	};
 
	//��������		�㼶��ʼ				�㼶����         �Ա���Ⱦ�㼶
	std::sort(RenderLayers.begin(), RenderLayers.end(), CompRenderLayer);
}

std::shared_ptr<FRenderLayer> FRenderLayerManage::FindByRenderLayer(int InRenderLayer)
{
	//����Ѱ�Ҳ㼶
	for (auto& Tmp : RenderLayers)
	{
		if (Tmp->GetRenderLayerType() == InRenderLayer)//�жϻ�ȡ��Ⱦ�㼶�����Ƿ�==Ϊ�������Ⱦ�㼶
		{
			return Tmp;
		}
	}
	//���û����Ⱦ�㼶�򷵻�Null
	return NULL;
}

void FRenderLayerManage::PreDraw(float DeltaTime)
{
	//����Ѱ�Ҳ㼶
	for (auto& Tmp : RenderLayers)
	{
		Tmp->PreDraw(DeltaTime);
	}
}

void FRenderLayerManage::Draw(float DeltaTime)
{
	//����Ѱ�Ҳ㼶
	for (auto& Tmp : RenderLayers)
	{
		Tmp->Draw(DeltaTime);
	}
}

void FRenderLayerManage::PostDraw(float DeltaTime)
{
	//����Ѱ�Ҳ㼶
	for (auto& Tmp : RenderLayers)
	{
		Tmp->Draw(DeltaTime);
	}
}

void FRenderLayerManage::Draw(int InLayer, float DeltaTime)
{
	//����Ѱ�Ҳ㼶
	for (auto& Tmp : RenderLayers)
	{
		if (Tmp->GetRenderLayerType() == InLayer)//�ж��ҵ���Ҫ�Ĳ㼶
		{
			Tmp->Draw(DeltaTime);//������Ⱦ
			break;
		}
	}
}

void FRenderLayerManage::FindObjectDraw(float DeltaTime, int InLayer, const CMeshComponent* InKey)
{
	for (auto& Tmp : RenderLayers) //������Ⱦ�㼰
	{
		if (Tmp->GetRenderLayerType() == InLayer)//�ж��ҵ���Ҫ�Ĳ㼶
		{
			Tmp->FindObjectDraw(DeltaTime, InKey); //ָ��ģ����Ⱦ
			break;
		}
	}
}
