#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"
#include "../StaticSampler/StaticSamplerObject.h"

//��¼�ṩ��Ⱦ��ص����ݵĽӿ�    
struct  FDirectXRootSignature :public IDirectXDeviceInterfece_Struct //�̳���DirectXDeviceInterfece
{	//��ǩ��

	FDirectXRootSignature();

	void BuildRootSignature(UINT InTextureNum = 1);//������ǩ��  ������ͼ���� ���û����ͼ��ռλ1

	void PreDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	void Draw(float DeltaTime);//��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw

	ID3D12RootSignature* GetRootSignature() { return RootSignature.Get(); }    //��ȡ��ǩ��

private:
	//��ǩ��
	ComPtr<ID3D12RootSignature>  RootSignature;
	//��̬����������
	FStaticSamplerObject StaticSamplerObject;
};