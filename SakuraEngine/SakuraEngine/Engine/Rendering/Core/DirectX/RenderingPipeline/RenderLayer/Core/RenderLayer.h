#pragma once
#include "../../../../../../Interface/DirectXDeviceInterfece.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/RenderingData.h"
#include "../../../../../../Shader/Core/ShaderType.h"

struct FDirectXPipelineState;
struct FGeometryMap;
struct FRenderingData;
struct FViewportInfo;
//��Ⱦ��
class FRenderLayer
	:public IDirectXDeviceInterfece
	,public std::enable_shared_from_this<FRenderLayer>//����ָ�� ���Ա���һ����ָ��
{
	
	friend struct FGeometry;//ֱ�ӽ��з���
	friend struct FGeometryMap;//ֱ�ӽ��з���
public:
	FRenderLayer();

	//����ע�ỷ�� ��ʼ��
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState);

	virtual void PreDraw(float DeltaTime);//Ԥ�Ȼ���
	virtual void Draw(float DeltaTime);//����(����ʱ)
	virtual void PostDraw(float DeltaTime);//���ƽ���

	//��Ⱦ�������ݣ�
	virtual void DrawObject(float DeltaTime, const FRenderingData& InRenderingData);
	//ָ����Ⱦ����
	virtual void FindObjectDraw(float DeltaTime, const CMeshComponent* InKey);

	//PSO����״̬(��Ⱦ�㼶) 
	virtual void BuildPSO();

	//ע����Ⱦ��
	void RegisterRenderLayer();

	//����Shader��
	virtual void BuildShaderMacro(std::vector<ShaderType::FShaderMacro> &InMacro);

	//���¼��� ����������
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
public:
	//��ȡ��Ⱦ�������ȼ�
	const UINT GetRenderPriority()const { return RenderPriority; }
	
public:
	// ����Shader
	virtual void BuildShader() {};

	//��ȡģ����Ⱦ�㼶����
	virtual int GetRenderLayerType() const = 0;
protected:
	UINT RenderPriority;//��Ⱦ����  ���ȼ�

protected:
	//��Ⱦ�㼶��Ӧ��Shader
	FShader VertexShader; //����Shader
	FShader PixelShader;	//����Shader

	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;//Ԫ������

	std::vector<FRenderingData> RenderDatas;//��Ⱦ��

	//�ӿ�
	FGeometryMap* GeometryMap;//������Ϣ
	FDirectXPipelineState* DirectXPipelineState;//DX��Ⱦ����״̬
};