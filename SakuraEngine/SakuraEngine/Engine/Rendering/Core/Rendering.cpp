#include "Rendering.h"
#include "../../Debug/EngineDebug.h"
#include "../../Platform/Windows/WindowsEngine.h"
#include "../../Rendering/Enigne/DurectX/Core/DirectXRenderingEngine.h"

//vector<IRenderingInterface*> IRenderingInterface::RenderingInterface;//.h��ľ�̬���� ��Ҫ����

IRenderingInterface::IRenderingInterface()
{

	 
	//RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	/*
	for (vector<IRenderingInterface*>::const_iterator Iter = RenderingInterface.begin();
		Iter != RenderingInterface.end(); 
		++Iter)     //ʹ�ñ����ķ�ʽ���ж�ʲôʱ�����
	{
		if (*Iter == this) 
		{
			RenderingInterface.erase(Iter);//�Ƴ�
			break;
		}
	}*/
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::PreDraw(float DeltaTime)//Ԥ��Ⱦ
{

	//�������ǵ������б�
//	ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), NULL));

}

void IRenderingInterface::Draw(float DeltaTime) //��Ⱦ����
{

}

void IRenderingInterface::PostDraw(float DeltaTime)//��Ⱦ����
{
}

