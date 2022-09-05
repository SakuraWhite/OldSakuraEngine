#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "Core/RenderLayer.h"


//�㼶����
class FRenderLayerManage
{
	//���Է�������
	friend class FRenderLayer;//���Է�����Ⱦ�㼶
	friend struct FGeometry;//���Է��ʼ���
	friend struct FGeometryMap;//���Է��ʼ���map
public:
	FRenderLayerManage();
	~FRenderLayerManage();//����

	//��ʼ����ǰ�㼶
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState);
	//���¼��� ����������
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	virtual void PreDraw(float DeltaTime);//Ԥ�Ȼ���
	virtual void Draw(float DeltaTime);//����(����ʱ)
	virtual void PostDraw(float DeltaTime);//���ƽ���

	//��Ҫ��Ⱦ�ĸ��� �����ƶ���Ⱦ�Ǹ��㼶
	virtual void Draw(int InLayer, float DeltaTime);
	//�ҵ�Ҫ���ƵĶ���
	virtual void FindObjectDraw(float DeltaTime, int InLayer, const CMeshComponent* InKey);

	// ����Shader
	virtual void BuildShader();

	//PSO����״̬����(��Ⱦ�㼶)
	virtual void BuildPSO();

	//��Ⱦ����ӿ�
	virtual void Sort();//������Ⱦ���ȼ���������

	//��̬ ������Ⱦ�㼶����
	static std::shared_ptr<FRenderLayer> FindByRenderLayer(int InRenderLayer);
protected:
	//��̬��Ⱦ�㼶
	static std::vector<std::shared_ptr<FRenderLayer>> RenderLayers;
};

//ע����Ⱦ�㼶 ģ��
template<class T>
//������Ⱦ�㼶
std::shared_ptr<T> CreateRenderLayer()
{
	std::shared_ptr<T> RenderLayer = std::make_shared<T>();//����ʵ��
	RenderLayer->RegisterRenderLayer();//ע����Ⱦ�㼰

	return RenderLayer;
}