#pragma once//��ֹ�ظ�����
#include "../../../../../Interface/DirectXDeviceInterfece.h"


//��¼�ṩ��Ⱦ��ص����ݵĽӿ�    
struct  FDirectXRootSignature :public IDirectXDeviceInterfece_Struct //�̳���DirectXDeviceInterfece
{	//��ǩ��

	FDirectXRootSignature();

	void BuildRootSignature();//������ǩ��

	void PreDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h��Ԥ��Ⱦ�ӿ� 
	void Draw(float DeltaTime);//��ģ����ʹ��Rendeing.h����Ⱦ�ӿ�  ����ʱ
	void PostDraw(float DeltaTime);//��ģ����ʹ��Rendeing.h�Ľ�����Ⱦ�ӿ� poatDraw

	ID3D12RootSignature* GetRootSignature() { return RootSignature.Get(); }    //��ȡ��ǩ��

private:
	//��ǩ��
	ComPtr<ID3D12RootSignature>  RootSignature;
};